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

#pragma endregion
