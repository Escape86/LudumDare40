#include "Player.h"
#include "Texture.h"
#include "Display.h"

const int PLAYER_VELOCITY = 10;

#pragma region Constructor

Player::Player()
{
	this->x = 0;
	this->y = 0;

	this->horizontalVelocity = 0;
	this->verticalVelocity = 0;

	this->texture = new Texture("resources/dot.bmp");
	this->texture->Load();

	//TODO: pulling this from texture size is probably not ideal
	this->width = this->texture->GetWidth();
	this->height = this->texture->GetHeight();
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
	//update position
	this->x += this->horizontalVelocity;
	this->y += this->verticalVelocity;

	//enforce screen bounds
	if (this->x < 0 || this->x + this->width > Display::GetScreenWidth())
		this->x -= this->horizontalVelocity;

	if (this->y < 0 || this->y + this->height > Display::GetScreenHeight())
		this->y -= this->verticalVelocity;

	//render
	this->Draw();
}

void Player::Draw()
{
	Display::QueueTextureForRendering(this->texture, this->x, this->y);
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

#pragma endregion
