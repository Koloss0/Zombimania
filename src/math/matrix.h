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
		float m00, m01, m02, m03,
		      m10, m11, m12, m13,
		      m20, m21, m22, m23,
		      m30, m31, m32, m33;
	};
} Mat4;

extern const Mat3 MAT3_ZERO;
extern const Mat3 MAT3_IDENTITY;
extern const Mat4 MAT4_ZERO;
extern const Mat4 MAT4_IDENTITY;

Mat4 mat4_ortho(float left, float bottom, float right, float top);

#endif
