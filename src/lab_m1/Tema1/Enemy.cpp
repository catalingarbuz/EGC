#include "Enemy.h"
#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2D.h"
#include <cmath>


using namespace std;
using namespace m1;

Enemy::Enemy()
{
	this->posX = 0;
	this->posY = 0;
	this->enemyAngle = 0;
	this->speed = 0;
	this->sizeX = 0;
	this->sizeY = 0;
	this->dead = false;
}

m1::Enemy::Enemy(float posX, float posY, float enemyAngle, float speed, float sizeX, float sizeY)
{
	this->posX = posX;
	this->posY = posY;
	this->enemyAngle = enemyAngle;
	this->speed = speed;
	this->sizeX = sizeX;
	this->sizeY = sizeY;
	this->dead = false;
}

Enemy::~Enemy()
{
}

void Enemy::moveEnemy(float playerX, float playerY, float deltaTime) {
	float dirX, dirY, h;

	dirX = playerX - this->posX;
	dirY = playerY - this->posY;

	h = sqrt(dirX * dirX + dirY * dirY);

	dirX /= h;
	dirY /= h;

    this->posX += deltaTime * this->speed * dirX;
	this->posY += deltaTime * this->speed * dirY;
	this->enemyAngle = atan2(playerY - this->posY, playerX - this->posX) - M_PI_2;
}
