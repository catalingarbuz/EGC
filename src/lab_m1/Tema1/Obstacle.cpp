#include "Obstacle.h"

using namespace std;
using namespace m1;

m1::Obstacle::Obstacle()
{
	this->posX = this->posY = this->height = this->width = 0;
}

m1::Obstacle::Obstacle(float posX, float posY, float width, float height)
{
	this->posX = posX;
	this->posY = posY;
	this->width = width;
	this->height = height;
}

m1::Obstacle::~Obstacle()
{
}
