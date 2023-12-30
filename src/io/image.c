#include "image.h"

#include <stdlib.h>
#include <stdio.h>

void image_destroy(Image img)
{
	free(img.data);
}

void image_print(Image img)
{
	unsigned long w = img.width;
	unsigned long h = img.height;
	
	printf("Raw Image Data:\n");
	for (unsigned long y = 0; y < h; y++)
	{
		for (unsigned long x = 0; x < w; x++)
		{
			printf("( ");
			for (unsigned int b = 0; b < IMG_BYTES_PER_PIX; b++)
			{
				printf("0x%02x ", img.data[y*w*IMG_BYTES_PER_PIX + x*IMG_BYTES_PER_PIX + b]);
			}
			printf(") ");
		}
		printf("\n");
	}
}