#include <sstream>
#include <SDL.h>
#pragma comment(lib, "SDL2.lib")
#undef main

#define INPUT_PATH "..\\resources\\spaceship.bmp"
#define WINDOW_NAME "SDL2 Keyboard/Mouse events"

int main(int argc, char ** argv)
{
	// variables
	bool quit = false;
	int x = 288;
	int y = 208;
	char windowName[1024];
	strcpy(windowName, WINDOW_NAME);

	SDL_Event event;
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Surface *image;
	SDL_Texture *texture;

	// init SDL
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow(windowName,SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
	renderer = SDL_CreateRenderer(window, -1, 0);

	image = SDL_LoadBMP(INPUT_PATH);
	texture = SDL_CreateTextureFromSurface(renderer, image);
	SDL_FreeSurface(image);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	// handle events
	while (!quit)
	{
		SDL_Delay(20);
		SDL_PollEvent(&event);

		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_LEFT:  x--; break;
			case SDLK_RIGHT: x++; break;
			case SDLK_UP:    y--; break;
			case SDLK_DOWN:  y++; break;
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
				SDL_ShowSimpleMessageBox(0, "Mouse", "Left button was pressed!", window);
				break;
			case SDL_BUTTON_RIGHT:
				SDL_ShowSimpleMessageBox(0, "Mouse", "Right button was pressed!", window);
				break;
			default:
				SDL_ShowSimpleMessageBox(0, "Mouse", "Some other button was pressed!", window);
				break;
			}
			break;
		case SDL_MOUSEMOTION:
			int mouseX = event.motion.x;
			int mouseY = event.motion.y;

			std::stringstream ss;
			ss << "X: " << mouseX << " Y: " << mouseY;
			SDL_SetWindowTitle(window, ss.str().c_str());
			break;
		}

		SDL_Rect dstrect = { x, y, 64, 64 };

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, &dstrect);
		SDL_RenderPresent(renderer);
	}

	// cleanup SDL
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}