#include "opengl_context.h"

#include "SDL.h"
#include "glad/glad.h"
#include "read_data.h"

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

GLuint create_shader(const char* shader_path, GLenum shader_type)
{
    const char* glsl_source = read_data(shader_path,  NULL);
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &glsl_source, NULL);
    glCompileShader(shader);
    check_shader_compilation_status(shader);
    free((char*)glsl_source);
    return shader;
}

void glc_prepare_context(int gl_profile, int gl_major, int gl_minor)
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, gl_profile);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gl_major);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, gl_minor);
}

OpenGLContext* glc_create_context()
{
    if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
        return NULL;

    OpenGLContext* ctx = malloc(sizeof(OpenGLContext));
    if (ctx == NULL)
        return NULL;

    glGenVertexArrays(1, &ctx->vao);
    glBindVertexArray(ctx->vao);
    glGenBuffers(1, &ctx->vbo);
    glGenBuffers(1, &ctx->ibo);
    glBindBuffer(GL_ARRAY_BUFFER, ctx->vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ctx->ibo);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return ctx;
}

void glc_fill_vertex_buffer(OpenGLContext* ctx, GLVertex* vertices, int num)
{
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLVertex) * num, vertices, GL_STATIC_DRAW);
    ctx->num_vertices = num;
}

void glc_fill_index_buffer(OpenGLContext* ctx, GLIndex* indices, int num)
{
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLIndex) * num, indices, GL_STATIC_DRAW);
    ctx->num_indices = num;
}

void glc_create_texture(OpenGLContext* ctx, Image* img)
{
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->w, img->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->data);

    ctx->tex = tex;
}

void glc_create_program(OpenGLContext* ctx, const char* vertex_file, const char* fragment_file)
{
    GLuint vertex_shader = create_shader(vertex_file, GL_VERTEX_SHADER);
    GLuint fragment_shader = create_shader(fragment_file, GL_FRAGMENT_SHADER);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    glUseProgram(program);
    glDeleteShader(fragment_shader);
    glDeleteShader(vertex_shader);

    glBindFragDataLocation(program, 0, "fragColor");

    GLint position_attrib_location = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(position_attrib_location);
    glVertexAttribPointer(position_attrib_location, 3, GL_FLOAT, GL_FALSE, sizeof(GLVertex), (void*)offsetof(GLVertex, position));

    GLint color_attrib_location = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(color_attrib_location);
    glVertexAttribPointer(color_attrib_location, 4, GL_FLOAT, GL_FALSE, sizeof(GLVertex), (void*)offsetof(GLVertex, color));

    GLint texcoord_attrib_location = glGetAttribLocation(program, "vTexCoords");
    glEnableVertexAttribArray(texcoord_attrib_location);
    glVertexAttribPointer(texcoord_attrib_location, 2, GL_FLOAT, GL_FALSE, sizeof(GLVertex), (void*)offsetof(GLVertex, tex_coords));

    ctx->program = program;
}

int glc_get_uniform_location(OpenGLContext* ctx, const char* name)
{
    return glGetUniformLocation(ctx->program, name);
}

void glc_transmit_matrix(OpenGLContext* ctx, int loc, mat4_t mat)
{
    glUniformMatrix4fv(loc, 1, GL_TRUE, mat);
}

void glc_clear_screen(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void glc_render_indices(OpenGLContext* ctx)
{
    glDrawElements(GL_TRIANGLES, ctx->num_indices, GL_UNSIGNED_INT, NULL);
}

void glc_destroy_context(OpenGLContext* ctx)
{
    glDeleteProgram(ctx->program);
    glDeleteBuffers(1, &ctx->ibo);
    glDeleteBuffers(1, &ctx->vbo);
    glDeleteVertexArrays(1, &ctx->vao);
    free(ctx);
}
