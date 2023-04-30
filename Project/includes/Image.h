#ifndef PICTURE_H
#define PICTURE_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct Color {
    uint8_t r, g, b;
} Color;

typedef struct Image Image;

Image *Image_new(uint32_t width, uint32_t height);

void Image_delete(Image **self);

uint32_t Image_width(const Image *image);

uint32_t Image_height(const Image *image);

Color Image_color(const Image *self, uint32_t x, uint32_t y);

void Image_point(Image *self, uint32_t x, uint32_t y, Color color);

void Image_clear(Image *self, Color color);

bool Image_to_BMP(const Image *self, FILE *file);

#endif
