#include "camera.h"

#include "math/matrix.h"

const Camera CAMERA_DEFAULT = {
	.x = 0.0, .y = 0.0, .z = 0.0,
	.rx = 0.0, .ry = 0.0,
	.transform = MAT4_IDENTITY,
	.view_mat = MAT4_IDENTITY
};

void camera_set_translation(Camera* camera, double x, double y, double z)
{
	Mat4 rotation = (Mat4)MAT4_IDENTITY;
	mat4_rotate_y(&rotation, -camera->ry);
	mat4_rotate_x(&rotation, -camera->rx);

	camera->view_mat = mat4_translation(-x, -y, -z);
	camera->view_mat = mat4_mult(&rotation, &camera->view_mat);

	camera->transform.x = (float)x;
	camera->transform.y = (float)y;
	camera->transform.z = (float)z;

	camera->x = x;
	camera->y = y;
	camera->z = z;
}

void camera_translate(Camera* camera, double x, double y, double z)
{
	camera_set_translation(camera, camera->x + x, camera->y + y, camera->z + z);
}

void camera_set_rotation(Camera* camera, double x, double y)
{
	Mat4 rotation = (Mat4)MAT4_IDENTITY;
	mat4_rotate_y(&rotation, -y);
	mat4_rotate_x(&rotation, -x);

	camera->view_mat = mat4_translation(-camera->x, -camera->y, -camera->z);
	camera->view_mat = mat4_mult(&rotation, &camera->view_mat);

	camera->transform = (Mat4)MAT4_IDENTITY;
	mat4_rotate_x(&camera->transform, x);
	mat4_rotate_y(&camera->transform, y);
	
	camera->transform.x = (float)camera->x;
	camera->transform.y = (float)camera->y;
	camera->transform.z = (float)camera->z;

	camera->rx = x;
	camera->ry = y;
}

void camera_rotate(Camera* camera, double x, double y)
{
	camera_set_rotation(camera, camera->rx + x, camera->ry + y);
}

/*
// rotates the camera from a local frame of reference.
void camera_rotate_local(Camera* camera, double x, double y)
	Mat4 rotation = (Mat4)MAT4_IDENTITY;
	mat4_rotate_y(&rotation, -y);
	mat4_rotate_x(&rotation, -x);

	camera->view_mat = mat4_mult(&rotation, &camera->view_mat);

	camera->rx += x;
	camera->ry += y;
*/

void camera_set_transform(Camera* camera, double x, double y, double z, double rx, double ry)
{
	camera_set_translation(camera, x, y, z);
	camera_set_rotation(camera, rx, ry);
}