#include "AreaTrigger.h"
#include "Constants.h"

#pragma region Constructor
AreaTrigger::AreaTrigger(int x, int y, ElementType type) : Object(x, y, ELEMENTTYPE_TO_CIRCLE_TEXTURE[type], type)
{

}
#pragma endregion

#pragma region Public Methods
AreaTrigger::~AreaTrigger()
{

}

void AreaTrigger::InjectFrame(unsigned int elapsedGameTime, unsigned int previousFrameTime)
{

}

#pragma endregion