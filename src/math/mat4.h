#ifndef MAT4_H
#define MAT4_H

#define MAT4_SIZE 16

typedef union
{
	float entries[MAT4_SIZE];

	struct
	{
		float right_x,   right_y,   right_z,   zero_0,
		      up_x,      up_y,      up_z,      zero_1,
		      forward_x, forward_y, forward_z, zero_2,
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
