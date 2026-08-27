#include "Drawable.h"

void Drawable::draw(sf::RenderWindow& window) {
	if (!_isVisible) {
		return;
	}
	drawSelf(window);
	for (auto const &el : _elements) {
		el->draw(window);
	}
}
void Drawable::addElement(std::unique_ptr<Drawable> el) {
	_elements.push_back(std::move(el));
}