#pragma once

#include <string>

#pragma region Forward Declarations
class Texture;
#pragma endregion

class Object
{
public:
	Object(std::string texturePath);
	virtual ~Object();

	virtual void InjectFrame() = 0;

	void Draw();

	bool TestCollision(Object* otherObject);

protected:
	int x;
	int y;
	int width;
	int height;

private:
	Texture* texture;
};