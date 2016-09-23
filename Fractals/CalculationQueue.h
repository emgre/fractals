#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <memory>
#include <mutex>
#include <atomic>
#include <SFML/Graphics.hpp>

class Tile;

constexpr unsigned int MIN_NUM_THREADS = 2;
constexpr unsigned int MAX_ITERATION = 255;

class ThreadsFinishedException : std::exception
{
	virtual const char* what() const { return "Worker thread should be killed."; }
};

class CalculationQueue
{
public:
	CalculationQueue();
	virtual ~CalculationQueue();

	void addTile(std::weak_ptr<Tile> tile);
	std::weak_ptr<Tile> getTile();
	bool isWorkerThreadsEnabled() const;

	void addTileReady(std::pair<std::weak_ptr<Tile>, std::vector<sf::Uint8>> tile);
	void update();

	static void threadRoutine(CalculationQueue* queue);
	static unsigned int calculateMandelbrot(const Tile& tile, unsigned long i, unsigned long j, unsigned int maxIteration);
	static double mapCoordinate(unsigned long x, unsigned long width, double min, double max);
	static void setPixel(std::vector<sf::Uint8>* pixels, unsigned long x, unsigned long y, unsigned long value);

private:
	std::vector<std::thread> threads;
	std::queue<std::weak_ptr<Tile>> tiles;
	std::queue<std::pair<std::weak_ptr<Tile>, std::vector<sf::Uint8>>> tilesReady;

	std::mutex tileMutex;
	std::condition_variable tileAvailable;
	std::atomic<bool> workerThreadEnabled;
};