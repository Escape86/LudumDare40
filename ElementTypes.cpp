#include "ElementTypes.h"
#include "Constants.h"

std::map<ElementType, const char*> ELEMENTTYPE_TO_CIRCLE_TEXTURE =
{
	{ WATER, BLUE_CIRCLE_TEXTURE_PATH  },
	{ FIRE,  RED_CIRCLE_TEXTURE_PATH   },
	{ PLANT, GREEN_CIRCLE_TEXTURE_PATH }
};

std::map<ElementType, const char*> ELEMENTTYPE_TO_DOT_TEXTURE =
{
	{ NONE,  BLACK_DOT_TEXTURE_PATH },
	{ WATER, BLUE_DOT_TEXTURE_PATH  },
	{ FIRE,  RED_DOT_TEXTURE_PATH   },
	{ PLANT, GREEN_DOT_TEXTURE_PATH }
};

std::map<ElementType, ElementType> ELEMENTTYPE_WEAKNESS =
{
	{ WATER, PLANT },
	{ FIRE,  WATER },
	{ PLANT, FIRE  }
};