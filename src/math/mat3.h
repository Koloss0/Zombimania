#ifndef MAT3_H
#define MAT3_H

#define MAT3_SIZE 9

typedef union
{
	float entries[MAT3_SIZE];

	struct
	{
		float right_x, right_y, zero_0,
		      up_x,    up_y,    zero_1,
		      x,       y,       z;
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
