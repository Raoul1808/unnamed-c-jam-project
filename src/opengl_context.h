#ifndef OPENGL_CONTEXT_H
#define OPENGL_CONTEXT_H

#include "image.h"
#include "matrix.h"

typedef struct
{
    unsigned int vao;
    unsigned int vbo;
    unsigned int ibo;
    unsigned int tex;
    int num_indices;
    int num_vertices;
    unsigned int program;
} OpenGLContext;

typedef struct
{
    float position[3];
    float color[4];
    float tex_coords[2];
} GLVertex;

typedef unsigned int GLIndex;

void glc_prepare_context(int gl_profile, int gl_major, int gl_minor);
OpenGLContext* glc_create_context();
void glc_fill_vertex_buffer(OpenGLContext* ctx, GLVertex* vertices, int num);
void glc_fill_index_buffer(OpenGLContext* ctx, GLIndex* indices, int num);
void glc_create_texture(OpenGLContext* ctx, Image* img);
void glc_create_program(OpenGLContext* ctx, const char* vertex_file, const char* fragment_file);
int glc_get_uniform_location(OpenGLContext* ctx, const char* name);
void glc_transmit_matrix(OpenGLContext* ctx, int loc, mat4_t mat);
void glc_clear_screen(float r, float g, float b, float a);
void glc_render_indices(OpenGLContext* ctx);
void glc_destroy_context(OpenGLContext* ctx);

#endif // OPENGL_CONTEXT_H
