#include "matrix.h"
#include "math/math.h"
#include "core/assert.h"

#include <math.h>
#include <stdlib.h>

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

static void rotate(float* x, float* y, double rad);

Mat4 mat4_ortho(float left, float bottom, float right, float top)
{
	const float NEAR = -1.0f;
	const float FAR =   1.0f;

    float x_scale =  2.0f / (right - left);
    float y_scale =  2.0f / (top - bottom);
	float z_scale = -2.0f / (FAR - NEAR);

	float x = -(right + left) / (right - left);
	float y = -(top + bottom) / (top - bottom);
	float z = -(FAR + NEAR)   / (FAR - NEAR);

    Mat4 mat = {
		.entries = {
			x_scale, 0,       0,       0,
			0,       y_scale, 0,       0,
			0,       0,       z_scale, 0,
			x,       y,       z,       1
		}
    };

	return mat;
}

Mat4 mat4_perspective(double fov, double aspect, double near, double far)
{
	ASSERT(near > 0.0, "near must be positive.");
	ASSERT(far > 0.0, "far must be positive.");

	double f = 1.0 / tan(math_deg_to_rad(fov) / 2.0);

	float x_scale = (float)(f / aspect);
	float y_scale = (float)f;
	float z_scale = (float)((far + near) / (near - far));

	float z = (float)(2.0 * far * near / (near - far));

	Mat4 mat = {
		.entries = {
			x_scale, 0,       0,       0,
			0,       y_scale, 0,       0,
			0,       0,       z_scale, -1,
			0,       0,       z,       0
		}
    };

	return mat;
}

void mat4_rotate_y(Mat4* mat, double rad)
{
	rotate(&mat->x_basis_x, &mat->x_basis_z, rad);
	rotate(&mat->z_basis_x, &mat->z_basis_z, rad);
}

static void rotate(float* x, float* y, double rad)
{
	float px = *x;
	float py = *y;

	double c = cos(rad);
	double s = sin(rad);

	*x = (float)(px*c - py*s);
	*y = (float)(py*c + px*s);
}