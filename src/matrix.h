#ifndef MATRIX_H
# define MATRIX_H

typedef float	vec4_t[4];
typedef float	mat4_t[16];

mat4_t	*mat_add(mat4_t mat1, mat4_t mat2);
mat4_t	*mat_mul(mat4_t mat1, mat4_t mat2);

void	mat_trans(mat4_t *mat, vec4_t vec);


#endif
