#include "CalculationQueue.h"
#include "Tile.h"
#include <iostream>

CalculationQueue::CalculationQueue()
{
	unsigned int numThreads = std::max(MIN_NUM_THREADS, std::thread::hardware_concurrency());
	threads.reserve(numThreads);

	workerThreadEnabled = true;

	for (unsigned int i = 0; i < numThreads; i++)
	{
		threads.push_back(std::thread(CalculationQueue::threadRoutine, this));
	}
}

CalculationQueue::~CalculationQueue()
{
	workerThreadEnabled = false;
	tileAvailable.notify_all();

	for (auto& thread : threads)
	{
		thread.join();
	}
}

void CalculationQueue::addTile(std::weak_ptr<Tile> tile)
{
	std::unique_lock<std::mutex> guard(tileMutex);
	tiles.push(tile);

	tileAvailable.notify_one();
}

std::weak_ptr<Tile> CalculationQueue::getTile()
{
	std::unique_lock<std::mutex> guard(tileMutex);

	tileAvailable.wait(guard, [this]() {return !isWorkerThreadsEnabled() || !tiles.empty(); });

	if (tiles.empty())
	{
		throw ThreadsFinishedException();
	}

	auto tile = tiles.front();
	tiles.pop();

	return tile;
}

bool CalculationQueue::isWorkerThreadsEnabled() const
{
	return workerThreadEnabled;
}

void CalculationQueue::addTileReady(std::pair<std::weak_ptr<Tile>, std::vector<sf::Uint8>> tile)
{
	tilesReady.push(tile);
}

void CalculationQueue::update()
{
	while (!tilesReady.empty())
	{
		auto t = tilesReady.front();
		if (!t.first.expired())
		{
			auto tile = t.first.lock();
			tile->updateData(t.second);
		}

		tilesReady.pop();
	}
}

void CalculationQueue::threadRoutine(CalculationQueue* queue)
{
	while (true)
	{
		try
		{
			auto tileWeakPtr = queue->getTile();

			if (tileWeakPtr.expired()) { continue; }

			auto tile = *tileWeakPtr.lock();

			std::vector<sf::Uint8> pixels;
			pixels.resize(TILE_SIZE * TILE_SIZE * 4);

			for (unsigned long i = 0; i < TILE_SIZE; i++)
			{
				for (unsigned long j = 0; j < TILE_SIZE; j++)
				{
					unsigned long n = CalculationQueue::calculateMandelbrot(tile, i, j, MAX_ITERATION);
					CalculationQueue::setPixel(&pixels, j, i, n);
				}
			}

			queue->addTileReady(std::make_pair(tileWeakPtr, pixels));
		}
		catch (ThreadsFinishedException e)
		{
			break;
		}
	}
}

inline unsigned int CalculationQueue::calculateMandelbrot(const Tile& tile, unsigned long i, unsigned long j, unsigned int maxIteration)
{
	auto rect = tile.getRect();
	double x0 = mapCoordinate(i, TILE_SIZE, rect.left, rect.left + rect.width);
	double y0 = mapCoordinate(j, TILE_SIZE, rect.top, rect.top - rect.height);
	double x = 0;
	double y = 0;
	double xSqr = 0;
	double ySqr = 0;
	unsigned long iteration = 0;
	while (xSqr + ySqr < 4 && iteration < maxIteration)
	{
		y = x * y;
		y += y;
		y += y0;
		x = xSqr - ySqr + x0;
		xSqr = x * x;
		ySqr = y * y;

		iteration++;
	}

	return iteration;
}

inline double CalculationQueue::mapCoordinate(unsigned long x, unsigned long width, double min, double max)
{
	double range = max - min;
	return x * (range / width) + min;
}

inline void CalculationQueue::setPixel(std::vector<sf::Uint8>* pixels, unsigned long x, unsigned long y, unsigned long value)
{
	unsigned long base = x * TILE_SIZE * 4 + y * 4;

	(*pixels)[base] = (unsigned int)(value);
	(*pixels)[base + 1] = (unsigned int)(value);
	(*pixels)[base + 2] = (unsigned int)(value);
	(*pixels)[base + 3] = 255;
}