#pragma once

#include "Object.h"

class AreaTrigger : public Object
{
public:
	AreaTrigger(int x, int y, ElementType type);
	~AreaTrigger();

	void InjectFrame() override;
};