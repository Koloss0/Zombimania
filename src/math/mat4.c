#include "mat4.h"
#include "math/math.h"
#include "core/assert.h"

#include <math.h>

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

Mat4 mat4_rotation_x(double rad)
{
	float c = (float)cos(rad);
	float s = (float)sin(rad);

	Mat4 mat = {
		.entries = {
			 1,  0,  0,  0,
			 0,  c,  s,  0,
			 0, -s,  c,  0,
			 0,  0,  0,  1
		}
    };
	
	return mat;
}

Mat4 mat4_rotation_y(double rad)
{
	float c = (float)cos(rad);
	float s = (float)sin(rad);

	Mat4 mat = {
		.entries = {
			 c,  0, -s,  0,
			 0,  1,  0,  0,
			 s,  0,  c,  0,
			 0,  0,  0,  1
		}
    };
	
	return mat;
}

Mat4 mat4_rotation_z(double rad)
{
	float c = (float)cos(rad);
	float s = (float)sin(rad);

	Mat4 mat = {
		.entries = {
			 c,  s,  0,  0,
			-s,  c,  0,  0,
			 0,  0,  1,  0,
			 0,  0,  0,  1
		}
    };
	
	return mat;
}

Mat4 mat4_mult(Mat4* m1, Mat4* m2)
{
	Mat4 result = MAT4_ZERO;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				result.entries[i + 4*j] += m1->entries[i + 4*k] * m2->entries[k + 4*j];
			}
		}
	}

	return result;
}

void mat4_rotate_y(Mat4* mat, double rad)
{
	math_rotatef(&mat->right_z, &mat->right_x, rad);
	math_rotatef(&mat->up_z, &mat->up_x, rad);
	math_rotatef(&mat->forward_z, &mat->forward_x, rad);

	// ( 0, -1) -> ( 1,  0)
	// (-1,  0) -> ( 0, -1)
	// ( 0,  1) -> (-1,  0)
	// ( 1,  0) -> ( 0,  1)
}

void mat4_rotate_x(Mat4* mat, double rad)
{
	math_rotatef(&mat->right_y, &mat->right_z, rad);
	math_rotatef(&mat->up_y, &mat->up_z, rad);
	math_rotatef(&mat->forward_y, &mat->forward_z, rad);
}