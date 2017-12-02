#include "Object.h"
#include "Texture.h"
#include "Display.h"

#pragma region Constructor

Object::Object(std::string texturePath)
{
	this->x = 0;
	this->y = 0;

	this->texture = new Texture(texturePath);
	this->texture->Load();

	//TODO: pulling this from texture size is probably not ideal
	this->width = this->texture->GetWidth();
	this->height = this->texture->GetHeight();
}

#pragma endregion

#pragma region Public Methods

Object::~Object()
{
	if (this->texture)
	{
		delete this->texture;
	}
}

void Object::Draw()
{
	Display::QueueTextureForRendering(this->texture, this->x, this->y);
}

bool Object::TestCollision(Object* otherObject)
{
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = this->x;
	rightA = this->x + this->width;
	topA = this->y;
	bottomA = this->y + this->height;

	//Calculate the sides of rect B
	leftB = otherObject->x;
	rightB = otherObject->x + otherObject->width;
	topB = otherObject->y;
	bottomB = otherObject->y + otherObject->height;

	//If any of the sides from A are outside of B
	if (bottomA <= topB)
	{
		return false;
	}

	if (topA >= bottomB)
	{
		return false;
	}

	if (rightA <= leftB)
	{
		return false;
	}

	if (leftA >= rightB)
	{
		return false;
	}

	//If none of the sides from A are outside B
	return true;
}

#pragma endregion
