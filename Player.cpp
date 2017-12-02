#include "Player.h"
#include "Texture.h"
#include "Display.h"

#pragma region Constructor

Player::Player()
{
	this->x = 0;
	this->y = 0;

	this->texture = new Texture("resources/dot.bmp");
	this->texture->Load();
}

#pragma endregion

#pragma region Public Methods

Player::~Player()
{
	if (this->texture)
	{
		delete this->texture;
	}
}

void Player::InjectFrame()
{
	this->Draw();
}

void Player::Draw()
{
	Display::QueueTextureForRendering(this->texture, this->x, this->y);
}

#pragma endregion
