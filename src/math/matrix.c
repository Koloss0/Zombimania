#include "matrix.h"
#include "math.h"

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

Mat4 mat4_ortho(float left, float bottom, float right, float top)
{
    const float DEG_TO_RAD = PI / 180.0f;
	const float NEAR = -1.0f;
	const float FAR =   1.0f;

    float x_scale =  2.0 / (right - left);
    float y_scale =  2.0 / (top - bottom);
	float z_scale = -2.0 / (FAR - NEAR);

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