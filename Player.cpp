#include "Player.h"
#include "Texture.h"
#include "Display.h"
#include "Constants.h"

#pragma region Constructor

Player::Player() : Object(0, 0, BLACK_DOT_TEXTURE_PATH, ElementType::NONE)
{
	this->horizontalVelocity = 0;
	this->verticalVelocity = 0;
}

#pragma endregion

#pragma region Public Methods

Player::~Player()
{

}

void Player::InjectFrame()
{
	//update position
	this->x += this->horizontalVelocity;
	this->y += this->verticalVelocity;

	//enforce screen bounds
	if (this->x < 0 || this->x + this->width > Display::GetScreenWidth())
		this->x -= this->horizontalVelocity;

	if (this->y < 0 || this->y + this->height > Display::GetScreenHeight())
		this->y -= this->verticalVelocity;
}

void Player::OnKeyDown(int key)
{
	switch (key)
	{
		case SDLK_w:
		case SDLK_UP:
			this->verticalVelocity -= PLAYER_VELOCITY;
			break;
		case SDLK_s:
		case SDLK_DOWN:
			this->verticalVelocity += PLAYER_VELOCITY;
			break;
		case SDLK_a:
		case SDLK_LEFT:
			this->horizontalVelocity -= PLAYER_VELOCITY;
			break;
		case SDLK_d:
		case SDLK_RIGHT:
			this->horizontalVelocity += PLAYER_VELOCITY;
			break;
	}

	//enforce velocity min/max values
	if (this->verticalVelocity > PLAYER_VELOCITY)
		this->verticalVelocity = PLAYER_VELOCITY;
	else if (this->verticalVelocity < -PLAYER_VELOCITY)
		this->verticalVelocity = -PLAYER_VELOCITY;

	if (this->horizontalVelocity > PLAYER_VELOCITY)
		this->horizontalVelocity = PLAYER_VELOCITY;
	else if (this->horizontalVelocity < -PLAYER_VELOCITY)
		this->horizontalVelocity = -PLAYER_VELOCITY;
}

void Player::OnKeyUp(int key)
{
	switch (key)
	{
		case SDLK_w:
		case SDLK_UP:
			this->verticalVelocity += PLAYER_VELOCITY;
			break;
		case SDLK_s:
		case SDLK_DOWN:
			this->verticalVelocity -= PLAYER_VELOCITY;
			break;
		case SDLK_a:
		case SDLK_LEFT:
			this->horizontalVelocity += PLAYER_VELOCITY;
			break;
		case SDLK_d:
		case SDLK_RIGHT:
			this->horizontalVelocity -= PLAYER_VELOCITY;
			break;
	}

	//enforce velocity min/max values
	if (this->verticalVelocity > PLAYER_VELOCITY)
		this->verticalVelocity = PLAYER_VELOCITY;
	else if (this->verticalVelocity < -PLAYER_VELOCITY)
		this->verticalVelocity = -PLAYER_VELOCITY;

	if (this->horizontalVelocity > PLAYER_VELOCITY)
		this->horizontalVelocity = PLAYER_VELOCITY;
	else if (this->horizontalVelocity < -PLAYER_VELOCITY)
		this->horizontalVelocity = -PLAYER_VELOCITY;
}

void Player::ResetHorizontalVelocity()
{
	this->horizontalVelocity = 0;
}

void Player::ResetVerticalVelocity()
{
	this->verticalVelocity = 0;
}

void Player::SetElementType(ElementType type)
{
	this->type = type;

	//destroy old texture and replace with new one that matches our new element type
	this->SetTexture(ELEMENTTYPE_TO_DOT_TEXTURE[type]);
}

#pragma endregion
