#pragma once

#include <vector>
#include <functional>
#include "SDL_events.h"
#include "SDL_ttf.h"

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

	static TTF_Font* const GetFont();

	struct QueuedText
	{
		int x;
		int y;
		std::string text;
		SDL_Color textColor;
		bool isVisible;
		int id;
	};

	static int CreateText(std::string text, int x, int y, SDL_Color textColor = { 0, 0, 0 });
	static bool UpdateText(int id, std::string text);
	static bool MoveText(int id, int x, int y);
	static bool SetTextIsVisible(int id, bool isVisible);
	static bool RemoveText(int id);

private:
	static SDL_Window* window;
	static SDL_Renderer* renderer;
	static TTF_Font* font;
	static std::function<void(SDL_Event e)> eventCallback;
	static int textControlIdCounter;

	struct QueuedTexture 
	{
		Texture* texture;
		int x;
		int y;
	};
	static std::vector<QueuedTexture> textureQueue;

	static std::vector<QueuedText> textQueue;
};