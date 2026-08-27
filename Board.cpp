#include "Board.h"
#include <cstdlib>
#include <ctime>
#include <random>
#include <ranges>
#include <algorithm>

Board::Board(int width, int height, int cellSize) :_width(width), _height(height), _cellSize(cellSize), _wrapEdges(false) {
    _cells.resize(static_cast<size_t>(_width) * _height, 0);
    _nextCells.resize(static_cast<size_t>(_width) * _height, 0);

    float shapeSize = static_cast<float>(cellSize);
    if (cellSize > 2) {
        shapeSize -= 1.0f; 
    }

    _cellShape.setSize(sf::Vector2f(shapeSize, shapeSize));
    _cellShape.setOutlineThickness(0);

    _isVisible = true;
}

int Board::getIndex(int x, int y) const {
    return y * _width + x;
}

void Board::toggleCell(int x, int y) {
    if (x >= 0 && x < _width && y < _height) {
        int idx = getIndex(x, y);
        _cells[idx] = _cells[idx] ? 0 : 1;
    }
}

void Board::setCellState(int x, int y, bool state) {
    if (x >= 0 && x < _width && y >= 0 && y < _height) {
        _cells[getIndex(x, y)] = state ? 1 : 0;
    }
}

void Board::clearBoard() {
    std::ranges::fill(_cells, 0);
}

void Board::randomizeBoard(int percentage) {
    clearBoard();
    percentage = std::clamp(percentage, 0, 100);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 99);

    int maxCells = _width * _height;

    for (int i = 0; i < maxCells; i++) {
        if (dist(gen) < percentage) {
            _cells[i] = 1;
        }
    }
}

int Board::countAliveNeighbours(int x, int y) const {
    int counter = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;

            int col = x + i;
            int row = y + j;

            if (_wrapEdges) {
                // wrap around using mod
                col = (col + _width) % _width;
                row = (row + _height) % _height;
                counter += _cells[getIndex(col, row)];
            }
            else {
                // walls
                if (col >= 0 && col < _width && row >= 0 && row < _height) {
                    counter += _cells[getIndex(col, row)];
                }
            }
        }
    }
    return counter;
}

void Board::updateBoard() {
    for (int y = 0; y < _height; y++) {
        for (int x = 0; x < _width; x++) {
            int idx = getIndex(x, y);
            int aliveNeighbours = countAliveNeighbours(x, y);
            bool isAlive = _cells[idx];

            if (isAlive && (aliveNeighbours == 2 || aliveNeighbours == 3)) {
                _nextCells[idx] = 1;
            }
            else if (!isAlive && aliveNeighbours == 3) {
                _nextCells[idx] = 1;
            }
            else {
                _nextCells[idx] = 0;
            }
        }
    }
    // swapping vectors instead of copying
    std::swap(_cells, _nextCells);
}

void Board::drawSelf(sf::RenderWindow& window) {
    for (int y = 0; y < _height; y++) {
        for (int x = 0; x < _width; x++) {
            int idx = getIndex(x, y);

            _cellShape.setPosition(static_cast<float>(x * _cellSize), static_cast<float>(y * _cellSize));

            if (_cells[idx]) {
                _cellShape.setFillColor(sf::Color::White);
            }
            else {
                _cellShape.setFillColor(sf::Color::Black);
            }

            window.draw(_cellShape);
        }
    }
    if (_currentGhost != nullptr) {
        sf::RectangleShape ghostShape(_cellShape.getSize());
        ghostShape.setFillColor(sf::Color(255, 120, 0, 150));

        for (int y = 0; y < _currentGhost->height; ++y) {
            for (int x = 0; x < _currentGhost->width; ++x) {
                
                if (_currentGhost->data[static_cast<size_t>(y) * _currentGhost->width + x]) {
                    int drawX = _ghostX + x;
                    int drawY = _ghostY + y;

                    
                    ghostShape.setPosition(static_cast<float>(drawX * _cellSize), static_cast<float>(drawY * _cellSize));
                    window.draw(ghostShape);
                }
            }
        }
    }
}



void Board::setWrapEdges(bool wrap) {
    _wrapEdges = wrap;
}

bool Board::getWrapEdges() const {
    return _wrapEdges;
}

int Board::getBoardSize() const {
    return _width;
}

void Board::setGhost(const Preset* preset, int cellX, int cellY) {
    _currentGhost = preset;
    _ghostX = cellX;
    _ghostY = cellY;
}

void Board::clearGhost() {
    _currentGhost = nullptr;
}

void Board::pasteGhost() {
    if (!_currentGhost) return;

    for (int y = 0; y < _currentGhost->height; ++y) {
        for (int x = 0; x < _currentGhost->width; ++x) {
            if (_currentGhost->data[static_cast<size_t>(y) * _currentGhost->width + x]) {
                int targetX = _ghostX + x;
                int targetY = _ghostY + y;

                if (targetX >= 0 && targetX < _width && targetY >= 0 && targetY < _height) {
                    _cells[getIndex(targetX, targetY)] = 1;
                }
            }
        }
    }
}

// Find bounding box of active cells to avoid saving empty space
Preset Board::extractPreset(const std::string& name) const {
    Preset p;
    p.name = name;

    int minX = _width, maxX = -1;
    int minY = _height, maxY = -1;

    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
            if (_cells[getIndex(x, y)]) {
                if (x < minX) minX = x;
                if (x > maxX) maxX = x;
                if (y < minY) minY = y;
                if (y > maxY) maxY = y;
            }
        }
    }

    if (maxX < 0) return p;

    p.width = (maxX - minX) + 1;
    p.height = (maxY - minY) + 1;
    p.data.resize(static_cast<size_t>(p.width) * p.height, 0);

    for (int y = 0; y < p.height; ++y) {
        for (int x = 0; x < p.width; ++x) {
            p.data[static_cast<size_t>(y) * p.width + x] = _cells[static_cast<size_t>(getIndex(minX + x, minY + y))];
        }
    }
    return p;
}