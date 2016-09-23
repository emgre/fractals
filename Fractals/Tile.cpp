#include "Tile.h"
#include <exception>

Tile::Tile(const sf::Rect<double> rect)
:rect(rect)
{
	if (rect.width != rect.height)
	{
		throw std::invalid_argument("Tile must be a square.");
	}
}

Tile::~Tile()
{

}

void Tile::updateData(const std::vector<sf::Uint8>& data)
{
	texture.create(TILE_SIZE, TILE_SIZE);
	texture.update(data.data());
	sprite.setTexture(texture);
	sprite.setPosition(rect.left, -rect.top);
	sprite.setScale(rect.width / (float)TILE_SIZE, rect.height / (float)TILE_SIZE);
}

void Tile::draw(sf::RenderWindow& wnd)
{
	wnd.draw(sprite);
}

const sf::Rect<double> Tile::getRect() const
{
	return rect;
}
