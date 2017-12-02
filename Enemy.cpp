#include "Enemy.h"
#include "Constants.h"

#pragma region Constructor

Enemy::Enemy(int x, int y, int targetX, int targetY, ElementType type) : Object(x, y, ELEMENTTYPE_TO_DOT_TEXTURE[type], type)
{
	this->targetX = targetX;
	this->targetY = targetY;
}

#pragma endregion

#pragma region Public Methods

Enemy::~Enemy()
{

}

void Enemy::InjectFrame()
{
	if (this->x > this->targetX)
	{
		this->x -= ENEMY_VELOCITY;
	}
	else if (this->x < this->targetX)
	{
		this->x += ENEMY_VELOCITY;
	}

	if (this->y > this->targetY)
	{
		this->y -= ENEMY_VELOCITY;
	}
	else if (this->y < this->targetY)
	{
		this->y += ENEMY_VELOCITY;
	}
}

#pragma endregion