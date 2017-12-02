#pragma once

#include <vector>
#include <functional>
#include "SDL_events.h"

#pragma region Forward Declarations
struct SDL_Window;
struct SDL_Renderer;
class Texture;
#pragma endregion

class Display
{
public:
	Display() = delete;

	static bool Initialize();
	static bool ShutDown();
	static void InjectFrame();
	static void SetEventCallback(std::function<void(SDL_Event e)> eventCallback);

	static SDL_Renderer* const GetRenderer();
	static void QueueTextureForRendering(Texture* const texture, int x, int y);

	static int GetScreenWidth();
	static int GetScreenHeight();

private:
	static SDL_Window* window;
	static SDL_Renderer* renderer;
	static std::function<void(SDL_Event e)> eventCallback;

	struct QueuedTexture 
	{
		Texture* texture;
		int x;
		int y;
	};
	static std::vector<QueuedTexture> textureQueue;
};