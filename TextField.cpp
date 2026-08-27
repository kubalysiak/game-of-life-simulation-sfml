#include "TextField.h"

TextField::TextField(float x, float y, float width, float height, const std::string& defaultText, int textSize, const sf::Font& font)
    : _string(defaultText), _isFocused(false)
{
    // Default colors
    _idleBgColor = sf::Color(50, 50, 55);
    _idleOutlineColor = sf::Color(80, 80, 90);
    _focusedBgColor = sf::Color(70, 70, 75);
    _focusedOutlineColor = sf::Color(100, 150, 255);

    _background.setPosition(x, y);
    _background.setSize(sf::Vector2f(width, height));
    _background.setFillColor(_idleBgColor);
    _background.setOutlineThickness(2);
    _background.setOutlineColor(_idleOutlineColor);

    _text.setFont(font);
    _text.setCharacterSize(textSize);
    _text.setFillColor(sf::Color::White); 
    _text.setString(_string);

    _text.setPosition(x + 10, y + (height - textSize) / 2.0f - 2);
    _isVisible = true;
}

void TextField::drawSelf(sf::RenderWindow& window) {
    window.draw(_background);
    window.draw(_text);
}

void TextField::setString(const std::string& str) {
    _string = str;
    _text.setString(_string + (_isFocused ? "_" : ""));
}

std::string TextField::getString() const {
    return _string;
}

void TextField::setFocus(bool focus) {
    _isFocused = focus;

    if (_isFocused) {
        _background.setFillColor(_focusedBgColor);
        _background.setOutlineColor(_focusedOutlineColor);
    }
    else {
        _background.setFillColor(_idleBgColor);
        _background.setOutlineColor(_idleOutlineColor);
    }

    setString(_string);
}

bool TextField::isFocused() const {
    return _isFocused;
}

sf::FloatRect TextField::getGlobalBounds() const {
    return _background.getGlobalBounds();
}

void TextField::setTextColor(sf::Color color) {
    _text.setFillColor(color);
}

void TextField::setColors(sf::Color idleBg, sf::Color idleOutline, sf::Color focusedBg, sf::Color focusedOutline) {
    _idleBgColor = idleBg;
    _idleOutlineColor = idleOutline;
    _focusedBgColor = focusedBg;
    _focusedOutlineColor = focusedOutline;

    setFocus(_isFocused);
}