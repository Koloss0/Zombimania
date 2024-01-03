#include "window.h"

#include "gfx/gl.h"

#include "core/log.h"
#include "core/assert.h"

#include <stdlib.h>
#include <stdbool.h>

struct Window
{
	GLFWwindow* glfw_window;
	WindowEventCallback event_callback;
};

static unsigned int num_windows = 0;

static GLFWwindow* create_glfw_window(
		int width, int height,
		const char* title, GLFWmonitor* monitor);
static void on_glfw_error(int error, const char* desc);
static void close_callback(GLFWwindow* glfw_window);
static void resize_callback(GLFWwindow* glfw_window, int width, int height);
static void key_callback(GLFWwindow* glfw_window, int key, int scancode, int action, int mods);
static void mouse_button_callback(GLFWwindow* glfw_window, int button, int action, int mods);
static void cursor_position_callback(GLFWwindow* glfw_window, double x, double y);
static void cursor_pos_to_screen_pos(const Window* window, double* x, double* y);

Window* window_create(const WindowSettings* window_settings)
{
	Window* window = calloc(1, sizeof(Window));

	if (!window)
	{
		LOG_ERROR("failed to create window: out of memory.");
		return NULL;
	}

	if (num_windows == 0)
	{
		if (!glfwInit())
		{
			LOG_ERROR("failed to create window: failed to initialise GLFW.");
			free(window);
			glfwTerminate();
			return NULL;
		}

		glfwSetErrorCallback(on_glfw_error);
	}
	
	GLFWmonitor* monitor = NULL;
	if (window_settings->fullscreen)
	{
		monitor = glfwGetPrimaryMonitor();
	}

	GLFWwindow* glfw_win = create_glfw_window(
			(int)window_settings->width,
			(int)window_settings->height,
			window_settings->title,
			monitor
			);

	if (!glfw_win)
	{
		LOG_ERROR("failed to create window: failed to create GLFW window.");
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
			LOG_ERROR("failed to create window: failed to load GLAD.");
			window_destroy(window);
			glfwTerminate();
			return NULL;
		}
	}
	
	glfwSetWindowUserPointer(glfw_win, window);

	if (window_settings->vsync)
	{
		glfwSwapInterval(1);
	}

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

void window_set_event_callback(Window* window, WindowEventCallback callback)
{
	window->event_callback = callback;
}

void window_set_mouse_mode(const Window* window, MouseMode mouse_mode)
{
	int mode;

	switch (mouse_mode)
	{
		case MOUSE_MODE_NORMAL:
			mode = GLFW_CURSOR_NORMAL;
			break;
		case MOUSE_MODE_CAPTURED:
			mode = GLFW_CURSOR_DISABLED;
			break;
		case MOUSE_MODE_HIDDEN:
			mode = GLFW_CURSOR_HIDDEN;
			break;
		default:
			ASSERT(false, "unknown mouse mode: %d.", mouse_mode);
			return;
	}

	glfwSetInputMode(window->glfw_window, GLFW_CURSOR, mode);
}

void window_get_size(Window* window, int* width, int* height)
{
	glfwGetWindowSize(window->glfw_window, width, height);
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
	double cx, cy;
	glfwGetCursorPos(window->glfw_window, &cx, &cy);
	cursor_pos_to_screen_pos(window, &cx, &cy);

	*x = cx;
	*y = cy;
}

static GLFWwindow* create_glfw_window(
		int width, int height,
		const char* title, GLFWmonitor* monitor)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	glfwWindowHint(GLFW_SCALE_TO_MONITOR, GL_TRUE);

	if (monitor)
	{
		// Fix content area size when playing in fullscreen.
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		if (mode)
		{
			width = mode->width;
			height = mode->height;
		}
	}

	GLFWwindow* window = glfwCreateWindow(width, height, title, monitor, NULL);
	
	if (!window)
		return NULL;
	
	glfwSetWindowCloseCallback(window, close_callback);
	glfwSetFramebufferSizeCallback(window, resize_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	return window;
}

static void on_glfw_error(int error, const char* desc)
{
	LOG_ERROR("GLFW Error: %i, Info: %s", error, desc);
}

static void close_callback(GLFWwindow* glfw_window)
{
	Window* window = (Window*)glfwGetWindowUserPointer(glfw_window);

	WindowEvent event = {
		.id = CLOSE_EVENT,
		.data = NULL
	};
	
	window->event_callback(event);
}

static void resize_callback(GLFWwindow* glfw_window, int width, int height)
{
	Window* window = (Window*)glfwGetWindowUserPointer(glfw_window);
	
	ResizeEventData data = {
		.width = width,
		.height = height
	};

	WindowEvent event = {
		.id = RESIZE_EVENT,
		.data = &data
	};
	
	window->event_callback(event);
}

static void key_callback(GLFWwindow* glfw_window, int key, int scancode, int action, int mods)
{
	Window* window = (Window*)glfwGetWindowUserPointer(glfw_window);
	
	KeyEventData data = {
		.key = key,
		.scancode = scancode,
		.action = action,
		.mods = mods
	};

	WindowEvent event = {
		.id = KEY_EVENT,
		.data = &data
	};
	
	window->event_callback(event);
}

static void mouse_button_callback(GLFWwindow* glfw_window, int button, int action, int mods)
{
	Window* window = (Window*)glfwGetWindowUserPointer(glfw_window);
	
	MouseButtonEventData data = {
		.button = button,
		.action = action,
		.mods = mods
	};

	WindowEvent event = {
		.id = MOUSE_BUTTON_EVENT,
		.data = &data
	};
	
	window->event_callback(event);
}

static void cursor_position_callback(GLFWwindow* glfw_window, double x, double y)
{
	Window* window = (Window*)glfwGetWindowUserPointer(glfw_window);
	
	cursor_pos_to_screen_pos(window, &x, &y);

	MouseMoveEventData data = {
		.x = x,
		.y = y
	};

	WindowEvent event = {
		.id = MOUSE_MOVE_EVENT,
		.data = &data
	};
	
	window->event_callback(event);
}

void cursor_pos_to_screen_pos(const Window* window, double* x, double* y)
{
	int width, height;
	glfwGetFramebufferSize(window->glfw_window, &width, &height);

	// flip y.
	*y = height - *y;
}