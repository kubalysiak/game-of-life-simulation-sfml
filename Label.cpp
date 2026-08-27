#include "Label.h"

Label::Label(float x, float y, const std::string& textStr, unsigned int size, const sf::Font& font, sf::Color color)
    : _isCentered(false)
{
    _text.setFont(font);
    _text.setString(textStr);
    _text.setCharacterSize(size);
    _text.setFillColor(color);
    _text.setPosition(x, y);
    _isVisible = true;
}

void Label::drawSelf(sf::RenderWindow& window) {
    window.draw(_text);
}

void Label::setString(const std::string& str) {
    _text.setString(str);

    if (_isCentered) {
        centerOrigin();
    }
}

void Label::setColor(sf::Color color) {
    _text.setFillColor(color);
}

void Label::setPosition(float x, float y) {
    _text.setPosition(x, y);
}


void Label::centerOrigin() {
    _isCentered = true;
    sf::FloatRect bounds = _text.getLocalBounds();
  
    _text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
}