#include "Enemy.h"
#include "Constants.h"

#pragma region Constructor

Enemy::Enemy(int x, int y) : Object(x, y, ENEMY_TEXTURE_PATH)
{

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