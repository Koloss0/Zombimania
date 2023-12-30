#ifndef BMP_H
#define BMP_H

#include "image.h"

#include <stdbool.h>

// image must be manually destroyed after final use.
Image io_load_bmp(const char* path, bool vflip);

#endif
