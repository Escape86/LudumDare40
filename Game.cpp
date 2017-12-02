#include "Game.h"
#include "Player.h"
#include "Enemy.h"

#pragma region Constructor

Game::Game()
{
	this->player = new Player();

	this->enemies.push_back(new Enemy(10, 25));
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
	for (Enemy* const enemy : this->enemies)
	{
		enemy->InjectFrame();
	}

	this->player->InjectFrame();

	//now that movements are updated check for collisions


	//draw enemies
	for (Enemy* const enemy : this->enemies)
	{
		enemy->Draw();
	}

	//draw player
	this->player->Draw();
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
