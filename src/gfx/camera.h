#ifndef CAMERA_H
#define CAMERA_H

#include "math/matrix.h"

typedef struct
{
	double x, y, z;
	double rx, ry;
	Mat4 view_mat;
} Camera;

extern const Camera CAMERA_DEFAULT;

void camera_set_translation(Camera* camera, double x, double y, double z);
void camera_set_rotation(Camera* camera, double x, double y);
void camera_translate(Camera* camera, double x, double y, double z);
void camera_rotate(Camera* camera, double x, double y);
void camera_set_transform(Camera* camera, double x, double y, double z, double rx, double ry);

#endif
