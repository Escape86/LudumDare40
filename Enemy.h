#pragma once

#include "Object.h"

class Enemy : public Object
{
public:
	Enemy(int x, int y);
	~Enemy();

	void InjectFrame() override;
};