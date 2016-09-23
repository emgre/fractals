#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

constexpr unsigned int TILE_SIZE = 512;

class Tile
{
public:
	Tile(const sf::Rect<double> rect);
	virtual ~Tile();

	void updateData(const std::vector<sf::Uint8>& data);
	void draw(sf::RenderWindow& wnd);

	const sf::Rect<double> getRect() const;

private:
	sf::Rect<double> rect;
	sf::Sprite sprite;
	sf::Texture texture;
};