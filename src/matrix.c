#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

mat4_t	*mat_add(mat4_t mat1, mat4_t mat2)
{
	mat4_t *res;
	res = malloc(sizeof(mat4_t));
	if (!res) return NULL;
	for (int i = 0; i < 16; i++)
		res[0][i] = mat1[i] + mat2[i];
	return res;
}

mat4_t	*mat_mul(mat4_t mat1, mat4_t mat2)
{
	mat4_t *res;
	res = malloc(sizeof(mat4_t));
	if (!res) return NULL;
        for (int i = 0; i < 16; i++)
	{
		res[0][i] = 0;
		for (int j = 0; j < 4; j++)
		{
                	res[0][i] += mat1[j+(i/4)*4] * mat2[(i%4)+(j*4)];
		}
	}
        return res;
}

void	mat_trans(mat4_t *mat, vec4_t vec)
{
	for (int i = 0; i < 16; i++)
		mat[0][i] *= vec[i%4];
}
