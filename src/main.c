#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
#include "glad/glad.h"

#include "image.h"

const char* glsl_vertex_source = "";

const char* glsl_fragment_source = "";

void check_shader_compilation_status(GLuint shader)
{
    GLint shader_compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_compiled);
    if (shader_compiled != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(shader, 1024, &log_length, message);
        printf("Error while compiling shader:\n%s\n", message);
    }
    else
        printf("Shader compiled successfully\n");
}

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

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    float vertices[] = {
            -0.5f, 0.5f, 0.0f, 0.0f, // Top left
            0.5f, 0.5f, 1.0f, 0.0f, // Top right
            0.5f, -0.5f, 1.0f, 1.0f, // Bottom right
            -0.5f, -0.5f, 0.0f, 1.0f, // Bottom left
    };

    unsigned int indices[] = {
            0, 1, 2,  // Link points 0, 1 and 2
            2, 3, 0,  // Link points 2, 3 and 0
    };

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    Image* img = load_image_from_file("res/tux.png");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->w, img->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->data);
    free_image(img);

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &glsl_vertex_source, NULL);
    glCompileShader(vertex_shader);
    check_shader_compilation_status(vertex_shader);

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &glsl_fragment_source, NULL);
    glCompileShader(fragment_shader);
    check_shader_compilation_status(fragment_shader);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    glUseProgram(program);

    GLint position_attrib_location = glGetAttribLocation(program, "position");
    glBindFragDataLocation(program, 0, "fragColor");
    glEnableVertexAttribArray(position_attrib_location);
    glVertexAttribPointer(position_attrib_location, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);

    GLint texcoord_attrib_location = glGetAttribLocation(program, "vTexCoords");
    glEnableVertexAttribArray(texcoord_attrib_location);
    glVertexAttribPointer(texcoord_attrib_location, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 2));

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

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        SDL_GL_SwapWindow(window);
    }

    glDeleteProgram(program);
    glDeleteShader(fragment_shader);
    glDeleteShader(vertex_shader);
    glDeleteTextures(1, &tex);
    glDeleteBuffers(1, &ibo);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
