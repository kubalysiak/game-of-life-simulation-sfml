#pragma once

#include "SFML/Graphics.hpp"
#include <vector>
#include <memory>

class Drawable {
public:
	virtual ~Drawable() = default;

	void draw(sf::RenderWindow& window);
	void addElement(std::unique_ptr<Drawable> el);

	void visibleOn() { _isVisible = true; }
	void visibleOff() { _isVisible = false; }
	void visibleToggle() { _isVisible = !_isVisible; }

	virtual void drawSelf(sf::RenderWindow& window) = 0;

protected:
	bool _isVisible = true;
	std::vector<std::unique_ptr< Drawable >> _elements;
};
