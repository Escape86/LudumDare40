#include "Game.h"
#include "Player.h"

#pragma region Constructor

Game::Game()
{
	this->player = new Player();
}

#pragma endregion

#pragma region Public Methods

Game::~Game()
{
	if (this->player)
	{
		delete this->player;
	}
}

void Game::InjectFrame()
{
	this->player->InjectFrame();
}

void Game::InjectKeyDown(int key)
{
	this->player->OnKeyDown(key);
}

void Game::InjectKeyUp(int key)
{
	this->player->OnKeyUp(key);
}

#pragma endregion
