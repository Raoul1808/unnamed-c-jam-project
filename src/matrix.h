#ifndef MATRIX_H
# define MATRIX_H

typedef float	vec4_t[4];
typedef float	mat4_t[16];

void	mat_identity(mat4_t *mat);
void	mat_add(mat4_t mat1, mat4_t mat2, mat4_t *res);
void	mat_mul(mat4_t mat1, mat4_t mat2, mat4_t *res);

void	mat_transf(mat4_t mat, vec4_t vec, mat4_t *res);

#endif
