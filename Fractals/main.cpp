#include <iostream>
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "CalculationQueue.h"
#include "Tile.h"

const unsigned int IMAGE_DIMENSION = 800;

CalculationQueue calcQueue;
std::vector<std::shared_ptr<Tile>> tiles;

void renderQueue()
{
	tiles.reserve(4);
	tiles.push_back(std::make_shared<Tile>(sf::Rect<double>(-1.5, 1.0, 1.0, 1.0)));
	tiles.push_back(std::make_shared<Tile>(sf::Rect<double>(-0.5, 1.0, 1.0, 1.0)));
	tiles.push_back(std::make_shared<Tile>(sf::Rect<double>(-1.5, 0.0, 1.0, 1.0)));
	tiles.push_back(std::make_shared<Tile>(sf::Rect<double>(-0.5, 0.0, 1.0, 1.0)));

	calcQueue.addTile(tiles[0]);
	calcQueue.addTile(tiles[1]);
	calcQueue.addTile(tiles[2]);
	calcQueue.addTile(tiles[3]);
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(IMAGE_DIMENSION, IMAGE_DIMENSION), "Fractals");
	window.setFramerateLimit(60);

	auto view = window.getDefaultView();
	view.setCenter(-0.5f, 0.0f);
	view.zoom(0.003f);
	window.setView(view);

	renderQueue();

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;

			case sf::Event::MouseWheelMoved:
				/*auto view = window.getDefaultView();
				view.zoom(0.0001f * event.mouseWheel.delta);
				window.setView(view);*/
				break;
			}
		}

		calcQueue.update();

		window.clear();
		for (auto& tile : tiles)
		{
			tile->draw(window);
		}
		window.display();
	}

	return 0;
}