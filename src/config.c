#include "config.h"

const WindowSettings WINDOW_SETTINGS = {
	.width = 480,
	.height = 300,
	.title = PROJECT_NAME " v" ZBM_VERSION,
	.fullscreen = false,
	.vsync = false
};

const int VIEWPORT_WIDTH = 320;
const int VIEWPORT_HEIGHT = 200;

const GameStateID INITIAL_STATE = MAIN_MENU;