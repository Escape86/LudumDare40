#include "Game.h"
#include "Player.h"
#include "Enemy.h"
#include "Display.h"

int debug_collision_text_id = -1;

#pragma region Constructor

Game::Game()
{
	this->player = new Player();

	this->enemies.push_back(new Enemy(10, 25));

	debug_collision_text_id = Display::CreateText("Player Collision", 0, 0);
	Display::SetTextIsVisible(debug_collision_text_id, false);
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
	bool collision = false;
	for (Enemy* const enemy : this->enemies)
	{
		if (this->player->TestCollision(enemy))
		{
			collision = true;
			break;
		}
	}
	Display::SetTextIsVisible(debug_collision_text_id, collision);

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
