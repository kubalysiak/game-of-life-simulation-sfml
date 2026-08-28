#pragma once
#include "Drawable.h"
#include "Preset.h"
#include <vector>
#include <cstdint> 
#include <string>

class Board : public Drawable {
public:
    Board(int width, int height, int cellSize);

    void updateBoard();
    void randomizeBoard(int percentage);
    void clearBoard();
    void toggleCell(int x, int y);
    void setCellState(int x, int y, bool state);

    void setWrapEdges(bool wrap);
    bool getWrapEdges() const;

    void drawSelf(sf::RenderWindow& window) override;
    int getBoardSize() const;
    // Preview mechanics for pasting presets
    void setGhost(const Preset* preset, int cellX, int cellY);
    void clearGhost();
    void pasteGhost();

    // Bounding box of active cells
    Preset extractPreset(const std::string& name) const;
private:
    int _width;
    int _height;
    int _cellSize;
    bool _wrapEdges = false;
    // Render optimization
    sf::Texture _boardTexture;
    sf::Sprite _boardSprite;
    std::vector<sf::Uint8> _pixelData;
    // Double buffer
    std::vector<uint8_t> _cells;
    std::vector<uint8_t> _nextCells;
 
    const Preset* _currentGhost = nullptr;
    int _ghostX = 0;
    int _ghostY = 0;
    // Static geometry for the background grid
    sf::VertexArray _gridLines;

    int getIndex(int x, int y) const;
    int countAliveNeighbours(int x, int y) const;
};