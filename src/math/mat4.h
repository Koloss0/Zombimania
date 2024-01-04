#ifndef MAT4_H
#define MAT4_H

#define MAT4_SIZE 16

typedef union
{
	float entries[MAT4_SIZE];

	struct
	{
		float x_basis_x, x_basis_y, x_basis_z, zero_0,
		      y_basis_x, y_basis_y, y_basis_z, zero_1,
		      z_basis_x, z_basis_y, z_basis_z, zero_2,
		      x,         y,         z,         w;
	};
} Mat4;

#define MAT4_ZERO {             \
	.entries = {                \
		0.0f, 0.0f, 0.0f, 0.0f, \
		0.0f, 0.0f, 0.0f, 0.0f, \
		0.0f, 0.0f, 0.0f, 0.0f, \
		0.0f, 0.0f, 0.0f, 0.0f  \
	}                           \
}

#define MAT4_IDENTITY {         \
	.entries = {                \
		1.0f, 0.0f, 0.0f, 0.0f, \
		0.0f, 1.0f, 0.0f, 0.0f, \
		0.0f, 0.0f, 1.0f, 0.0f, \
		0.0f, 0.0f, 0.0f, 1.0f  \
	}                           \
}

static inline Mat4 mat4_translation(double x, double y, double z)
{
	float tx = (float)x;
	float ty = (float)y;
	float tz = (float)z;

	Mat4 mat = {
		.entries = {
			 1,  0,  0,  0,
			 0,  1,  0,  0,
			 0,  0,  1,  0,
			 tx, ty, tz, 1
		}
    };

	return mat;
}

Mat4 mat4_ortho(float left, float bottom, float right, float top);
Mat4 mat4_perspective(double fov, double aspect, double near, double far);
Mat4 mat4_rotation_x(double rad);
Mat4 mat4_rotation_y(double rad);
Mat4 mat4_rotation_z(double rad);

Mat4 mat4_mult(Mat4* m1, Mat4* m2);

void mat4_rotate_x(Mat4* mat, double rad);
void mat4_rotate_y(Mat4* mat, double rad);

static inline void mat4_translate(Mat4* mat, double x, double y, double z)
{
	mat->x += (float)x;
	mat->y += (float)y;
	mat->z += (float)z;
}

#endif
