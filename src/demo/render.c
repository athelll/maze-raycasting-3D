#include "demo.h"

void draw_line(SDL_instance instance)
{
	SDL_SetRenderDrawColor(instance.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderDrawLine(instance.renderer, 10, 10, 100, 100);
}

int init_instance(SDL_instance *instance)
{
	//initialize SDL first
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
		return (1);
	}

	// create a window and derefence our main SDL instance->window to point to the new window
	instance->window = SDL_CreateWindow("SDL2 \\o/", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1260, 720, 0);
	if (instance->window == NULL)
	{
		fprintf(stderr, "Unable to create SDL Window: %s\n", SDL_GetError());
		SDL_Quit();
		return (1);
	}

	// create a new renderer linked to the newly created window
	instance->renderer = SDL_CreateRenderer(instance->window, -1 , SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (instance->renderer == NULL)
	{
		SDL_DestroyWindow(instance->window);
		fprintf(stderr, "Unable to create SDL Renderer: %s\n", SDL_GetError());
		SDL_Quit();
		return (1);
	}
	return (0);
}

int main(void)
{
	SDL_instance instance;

	if (init_instance(&instance) != 0)
		return (1);

	while (1)
  {
    SDL_SetRenderDrawColor(instance.renderer, 0, 0, 0, 0);
    SDL_RenderClear(instance.renderer);
    draw_line(instance);
    SDL_RenderPresent(instance.renderer);
  }
	SDL_DestroyRenderer(instance.renderer);
	SDL_DestroyWindow(instance.window);
	SDL_Quit();
	return (0);
}
