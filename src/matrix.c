#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

void	mat_add(mat4_t mat1, mat4_t mat2, mat4_t *res)
{
	for (int i = 0; i < 16; i++)
		res[0][i] = mat1[i] + mat2[i];
}

void	mat_mul(mat4_t mat1, mat4_t mat2, mat4_t *res)
{
        for (int i = 0; i < 16; i++)
	{
		res[0][i] = 0;
		for (int j = 0; j < 4; j++)
		{
			res[0][i] += mat1[j+(i/4)*4] * mat2[(i%4)+(j*4)];
		}
	}
}

void	mat_identity(mat4_t *mat)
{
	for (int i = 0; i < 16; i++)
		mat[0][i] = i%5==0;
}

void	mat_transf(mat4_t mat, vec4_t vec, mat4_t *res)
{
	for (int i = 0; i < 16; i++)
		res[0][i] = mat[i] * vec[i%4];
}
