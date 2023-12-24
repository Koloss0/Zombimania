#include "matrix.h"

const Mat3 MAT3_ZERO = {
	.entries = {
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f
	}
};

const Mat3 MAT3_IDENTITY = {
	.entries = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	}
};

const Mat4 MAT4_ZERO = {
	.entries = {
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f
	}
};

const Mat4 MAT4_IDENTITY = {
	.entries = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	}
};

/*
static void multiplyMatrix(float* m1, size_t rows1, size_t cols1, float* m2, size_t rows1, size_t cols1)
{
	float result[R1][C2];

	for (int i = 0; i < R1; i++) {
		for (int j = 0; j < C2; j++) {
			result[i][j] = 0;

			for (int k = 0; k < R2; k++) {
				result[i][j] += m1[i][k] * m2[k][j];
			}
		}
	}
}
*/

Mat3 mat3_mult(Mat3 m1, Mat3 m2)
{
	Mat3 result;

	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			int index = 3*row + col;

			result.entries[index] = 0;

			for (int i = 0; i < 3; i++)
			{
				result.entries[index] += m1.entries[3*row + i] * m2.entries[col + 3*i];
			}
		}
	}

	return result;
}

Mat4 mat4_mult(Mat4 m1, Mat4 m2)
{
	Mat4 result;
	
	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			int index = 3*row + col;

			result.entries[index] = 0;

		}
	}

	return result;
}
