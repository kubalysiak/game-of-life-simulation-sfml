#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Drawable.h"

class TextField : public Drawable {
public:
    TextField(float x, float y, float width, float height, const std::string& defaultText, int textSize, const sf::Font& font);

    void drawSelf(sf::RenderWindow& window) override;
    void setString(const std::string& str);
    std::string getString() const;

    void setFocus(bool focus);
    bool isFocused() const;
    sf::FloatRect getGlobalBounds() const;

    void setTextColor(sf::Color color);
    void setColors(sf::Color idleBg, sf::Color idleOutline, sf::Color focusedBg, sf::Color focusedOutline);

private:
    sf::RectangleShape _background;
    sf::Text _text;
    std::string _string;
    bool _isFocused;

    sf::Color _idleBgColor;
    sf::Color _idleOutlineColor;
    sf::Color _focusedBgColor;
    sf::Color _focusedOutlineColor;
};