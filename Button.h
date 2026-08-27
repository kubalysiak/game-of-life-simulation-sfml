#pragma once
#include <SFML/Graphics.hpp>
#include "Drawable.h"

class Button : public Drawable {
public:
    Button(float x, float y, float width, float height);

    void drawSelf(sf::RenderWindow& window) override;

    void setBaseColor(sf::Color color);
    void setOutlineColor(sf::Color color);
    sf::FloatRect getGlobalBounds() const;
    void updateHover(sf::Vector2f mousePos);

private:
    sf::RectangleShape _shape;
    sf::Color _baseColor;
};