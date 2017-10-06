#include <sstream>
#include <thread>

#include <SDL.h>
#pragma comment(lib, "SDL2.lib")
#undef main

#define INPUT_PATH  "..\\resources\\spaceship.bmp"
#define WINDOW_NAME "SDL2 Keyboard/Mouse events"

#define FF_REFRESH_EVENT	(SDL_USEREVENT)
#define FF_QUIT_EVENT		(SDL_USEREVENT + 1)

// variables
bool quit;
int x;
int y;
int mouseX;
int mouseY;
int cnt;
char windowName[1024];
std::thread *refreshThread;
bool isRefresh;

SDL_Event event;
SDL_Window *window;
SDL_Renderer *renderer;
SDL_Surface *image;
SDL_Texture *texture;

/* schedule a video refresh in 'delay' ms */
Uint32 scheduleRefresh(int delay_ms, void *data) {
	SDL_Event event;
	event.type = FF_REFRESH_EVENT;
	event.user.data1 = data;
	SDL_PushEvent(&event);
	SDL_Delay(delay_ms);
	return 0; /* 0 means stop timer */
}

/* schedule a quit event */
Uint32 scheduleQuit(int delay_ms, void *data) {
	SDL_Event event;
	event.type = FF_QUIT_EVENT;
	event.user.data1 = data;
	SDL_PushEvent(&event);
	SDL_Delay(delay_ms);
	return 0; /* 0 means stop timer */
}

void refreshThreadFunc(void *data) {
	int *p = (int *)data;
	while (isRefresh && *p < 10) {
		scheduleRefresh(1000, (void *)p);
		*p += 1;
	}
	scheduleQuit(1000, (void *)p);
}

int main(int argc, char ** argv)
{
	int *p;
	quit = false;
	x = 288;
	y = 208;
	cnt = 0;
	isRefresh = true;
	strcpy(windowName, WINDOW_NAME);

	// init SDL
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
	window = SDL_CreateWindow(windowName,SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
	renderer = SDL_CreateRenderer(window, -1, 0);

	image = SDL_LoadBMP(INPUT_PATH);
	texture = SDL_CreateTextureFromSurface(renderer, image);
	SDL_FreeSurface(image);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	refreshThread = new std::thread(refreshThreadFunc, &cnt);
	refreshThread->detach();

	// handle events
	while (!quit)
	{
		SDL_WaitEvent(&event);

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
			mouseX = event.motion.x;
			mouseY = event.motion.y;

			sprintf(windowName, "X : %d - Y : %d", mouseX, mouseY);
			SDL_SetWindowTitle(window, windowName);
			break;
		case FF_REFRESH_EVENT:
			p = (int*)event.user.data1;
			printf("Refresh: %d\n", *p);
			break;
		case FF_QUIT_EVENT:
			p = (int*)event.user.data1;
			printf("Quit: %d\n", *p);
			quit = true;
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