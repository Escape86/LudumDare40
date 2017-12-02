#include "Display.h"
#include "Game.h"

int main(int argc, char* args[])
{
	if (!Display::Initialize())
	{
		return -1;
	}

	Game* game = new Game();

	//game loop
	while (true)
	{
		//update game objects
		game->InjectFrame();

		//draw the frame
		Display::InjectFrame();
	}

	delete game;

	if (!Display::ShutDown())
	{
		return -1;
	}

	return 0;
}