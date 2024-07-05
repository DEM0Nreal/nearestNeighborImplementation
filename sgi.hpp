class sgi {
private:
	size_t cellSize;
	size_t elementCount;
	std::vector<std::vector<std::vector<Positioned*>>> grid;

	Positioned* closestInCell(float x, float y, int gx, int gy,
			                   double *dist);

public:
	sgi(size_t, sf::Vector2i);
	void addElement(Positioned *element);
	void removeElement(Positioned *element);
	void updateElement(Positioned *element);
	std::vector<Positioned*> listAll();
	std::vector<Positioned*> listFromCell(int x, int y);
	Positioned* nearestNeighbor(float, float);
	size_t getCellSize();
};

#include "sgi.cpp"
