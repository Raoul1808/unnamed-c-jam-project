#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"

int main(void)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Failed to initialize SDL: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_version ver;
	SDL_GetVersion(&ver);
	printf("Using SDL v%d.%d.%d\n", ver.major, ver.minor, ver.patch);

	SDL_Window* window = SDL_CreateWindow("Unnamed C Jam Project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		printf("Failed to create SDL Window: %s\n", SDL_GetError());
		SDL_Quit();
		return EXIT_FAILURE;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)
	{
		printf("Failed to create SDL Renderer: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return EXIT_FAILURE;
	}

	unsigned char running = 1;
	SDL_Event event;
	SDL_Rect rect = {
		.x=100,
		.y=100,
		.w=100,
		.h=100
	};
	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				running = 0;
				break;
			}
		}

		SDL_SetRenderDrawColor(renderer, 100, 149, 237, 255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderFillRect(renderer, &rect);
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	return EXIT_SUCCESS;
}
