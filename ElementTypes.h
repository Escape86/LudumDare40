#pragma once

#include <map>

enum ElementType
{
	NONE,
	WATER,
	FIRE,
	PLANT
};

extern std::map<ElementType, const char*> ELEMENTTYPE_TO_CIRCLE_TEXTURE;

extern std::map<ElementType, const char*> ELEMENTTYPE_TO_DOT_TEXTURE;

extern std::map<ElementType, ElementType> ELEMENTTYPE_WEAKNESS;