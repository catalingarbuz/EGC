#include "Projectile.h"

using namespace std;
using namespace m1;

Projectile::Projectile()
{
	this->posX = 0;
	this->posY = 0;
	this->projectileAngle = 0;
	this->speed = 0;
	this->setAngle = 1;
	this->maxDistance = 1;
	this->distance = 0;
	this->destroyed = 0;
}

Projectile::Projectile(float posX, float posY, float projectileAngle, float speed, float maxDistance)
{
	this->posX = posX;
	this->posY = posY;
	this->projectileAngle = projectileAngle;
	this->speed = speed;
	this->setAngle = 1;
	this->maxDistance = maxDistance;
	this->distance = 0;
	this->destroyed = 0;
}

Projectile::~Projectile()
{
}

void Projectile::moveProjectile(float deltaTime)
{
	this->distance++;
	this->posX += deltaTime * this->speed * cos(this->projectileAngle);
	this->posY += deltaTime * this->speed * sin(this->projectileAngle);

}
