#include "gl.h"

#include <stdio.h>

GLenum gl_verify_error_(const char* file, int line)
{
	GLenum error = glGetError();

	while (error != GL_NO_ERROR)
	{
		const char* str;

		switch (error)
		{
			case GL_INVALID_ENUM:
				str = "INVALID_ENUM";
				break;
			case GL_INVALID_VALUE:
				str = "INVALID_VALUE";
				break;
			case GL_INVALID_OPERATION:
				str = "INVALID_OPERATION";
				break;
			case GL_OUT_OF_MEMORY:
				str = "OUT_OF_MEMORY";
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				str = "INVALID_FRAMEBUFFER_OPERATION";
				break;
			default:
				str = "Unkown";
				break;
		}

		fprintf(stderr, "[GL] OpenGL Error: %s (%s, line %i)\n", str, file, line);
	
		error = glGetError();
	}

	return error;
}

