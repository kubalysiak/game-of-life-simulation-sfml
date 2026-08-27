#include "Button.h"

Button::Button(float x, float y, float width, float height) {
    _baseColor = sf::Color(70, 70, 70);
    _shape.setFillColor(_baseColor);
    _shape.setOutlineColor(sf::Color::White);
    _shape.setOutlineThickness(1);
    _shape.setPosition(x, y);
    _shape.setSize(sf::Vector2f(width, height));
    _isVisible = true;
}

void Button::drawSelf(sf::RenderWindow& window) {
    window.draw(_shape);
}

void Button::setBaseColor(sf::Color color) {
    _baseColor = color;
    _shape.setFillColor(_baseColor);
}

void Button::setOutlineColor(sf::Color color) {
    _shape.setOutlineColor(color);
}

sf::FloatRect Button::getGlobalBounds() const {
    return _shape.getGlobalBounds();
}
void Button::updateHover(sf::Vector2f mousePos) {
    if (_shape.getGlobalBounds().contains(mousePos)) {
        sf::Color hoverColor(
            std::min(255, _baseColor.r + 30),
            std::min(255, _baseColor.g + 30),
            std::min(255, _baseColor.b + 30)
        );
        _shape.setFillColor(hoverColor);
    }
    else {
        _shape.setFillColor(_baseColor);
    }
}