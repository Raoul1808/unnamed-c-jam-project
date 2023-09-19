#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
#include "glad/glad.h"

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

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	SDL_Window* window = SDL_CreateWindow("Unnamed C Jam Project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if (window == NULL)
	{
		printf("Failed to create SDL Window: %s\n", SDL_GetError());
		SDL_Quit();
		return EXIT_FAILURE;
	}

	SDL_GLContext gl_context = SDL_GL_CreateContext(window);
	if (gl_context == NULL)
	{
		printf("Failed to create SDL GL Context: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return EXIT_FAILURE;
	}

	if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
	{
		printf("Failed to load OpenGL 3.3 Core\n");
		SDL_GL_DeleteContext(gl_context);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return EXIT_FAILURE;
	}

	GLuint vbo;
	glGenBuffers(1, &vbo);
	printf("%d\n", vbo);
	glDeleteBuffers(1, &vbo);

	unsigned char running = 1;
	SDL_Event event;
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

		glClearColor(0.1f, 0.5f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	return EXIT_SUCCESS;
}
