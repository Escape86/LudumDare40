#include "Enemy.h"

#pragma region Constructor

Enemy::Enemy(int x, int y) : Object("resources/enemy.bmp")
{
	this->x = x;
	this->y = y;
}

#pragma endregion

#pragma region Public Methods

Enemy::~Enemy()
{

}

void Enemy::InjectFrame()
{

}

#pragma endregion