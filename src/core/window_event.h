#ifndef WINDOW_EVENT_H
#define WINDOW_EVENT_H

typedef enum {
	CLOSE_EVENT = 0,
	RESIZE_EVENT,
	KEY_EVENT,
	MOUSE_BUTTON_EVENT,
	MOUSE_MOVE_EVENT
} WindowEventID;

typedef struct
{
	WindowEventID id;
	void* data;
} WindowEvent;

typedef struct
{
	int width, height;
} ResizeEventData;

typedef struct
{
	int key, scancode, action, mods;
} KeyEventData;

typedef struct
{
	int button, action, mods;
} MouseButtonEventData;

typedef struct
{
	double x, y, delta_x, delta_y;
} MouseMoveEventData;

#endif