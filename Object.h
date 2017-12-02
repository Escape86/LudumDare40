#pragma once

#include <string>
#include "ElementTypes.h"

#pragma region Forward Declarations
class Texture;
#pragma endregion

class Object
{
public:
	Object(int x, int y, std::string texturePath, ElementType type);
	virtual ~Object();

	virtual void InjectFrame() = 0;

	void Draw();

	bool TestCollision(Object* otherObject);
	void SetTexture(std::string texturePath);
	ElementType GetElementType();

protected:
	int x;
	int y;
	int width;
	int height;

	ElementType type;

private:
	Texture* texture;
};