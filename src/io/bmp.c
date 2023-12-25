#include "bmp.h"
#include "image.h"
#include "core/log.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/////////////////////////////////////////
// Headers
/////////////////////////////////////////
typedef struct
{
	uint8_t magic_number[2];
	uint32_t file_size;
	uint8_t reserved[4];
	uint32_t data_offset;
} BMPHeader;

#define BMP_HEADER_BYTES (2 + 4 + 4 + 4)

typedef struct
{
	uint32_t header_size;
	int32_t width;
	int32_t height;
	uint16_t colour_planes;
	uint16_t bpp;
	uint32_t compression;
	uint32_t img_size;
	int32_t x_ppm;
	int32_t y_ppm;
	uint32_t clr_used;
	uint32_t clr_important;
	uint32_t red_mask;
	uint32_t green_mask;
	uint32_t blue_mask;
	uint32_t alpha_mask;
	uint32_t cs_type;
	int32_t red_x;
	int32_t red_y;
	int32_t red_z;
	int32_t green_x;
	int32_t green_y;
	int32_t green_z;
	int32_t blue_x;
	int32_t blue_y;
	int32_t blue_z;
	uint32_t gamma_red;
	uint32_t gamma_green;
	uint32_t gamma_blue;
	uint32_t intent;
	uint32_t profile_data;
	uint32_t profile_size;
	uint32_t reserved;
} DIBHeader;

// DIB Headers
#define BITMAP_CORE_HEADER_BYTES (4 + 2 + 2 + 2 + 2)
#define BITMAP_INFO_HEADER_BYTES 40
#define BITMAP_V2_INFO_HEADER_BYTES 52
#define BITMAP_V3_INFO_HEADER_BYTES 56
#define BITMAP_V4_HEADER_BYTES 108
#define BITMAP_V5_HEADER_BYTES 124

static uint16_t le_to_cpu_16(const uint8_t *buf)
{
   return (uint16_t)((uint16_t)buf[0] | (uint16_t)buf[1] << 8);
}

static uint32_t le_to_cpu_32(const uint8_t *buf)
{
   return (uint32_t)buf[0] |
	  (uint32_t)buf[1] << 8 |
	  (uint32_t)buf[2] << 16 |
	  (uint32_t)buf[3] << 24;
}

static int read_bmp_header(BMPHeader* h, FILE* f)
{
	uint8_t buf[BMP_HEADER_BYTES];

	if (fread(buf, 1, sizeof(buf), f) != sizeof(buf))
	{
		LOG_ERROR("Failed to load BMP: failed to read file header.");
		return -1;
	}

	h->magic_number[0] = buf[0];
	h->magic_number[1] = buf[1];

	h->file_size = le_to_cpu_32(buf + 2);

	h->reserved[0] = buf[6];
	h->reserved[1] = buf[7];
	h->reserved[2] = buf[8];
	h->reserved[3] = buf[9];

	h->data_offset = le_to_cpu_32(buf+10);

	return 0;
}

static int read_dib_header(DIBHeader* h, FILE* f)
{
	uint8_t buf[BITMAP_V5_HEADER_BYTES];

	if (fread(buf, 1, sizeof(uint32_t), f) != sizeof(uint32_t))
	{
		LOG_ERROR("Failed to load BMP.");
		return -1;
	}
	
	uint32_t header_size = le_to_cpu_32(buf);

	if (header_size != BITMAP_CORE_HEADER_BYTES &&
	    header_size != BITMAP_INFO_HEADER_BYTES &&
	    header_size != BITMAP_V2_INFO_HEADER_BYTES &&
	    header_size != BITMAP_V3_INFO_HEADER_BYTES &&
	    header_size != BITMAP_V4_HEADER_BYTES &&
	    header_size != BITMAP_V5_HEADER_BYTES)
	{
		LOG_ERROR("Failed to load BMP: unsupported DIB header (%u bytes).", header_size);
		return -1;
	}

	uint32_t rem_bytes = header_size - 4;

	if (fread(buf + sizeof(uint32_t), 1, rem_bytes, f) != rem_bytes)
	{
		LOG_ERROR("Failed to load BMP.");
		return -1;
	}
	
	h->header_size = header_size;
	
	if (header_size == BITMAP_CORE_HEADER_BYTES)
	{
		h->width         = (int32_t)le_to_cpu_16(buf + 4);
		h->height        = (int32_t)le_to_cpu_16(buf + 6);
		h->colour_planes =          le_to_cpu_16(buf + 8);
		h->bpp           =          le_to_cpu_16(buf + 10);
	}
	else
	{
		// BITMAPINFOHEADER
		h->width         = (int32_t)le_to_cpu_32(buf + 4);
		h->height        = (int32_t)le_to_cpu_32(buf + 8);
		h->colour_planes =          le_to_cpu_16(buf + 12);
		h->bpp           =          le_to_cpu_16(buf + 14);
		h->compression   =          le_to_cpu_32(buf + 16);
		h->img_size      =          le_to_cpu_32(buf + 20);
		h->x_ppm         = (int32_t)le_to_cpu_32(buf + 24);
		h->y_ppm         = (int32_t)le_to_cpu_32(buf + 28);
		h->clr_used      =          le_to_cpu_32(buf + 32);
		h->clr_important =          le_to_cpu_32(buf + 36);

		if (header_size >= BITMAP_V2_INFO_HEADER_BYTES)
		{
			h->red_mask      =          le_to_cpu_32(buf + 40);
			h->green_mask    =          le_to_cpu_32(buf + 44);
			h->blue_mask     =          le_to_cpu_32(buf + 48);
		}

		if (header_size >= BITMAP_V3_INFO_HEADER_BYTES)
		{
			h->alpha_mask    =          le_to_cpu_32(buf + 52);
		}
		
		if (header_size >= BITMAP_V4_HEADER_BYTES)
		{
			h->cs_type       =          le_to_cpu_32(buf + 56);
			h->red_x         = (int32_t)le_to_cpu_32(buf + 60);
			h->red_y         = (int32_t)le_to_cpu_32(buf + 64);
			h->red_z         = (int32_t)le_to_cpu_32(buf + 68);
			h->green_x       = (int32_t)le_to_cpu_32(buf + 72);
			h->green_y       = (int32_t)le_to_cpu_32(buf + 76);
			h->green_z       = (int32_t)le_to_cpu_32(buf + 80);
			h->blue_x        = (int32_t)le_to_cpu_32(buf + 84);
			h->blue_y        = (int32_t)le_to_cpu_32(buf + 88);
			h->blue_z        = (int32_t)le_to_cpu_32(buf + 92);
			h->gamma_red     =          le_to_cpu_32(buf + 96);
			h->gamma_green   =          le_to_cpu_32(buf + 100);
			h->gamma_blue    =          le_to_cpu_32(buf + 104);
		}
		
		if (header_size == BITMAP_V5_HEADER_BYTES)
		{
			h->intent        =          le_to_cpu_32(buf + 108);
			h->profile_data  =          le_to_cpu_32(buf + 112);
			h->profile_size  =          le_to_cpu_32(buf + 116);
			h->reserved      =          le_to_cpu_32(buf + 120);
		}
	}

	if (h->colour_planes != 1)
	{
		LOG_ERROR("Failed to load BMP: invalid DIB header.");
		return -1;
	}
	
	return 0;
}

static void read_pixel_row(uint8_t* dest, uint8_t* src, unsigned long width, unsigned int bpp)
{
	unsigned int in_bytes_per_pix = bpp/8;
	if (bpp == 24)
	{
		// assumes BGR format
		for (unsigned long pix = 0; pix < width; pix++)
		{
			unsigned long sbyte = pix*in_bytes_per_pix;
			unsigned long dbyte = pix*IMG_BYTES_PER_PIX;
			dest[dbyte]   = src[sbyte+2];
			dest[dbyte+1] = src[sbyte+1];
			dest[dbyte+2] = src[sbyte];
			dest[dbyte+3] = 255;
		}
	}
	else if (bpp == 32)
	{
		// assumes BGRA format
		for (unsigned long pix = 0; pix < width; pix++)
		{
			unsigned long sbyte = pix*in_bytes_per_pix;
			unsigned long dbyte = pix*IMG_BYTES_PER_PIX;
			dest[dbyte]   = src[sbyte+2];
			dest[dbyte+1] = src[sbyte+1];
			dest[dbyte+2] = src[sbyte];
			dest[dbyte+3] = src[sbyte+3];
		}
	}
}

static int read_pixel_array(uint8_t** data, FILE* f, uint32_t offset, long width, long height, unsigned int bpp, bool vflip)
{
	if (bpp != 24 && bpp != 32)
	{
		LOG_ERROR("Failed to load BMP: %u bits-per-pixel unsupported. Use 24 or 32 bpp.", bpp);
		return -1;
	}

	unsigned long w = (unsigned long)(width < 0 ? -width : width);
	unsigned long h = (unsigned long)(height < 0 ? -height : height);

	vflip = height < 0 ? !vflip : vflip;

	unsigned long row_size = ((unsigned long)bpp * w + 31) / 32 * 4;
	
	uint8_t buf[row_size * h];

	fseek(f, (long int)offset, SEEK_SET);

	if (fread(buf, 1, sizeof(buf), f) != sizeof(buf))
	{
		LOG_ERROR("Failed to load BMP: failed to read pixel array.");
		return -1;
	}

	uint8_t* pixel_arr = malloc(w*h*4);

	if (pixel_arr == NULL)
	{
		LOG_ERROR("Failed to load BMP: cannot allocate heap memory.");
		return -1;
	}

	if (vflip)
	{
		for (unsigned long i = 0; i < h; i++)
		{
			read_pixel_row(pixel_arr + (h-i-1)*w*IMG_BYTES_PER_PIX, buf + i*row_size, w, bpp);
		}
	}
	else
	{
		for (unsigned long i = 0; i < h; i++)
		{
			read_pixel_row(pixel_arr + i*w*IMG_BYTES_PER_PIX, buf + i*row_size, w, bpp);
		}
	}

	*data = pixel_arr;

	return 0;
}

Image io_load_bmp(const char* path, bool vflip)
{
	Image img;
	FILE* file;
	int err;
	BMPHeader header;
	DIBHeader dib;

	img.data = NULL;
	img.width = 0;
	img.height = 0;

	file = fopen(path, "rb");

	if (file == NULL)
	{
		LOG_ERROR("Failed to open '%s'.", path);
		return img;
	}

	err = read_bmp_header(&header, file);

	if (err != 0)
	{
		return img;
	}

	if (header.magic_number[0] != 'B' || header.magic_number[1] != 'M')
	{
		LOG_ERROR("Failed to load BMP: only files marked with BM are supported.");
		return img;
	}
	
	err = read_dib_header(&dib, file);

	if (err != 0)
	{
		return img;
	}

	uint8_t* pixel_array;
	err = read_pixel_array(&pixel_array, file, header.data_offset, dib.width, dib.height, dib.bpp, vflip);

	if (err != 0)
	{
		return img;
	}
	
	unsigned long w = (unsigned long)(dib.width < 0 ? -dib.width : dib.width);
	unsigned long h = (unsigned long)(dib.height < 0 ? -dib.height : dib.height);
	
	img.data = pixel_array;
	img.width = w;
	img.height = h;
	
	fclose(file);

	return img;
}
