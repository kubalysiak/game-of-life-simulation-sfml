#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Drawable.h"

class Label : public Drawable {
public:
    Label(float x, float y, const std::string& textStr, unsigned int size, const sf::Font& font, sf::Color color = sf::Color::White);

    void drawSelf(sf::RenderWindow& window) override;
    void setString(const std::string& str);
    void setColor(sf::Color color);
    void setPosition(float x, float y);
    void centerOrigin();

private:
    sf::Text _text;
    bool _isCentered;
};