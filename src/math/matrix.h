#ifndef MATRIX_H
#define MATRIX_H

#define MAT3_SIZE 9
#define MAT4_SIZE 16

typedef union
{
	float entries[MAT3_SIZE];

	struct
	{
		float m00, m01, m02,
		      m10, m11, m12,
		      m20, m21, m22;
	};
} Mat3;

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

extern const Mat3 MAT3_ZERO;
extern const Mat3 MAT3_IDENTITY;
extern const Mat4 MAT4_ZERO;
extern const Mat4 MAT4_IDENTITY;

Mat4 mat4_ortho(float left, float bottom, float right, float top);
Mat4 mat4_perspective(double fov, double aspect, double near, double far);

void mat4_rotate_y(Mat4* mat4, double rad);

#endif
