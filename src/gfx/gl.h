#ifndef GL_H
#define GL_H

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

GLenum gl_verify_error_(const char* file, int line);

#define GL_VERIFY_ERROR() gl_verify_error_(__FILE__, __LINE__)

#define GL_CMD(X) do { X; GL_VERIFY_ERROR(); } while(0)

#endif
