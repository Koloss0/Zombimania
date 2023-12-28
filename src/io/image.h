#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>

#define IMG_BPP 32
#define IMG_BYTES_PER_PIX IMG_BPP / 8

typedef struct
{
	uint8_t* data;
	unsigned long width;
	unsigned long height;
} Image;

void image_destroy(Image image);

void image_print();

#endif
