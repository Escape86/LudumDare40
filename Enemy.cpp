#include "Enemy.h"
#include "Constants.h"

#pragma region Constructor

Enemy::Enemy(int x, int y) : Object(ENEMY_TEXTURE_PATH)
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