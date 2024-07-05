sgi::sgi(size_t cellSize, sf::Vector2i worldSize){
	this->cellSize = cellSize;

	int width = worldSize.x/this->cellSize;
	int height = worldSize.y/this->cellSize;
	
	this->grid.resize(width);

	for (int i = 0; i < width; ++i){
		this->grid[i].resize(height);
	}

	this->elementCount = 0;
	
}

void sgi::addElement(Positioned *element){
	float x = element->posX;
	float y = element->posY;

	element->prevPosX = x;
	element->prevPosY = y;

	int gridX = x/this->cellSize;
	int gridY = y/this->cellSize;

	// bounds checking
	if (gridX >= this->grid.size()){
		gridX = this->grid.size()-1;
	}
	if (gridX < 0) gridX = 0;

	if (gridY >= this->grid[0].size()){
		gridY = this->grid[0].size()-1;
	}
	if (gridY < 0) gridY = 0;

	//printf("actual: %6.2f : %6.2f\tgrid: %d : %d\n", x, y, gridX, gridY);

	this->grid[gridX][gridY].push_back(element);

	++this->elementCount;
}

void sgi::removeElement(Positioned *element){
	float x = element->prevPosX;
	float y = element->prevPosY;
	if (x == -1 || y == -1){
		return;
	}

	int gridX = x/this->cellSize;
	int gridY = y/this->cellSize;

	// if somehow it's outside grid
	if (gridX < 0 || gridX >= this->grid.size()
			|| gridY < 0 || gridY >= this->grid[0].size()){
		return;
	}

	for (int i = 0; i < this->grid[gridX][gridY].size(); ++i){
		if (this->grid[gridX][gridY][i] == element){
			this->grid[gridX][gridY].erase(this->grid[gridX][gridY].begin() + i);
			break;
		}
	}

	--this->elementCount;
}

void sgi::updateElement(Positioned *element){
	this->removeElement(element);
	this->addElement(element);
}

std::vector<Positioned*> sgi::listAll(){
	std::vector<Positioned*> allElements;

	for (int i = 0; i < grid.size(); ++i){
		for (int j = 0; j < grid[i].size(); ++j){
			// add elements of grid cell to allElements
			allElements.insert(allElements.end(), 
					grid[i][j].begin(), grid[i][j].end());
		}
	}

	return allElements;
}

std::vector<Positioned*> sgi::listFromCell(int x, int y){
	return this->grid[x][y];
}

// private function closestInCell
Positioned* sgi::closestInCell(float x, float y, int gx, int gy, 
		                                  double *dist = nullptr){
	if (gx < 0 || gx >= this->grid.size()
			|| gy < 0 || gy >= this->grid[0].size()){
		return nullptr;
	}
	double minDist = distance(-1, -1, _window_width, _window_height);
	Positioned *closest = nullptr;
	for (auto element : listFromCell(gx, gy)){
		double curDist = distance(x, y, element->posX, element->posY);
		if (minDist > curDist){
			minDist = curDist;
			closest = element;
		}
	}
	if (dist) *dist = minDist;
	return closest;
}

Positioned* sgi::nearestNeighbor(float x, float y){
	if (elementCount == 0) return nullptr;

	int gridX = x/this->cellSize;
	int gridY = y/this->cellSize;

	int dist = 1;

	double closestDist;

	Positioned *closest = closestInCell(x, y, gridX, gridY, &closestDist);

	while (!(dist == 2 && closest) && dist < std::max(grid.size(), grid[0].size())){
		//printf("dist: %d\teleCount: %d\tmax: %d\n", dist, elementCount,
		//		std::max(grid.size(), grid[0].size()));
		// top down lines of "the box"
		for (int i = -dist; i <= dist; ++ i){
			Positioned *temp1 = 0, *temp2 = 0;
			double tempDist;
			temp1 = closestInCell(x, y, gridX+i, gridY+dist, &tempDist);
			if (temp1 && tempDist < closestDist){
				closestDist = tempDist;
				closest = temp1;
					
			}
			temp2 = closestInCell(x, y, gridX+i, gridY-dist, &tempDist);
			if (temp2 && tempDist < closestDist){
				closestDist = tempDist;
				closest = temp2;
			}
		}

		// left right lines of "the box" excluding corners
		for (int i = -dist+1; i < dist; ++i){
			Positioned *temp1, *temp2;
			double tempDist;
			temp1 = closestInCell(x, y, gridX+dist, gridY+i, &tempDist);
			if (temp1 && tempDist < closestDist){
				closestDist = tempDist;
				closest = temp1;
					
			}
			temp2 = closestInCell(x, y, gridX-dist, gridY+i, &tempDist);
			if (temp2 && tempDist < closestDist){
				closestDist = tempDist;
				closest = temp2;
			}
		}

		++dist;
	}


	return closest;
}

size_t sgi::getCellSize(){
	return this->cellSize;
}
