#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include "includes\Image.h"
#include "includes\LinearAlgebra.h"
#include "Solid\Solid.h"

static const Color backgroundcolor = { .r = 212, .g = 204, .b = 167 };

int main(void)
{
    Image *image = Image_new(1280, 720, malloc);
    {
        Image_clear(image, backgroundcolor);

        Ellipsoid *circles[2];

        circles[0] = Ellipsoid_new(
            (Vec3) { .x = 0.0, .y = -200.0, .z = 1000.0 },
            (Dim3) { .width = 100.0, .height = 100.0, .depth = 100.0 },
            (Rot3) { .alpha = 0.0, .beta = M_PI_2, .gamma = 0.0 });

        circles[1] = Ellipsoid_new(
            (Vec3) { .x = 150.0, .y = -200.0, .z = 1000.0 },
            (Dim3) { .width = 100.0, .height = 100.0, .depth = 100.0 },
            (Rot3) { .alpha = 0.0, .beta = M_PI_2, .gamma = 0.0 });

        Union *solid = Union_new(circles[0], circles[1]);

        Solid_render(solid, image, backgroundcolor);

        FILE *file = fopen("test.bmp", "wb");
        {
            Image_to_BMP(image, file);
        }
        fclose(file);
    }
    Image_delete(&image, free);

    return 0;
}
