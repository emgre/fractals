#pragma once

#include <memory>
#include <vector>
#include "Point.h"

using namespace std;

struct AABB
{
	Point centre;
	Point halfSize;

	AABB(Point centre = {}, Point halfSize = {}) : centre(centre), halfSize(halfSize){};

	bool intersects(AABB other)
	{
		if (centre.x + halfSize.x > other.centre.x - other.halfSize.x || centre.x - halfSize.x < other.centre.x + other.halfSize.x)
		{
			if (centre.y + halfSize.y > other.centre.y - other.halfSize.y || centre.y - halfSize.y < other.centre.y + other.halfSize.y)
			{
				return true;
			}
		}
		return false;
	}
};

template <class T>
class Quadtree
{
public:
	Quadtree<T>(AABB boundary = {});
	virtual ~Quadtree() {};

	void setData(const T* data);
	T* getData() const;

	void subdivide();
	std::vector<T*> queryRange(const AABB range) const;

private:
	T* data;
	unique_ptr<Quadtree> nw, ne, sw, se;
	AABB boundary;
};

template <class T>
Quadtree<T>::Quadtree(AABB boundary)
{
	this->boundary = boundary;
}

template <class T>
void Quadtree<T>::setData(const T* data)
{
	this->data = data;
}

template <class T>
T* Quadtree<T>::getData() const
{
	return data;
}

template <class T>
void Quadtree<T>::subdivide()
{
	nw = make_unique<Quadtree>(AABB(boundary.center - boundary.halfSize, boundary.center - boundary.halfSize));
	ne = make_unique<Quadtree>(AABB(boundary.center - boundary.halfSize, boundary.center + boundary.halfSize));
	sw = make_unique<Quadtree>(AABB(boundary.center + boundary.halfSize, boundary.center - boundary.halfSize));
	se = make_unique<Quadtree>(AABB(boundary.center + boundary.halfSize, boundary.center + boundary.halfSize));
}

template <class T>
std::vector<T*> Quadtree<T>::queryRange(const AABB range) const
{
	std::vector<T*> result;

	if (!boundary.intersects(range))
	{
		return result;
	}

	if (data)
	{
		result.push_back(data);
	}

	if (!nw)
	{
		return result;
	}

	result.push_back(nw.queryRange(range));
	result.push_back(ne.queryRange(range));
	result.push_back(sw.queryRange(range));
	result.push_back(se.queryRange(range));

	return result;
}