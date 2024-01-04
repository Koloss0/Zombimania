#ifndef MAT3_H
#define MAT3_H

#define MAT3_SIZE 9

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

#define MAT3_ZERO {       \
	.entries = {          \
		0.0f, 0.0f, 0.0f, \
		0.0f, 0.0f, 0.0f, \
		0.0f, 0.0f, 0.0f  \
	}                     \
}

#define MAT3_IDENTITY {   \
	.entries = {          \
		1.0f, 0.0f, 0.0f, \
		0.0f, 1.0f, 0.0f, \
		0.0f, 0.0f, 1.0f  \
	}                     \
}

#endif
