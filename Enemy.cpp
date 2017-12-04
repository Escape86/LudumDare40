#include "Enemy.h"
#include "Constants.h"

#pragma region Constructor

Enemy::Enemy(double x, double y, int targetX, int targetY, ElementType type) : Object(x, y, ELEMENTTYPE_TO_DOT_TEXTURE[type], type)
{
	this->targetX = targetX;
	this->targetY = targetY;
}

#pragma endregion

#pragma region Public Methods

Enemy::~Enemy()
{

}

void Enemy::InjectFrame(unsigned int elapsedGameTime, unsigned int previousFrameTime)
{
	double x1 = this->x;
	double x2 = this->targetX;
	double y1 = this->y;
	double y2 = this->targetY;

	double xDistance = x2 - x1;
	double yDistance = y2 - y1;

	if (xDistance == 0 && yDistance == 0)
		return;

	double xRatio;
	double yRatio;

	//which direction do we need to move the farthest in?
	if (abs(xDistance) > abs(yDistance))
	{
		xRatio = this->x > targetX ? -1.0 : 1.0;
		yRatio = yDistance / abs(xDistance);
	}
	else
	{
		xRatio = xDistance / yDistance;
		yRatio = this->y > targetY ? -1.0 : 1.0;
	}

	this->x += (xRatio * ENEMY_VELOCITY);
	this->y += (yRatio * ENEMY_VELOCITY);
}

#pragma endregion