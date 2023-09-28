#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"

#include "image.h"
#include "matrix.h"
#include "opengl_context.h"

int main(int argc, char **argv)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Failed to initialize SDL: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_version ver;
    SDL_GetVersion(&ver);
    printf("Using SDL v%d.%d.%d\n", ver.major, ver.minor, ver.patch);

    glc_prepare_context(SDL_GL_CONTEXT_PROFILE_CORE, 3, 3);

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

    OpenGLContext* gl_ctx = glc_create_context();

    GLVertex vertices[] = {
            100.0f, 400.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // Top left
            400.0f, 400.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, // Top right
            400.0f, 100.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, // Bottom right
            100.0f, 100.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom left
    };

    GLIndex indices[] = {
            0, 1, 2,  // Link points 0, 1 and 2
            2, 3, 0,  // Link points 2, 3 and 0
    };

    glc_fill_vertex_buffer(gl_ctx, vertices, 4);
    glc_fill_index_buffer(gl_ctx, indices, 6);

    Image* img = load_image_from_file("res/tux.png");
    glc_create_texture(gl_ctx, img);
    free_image(img);

    glc_create_program(gl_ctx, "res/default.vert", "res/default.frag");

    float view_left = 0.0f;
    float view_right = 1280.0f;
    float view_top = 0.0f;
    float view_bottom = 720.0f;
    float far_clip = 1.0f;
    float near_clip = -1.0f;
    mat4_t cam_mat = {
            2 / (view_right - view_left), 0, 0, -(view_right + view_left) / (view_right - view_left),
            0, 2 / (view_top - view_bottom), 0, -(view_top + view_bottom) / (view_top - view_bottom),
            0, 0, -2 / (far_clip - near_clip), -(far_clip + near_clip) / (far_clip - near_clip),
            0, 0, 0, 1,
    };

    int cam_loc = glc_get_uniform_location(gl_ctx, "projection");

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

        float theta = sin(SDL_GetTicks() / 1000.0f);
        float sin_theta = sin(theta);
        float cos_theta = cos(theta);
        mat4_t rot_mat = {
                cos_theta, -sin_theta, 0, 0,
                sin_theta, cos_theta, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1,
        };

        mat4_t scale_mat = {
                cos_theta, 0, 0, 0,
                0, sin_theta, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1,
        };

        mat4_t proj_mat, proj_mat2;
        mat_mul(cam_mat, rot_mat, &proj_mat);
        mat_mul(proj_mat, scale_mat, &proj_mat2);
        glc_transmit_matrix(gl_ctx, cam_loc, proj_mat2);

        glc_clear_screen(0.1f, 0.4f, 0.9f, 1.0f);
        glc_render_indices(gl_ctx);

        SDL_GL_SwapWindow(window);
    }

    glc_destroy_context(gl_ctx);
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
