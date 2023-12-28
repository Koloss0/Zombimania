#include "window.h"

#include "gfx/gfx.h"
#include "gfx/gl.h"
#include "core/log.h"

#include <stdlib.h>

struct Window
{
	GLFWwindow* glfw_window;
};

static unsigned int num_windows = 0;

static GLFWwindow* create_glfw_window(unsigned int width, unsigned int height, const char* title);
static void on_glfw_error(int error, const char* desc);
static void resize_callback(GLFWwindow* window, int width, int height);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void cursor_position_callback(GLFWwindow* window, double x, double y);

Window* window_create(const WindowSettings* window_settings)
{
	Window* window = malloc(sizeof(Window));

	if (!window)
	{
		LOG_ERROR("failed to create window: out of memory");
		return NULL;
	}

	if (num_windows == 0)
	{
		if (!glfwInit())
		{
			LOG_ERROR("failed to create window: failed to initialise GLFW");
			free(window);
			glfwTerminate();
			return NULL;
		}

		glfwSetErrorCallback(on_glfw_error);
	}

	GLFWwindow* glfw_win = create_glfw_window(
			window_settings->width,
			window_settings->height,
			window_settings->title
		);

	if (!glfw_win)
	{
		LOG_ERROR("failed to create window: failed to create GLFW window");
		free(window);
		glfwTerminate();
		return NULL;
	}

	glfwMakeContextCurrent(glfw_win);

	window->glfw_window = glfw_win;

	if (num_windows == 0)
	{
		bool success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		if (!success)
		{
			LOG_ERROR("failed to create window: failed to load GLAD");
			window_destroy(window);
			glfwTerminate();
			return NULL;
		}
	}
	
	glfwSetWindowUserPointer(glfw_win, window);

	glfwSetFramebufferSizeCallback(glfw_win, resize_callback);
	glfwSetKeyCallback(glfw_win, key_callback);
	glfwSetCursorPosCallback(glfw_win, cursor_position_callback);

	// FIXME: this causes gfx_fit_viewport to be called before gfx_init is called.
	//resize_callback(glfw_win, (int)window_settings->width, (int)window_settings->height);

	num_windows++;

	return window;
}

void window_destroy(Window* window)
{
	glfwDestroyWindow(window->glfw_window);
	free(window);

	if (num_windows == 1)
	{
		glfwTerminate();
	}

	num_windows--;
}

void window_update(Window* window)
{
	glfwSwapBuffers(window->glfw_window);
	glfwPollEvents();
}

bool window_should_close(Window* window)
{
	return glfwWindowShouldClose(window->glfw_window);
}

bool window_is_key_pressed(const Window* window, int key)
{
	return glfwGetKey(window->glfw_window, key) == GLFW_PRESS;
}

bool window_is_mouse_pressed(const Window* window, int button)
{
	return glfwGetMouseButton(window->glfw_window, button) == GLFW_PRESS;
}

void window_get_mouse_pos(const Window* window, double* x, double* y)
{
	glfwGetCursorPos(window->glfw_window, x, y);
}

static GLFWwindow* create_glfw_window(unsigned int width, unsigned int height, const char* title)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* glfw_window = glfwCreateWindow((int)width, (int)height, title, NULL, NULL);
	
	if (!glfw_window)
		return NULL;

	return glfw_window;
}

static void on_glfw_error(int error, const char* desc)
{
	LOG_ERROR("GLFW Error: %i, Info: %s", error, desc);
}

static void resize_callback(GLFWwindow* window, int width, int height)
{
	//Window* user_win = (Window*)glfwGetWindowUserPointer(window);
	// TODO: use callback instead.
	gfx_fit_viewport(width, height);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

}

static void cursor_position_callback(GLFWwindow* window, double x, double y)
{

}
