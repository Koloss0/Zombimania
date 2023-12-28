#include "shader.h"
#include "gl.h"
#include "core/log.h"
#include "math/matrix.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LINE_LENGTH 1024

static const char* VERT_LABEL = "/// VERT\n";
static const char* FRAG_LABEL = "/// FRAG\n";

enum ParseError
{
	OK = 0,
	ERROR = -1,
	OUT_OF_MEMORY = -2,
	BAD_FORMAT = -3
};

static enum ParseError parse_shader_file(const char* path, char** vertex, char** fragment)
{
	FILE* fp = fopen(path, "r");

	if (!fp)
	{
		perror("cannot read shader file");
		return ERROR;
	}

	char* vert_buf = malloc(1);
	char* frag_buf = malloc(1);

	if (!vert_buf || !frag_buf)
	{
		LOG_ERROR("out of memory.");

		if (vert_buf)
			free(vert_buf);
		
		if (frag_buf)
			free(frag_buf);

		return OUT_OF_MEMORY;
	}

	vert_buf[0] = '\0';
	frag_buf[0] = '\0';

	size_t vert_len = 0;
	size_t frag_len = 0;

	enum Section { NONE, VERT, FRAG };
	enum Section section = NONE;

	bool has_vert = false;
	bool has_frag = false;
	
	char line[MAX_LINE_LENGTH];

	while (fgets(line, sizeof(line), fp))
	{
		if (strcmp(line, VERT_LABEL) == 0)
		{
			section = VERT;
			has_vert = true;
		}
		else if (strcmp(line, FRAG_LABEL) == 0)
		{
			section = FRAG;
			has_frag = true;
		}
		else
		{
			char* new_buf;

			switch (section)
			{
				case VERT:
					vert_len += strlen(line);
					new_buf = realloc(vert_buf, vert_len + 1);

					if (!new_buf)
					{
						LOG_ERROR("out of memory.");

						free(vert_buf);
						free(frag_buf);
						return OUT_OF_MEMORY;
					}

					vert_buf = new_buf;
					strcat(vert_buf, line);

					break;
				case FRAG:
					frag_len += strlen(line);
					new_buf = realloc(frag_buf, frag_len + 1);
					
					if (!new_buf)
					{
						LOG_ERROR("out of memory.");

						free(vert_buf);
						free(frag_buf);
						return OUT_OF_MEMORY;
					}

					frag_buf = new_buf;
					strcat(frag_buf, line);

					break;
				default:
					break;
			}
		}
	}

	if (!has_vert)
	{
		LOG_ERROR("missing vertex shader in '%s'.", path);

		free(vert_buf);
		free(frag_buf);
		return BAD_FORMAT;
	}
	
	if (!has_frag)
	{
		LOG_ERROR("missing fragment shader in '%s'.", path);

		free(vert_buf);
		free(frag_buf);
		return BAD_FORMAT;
	}

	*vertex = vert_buf;
	*fragment = frag_buf;

	return OK;
}

Shader shader_create(const char* path)
{
	Shader shader = { .id = 0 };
	GLuint vs, fs;

	GLint success;
	GLchar info_log[512];
	
	char* vs_source = NULL;
	char* fs_source = NULL;

	parse_shader_file(path, &vs_source, &fs_source);

	if (!vs_source || !fs_source)
	{
		if (vs_source)
			free(vs_source);
		
		if (fs_source)
			free(fs_source);

		return shader;
	}

	GL_CMD(vs = glCreateShader(GL_VERTEX_SHADER));
	GL_CMD(glShaderSource(vs, 1, (const GLchar* const*)&vs_source, NULL));
	GL_CMD(glCompileShader(vs));

	glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vs, 512, NULL, info_log);
		glDeleteShader(vs);

		LOG_ERROR("failed to compile vertex shader.\ninfo: %s", info_log);

		free(vs_source);
		free(fs_source);

		return shader;
	}
	
	GL_CMD(fs = glCreateShader(GL_FRAGMENT_SHADER));
	GL_CMD(glShaderSource(fs, 1, (const GLchar* const*)&fs_source, NULL));
	GL_CMD(glCompileShader(fs));

	glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fs, 512, NULL, info_log);
		glDeleteShader(vs);
		glDeleteShader(fs);
		
		LOG_ERROR("failed to compile fragment shader.\ninfo: %s", info_log);

		free(vs_source);
		free(fs_source);

		return shader;
	}

	GLuint program;
	GL_CMD(program = glCreateProgram());
	GL_CMD(glAttachShader(program, vs));
	GL_CMD(glAttachShader(program, fs));
	GL_CMD(glLinkProgram(program));
	GL_CMD(glDeleteShader(vs));
	GL_CMD(glDeleteShader(fs));

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, info_log);
		
		LOG_ERROR("failed to link shader program.\ninfo: %s", info_log);

		free(vs_source);
		free(fs_source);

		return shader;
	}

	shader.id = program;

	return shader;
}

void shader_destroy(const Shader shader)
{
	GL_CMD(glDeleteProgram(shader.id));
}

void shader_use(const Shader shader)
{
	GL_CMD(glUseProgram(shader.id));
}

void shader_set_int(const Shader shader, const char* name, GLint value)
{
	GLint int_uniform;
	GL_CMD(int_uniform = glGetUniformLocation(shader.id, name));
	GL_CMD(glUniform1i(int_uniform, value));
}

void shader_set_mat3(const Shader shader, const char* name, const Mat3* mat3)
{
	GLint mat_uniform;
	GL_CMD(mat_uniform = glGetUniformLocation(shader.id, name));
	GL_CMD(glUniformMatrix3fv(mat_uniform, 1, GL_FALSE, mat3->entries));
}

void shader_set_mat4(const Shader shader, const char* name, const Mat4* mat4)
{
	GLint mat_uniform;
	GL_CMD(mat_uniform = glGetUniformLocation(shader.id, name));
	GL_CMD(glUniformMatrix4fv(mat_uniform, 1, GL_FALSE, mat4->entries));
}
