#ifndef CAMERA_H
#define CAMERA_H

typedef struct
{
	double x, y, z;
	double rx, ry;
} Camera;

extern const Camera CAMERA_DEFAULT;

#endif
