/* Ozymandias */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <stdbool.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>

#include "includes\Image.h"

/* TODO: Maybe use a LISP-like language internally or use a pseudo symbolic algebra range system to replace the current object-oriented system */
/* TODO: Add parameterized episolids (episterea) */

typedef double Mat4[4][4]; /* TODO: Maybe make this a 3x4 matrix instead */

static void Mat4_invert(Mat4 self)
{
    assert(self != NULL);

    const double denom =
        self[0][0] *
            (self[1][1] * (self[2][2] * self[3][3] - self[2][3] * self[3][2]) 
           + self[1][2] * (self[2][3] * self[3][1] - self[2][1] * self[3][3])
           + self[1][3] * (self[2][1] * self[3][2] - self[2][2] * self[3][1]))
      + self[0][1] *
            (self[1][0] * (self[2][3] * self[3][2] - self[2][2] * self[3][3])
           + self[1][2] * (self[2][0] * self[3][3] - self[2][3] * self[3][0])
           + self[1][3] * (self[2][2] * self[3][0] - self[2][0] * self[3][2]))
      + self[0][2] *
            (self[1][0] * (self[2][1] * self[3][3] - self[2][3] * self[3][1])
           + self[1][1] * (self[2][3] * self[3][0] - self[2][0] * self[3][3])
           + self[1][3] * (self[2][0] * self[3][1] - self[2][1] * self[3][0]))
      + self[0][3] *
            (self[1][0] * (self[2][2] * self[3][1] - self[2][1] * self[3][2])
           + self[1][1] * (self[2][0] * self[3][2] - self[2][2] * self[3][0])
           + self[1][2] * (self[2][1] * self[3][0] - self[2][0] * self[3][1]));

    assert(denom != 0.0);

    const Mat4 temp = {
        {
            (self[1][1] * (self[2][2] * self[3][3] - self[2][3] * self[3][2]) + self[1][2] * (self[2][3] * self[3][1] - self[2][1] * self[3][3]) + self[1][3] * (self[2][1] * self[3][2] - self[2][2] * self[3][1])) / denom,
            (self[0][1] * (self[2][3] * self[3][2] - self[2][2] * self[3][3]) + self[0][2] * (self[2][1] * self[3][3] - self[2][3] * self[3][1]) + self[0][3] * (self[2][2] * self[3][1] - self[2][1] * self[3][2])) / denom,
            (self[0][1] * (self[1][2] * self[3][3] - self[1][3] * self[3][2]) + self[0][2] * (self[1][3] * self[3][1] - self[1][1] * self[3][3]) + self[0][3] * (self[1][1] * self[3][2] - self[1][2] * self[3][1])) / denom,
            (self[0][1] * (self[1][3] * self[2][2] - self[1][2] * self[2][3]) + self[0][2] * (self[1][1] * self[2][3] - self[1][3] * self[2][1]) + self[0][3] * (self[1][2] * self[2][1] - self[1][1] * self[2][2])) / denom
        },
        {
            (self[1][0] * (self[2][3] * self[3][2] - self[2][2] * self[3][3]) + self[1][2] * (self[2][0] * self[3][3] - self[2][3] * self[3][0]) + self[1][3] * (self[2][2] * self[3][0] - self[2][0] * self[3][2])) / denom,
            (self[0][0] * (self[2][2] * self[3][3] - self[2][3] * self[3][2]) + self[0][2] * (self[2][3] * self[3][0] - self[2][0] * self[3][3]) + self[0][3] * (self[2][0] * self[3][2] - self[2][2] * self[3][0])) / denom,
            (self[0][0] * (self[1][3] * self[3][2] - self[1][2] * self[3][3]) + self[0][2] * (self[1][0] * self[3][3] - self[1][3] * self[3][0]) + self[0][3] * (self[1][2] * self[3][0] - self[1][0] * self[3][2])) / denom,
            (self[0][0] * (self[1][2] * self[2][3] - self[1][3] * self[2][2]) + self[0][2] * (self[1][3] * self[2][0] - self[1][0] * self[2][3]) + self[0][3] * (self[1][0] * self[2][2] - self[1][2] * self[2][0])) / denom
        },
        {
            (self[1][0] * (self[2][1] * self[3][3] - self[2][3] * self[3][1]) + self[1][1] * (self[2][3] * self[3][0] - self[2][0] * self[3][3]) + self[1][3] * (self[2][0] * self[3][1] - self[2][1] * self[3][0])) / denom,
            (self[0][0] * (self[2][3] * self[3][1] - self[2][1] * self[3][3]) + self[0][1] * (self[2][0] * self[3][3] - self[2][3] * self[3][0]) + self[0][3] * (self[2][1] * self[3][0] - self[2][0] * self[3][1])) / denom,
            (self[0][0] * (self[1][1] * self[3][3] - self[1][3] * self[3][1]) + self[0][1] * (self[1][3] * self[3][0] - self[1][0] * self[3][3]) + self[0][3] * (self[1][0] * self[3][1] - self[1][1] * self[3][0])) / denom,
            (self[0][0] * (self[1][3] * self[2][1] - self[1][1] * self[2][3]) + self[0][1] * (self[1][0] * self[2][3] - self[1][3] * self[2][0]) + self[0][3] * (self[1][1] * self[2][0] - self[1][0] * self[2][1])) / denom
        },
        {
            (self[1][0] * (self[2][2] * self[3][1] - self[2][1] * self[3][2]) + self[1][1] * (self[2][0] * self[3][2] - self[2][2] * self[3][0]) + self[1][2] * (self[2][1] * self[3][0] - self[2][0] * self[3][1])) / denom,
            (self[0][0] * (self[2][1] * self[3][2] - self[2][2] * self[3][1]) + self[0][1] * (self[2][2] * self[3][0] - self[2][0] * self[3][2]) + self[0][2] * (self[2][0] * self[3][1] - self[2][1] * self[3][0])) / denom,
            (self[0][0] * (self[1][2] * self[3][1] - self[1][1] * self[3][2]) + self[0][1] * (self[1][0] * self[3][2] - self[1][2] * self[3][0]) + self[0][2] * (self[1][1] * self[3][0] - self[1][0] * self[3][1])) / denom,
            (self[0][0] * (self[1][1] * self[2][2] - self[1][2] * self[2][1]) + self[0][1] * (self[1][2] * self[2][0] - self[1][0] * self[2][2]) + self[0][2] * (self[1][0] * self[2][1] - self[1][1] * self[2][0])) / denom
        }
    };

    memcpy(self, temp, sizeof temp);
}

// typedef struct AnglePair { double theta, phi; } AnglePair;
// typedef struct Vec2 { double x, y; } Vec2;
// typedef struct Dim2 { double width, height; } Dim2;
typedef struct Vec3 { double x, y, z; } Vec3;
typedef struct Dim3 { double width, height, depth; } Dim3;
// typedef struct Vec4 { double x, y, z, w; } Vec4;

typedef struct Ray {
    Vec3 pos, dpos;
} Ray;

static Vec3 Mat4_mult_Vec3(const Mat4 self, Vec3 vec)
{
    assert(self != NULL);

    return (Vec3) {
        .x = self[0][0] * vec.x + self[0][1] * vec.y + self[0][2] * vec.z + self[0][3],
        .y = self[1][0] * vec.x + self[1][1] * vec.y + self[1][2] * vec.z + self[1][3],
        .z = self[2][0] * vec.x + self[2][1] * vec.y + self[2][2] * vec.z + self[2][3]
    };
}

static Ray Mat4_mult_Ray(const Mat4 self, Ray ray) /* TODO: Maybe `ray` should be modified to prevent needless copying */
{
    assert(self != NULL);

    const Mat4 temp = {
        { self[0][0], self[0][1], self[0][2], 0.0 },
        { self[1][0], self[1][1], self[1][2], 0.0 },
        { self[2][0], self[2][1], self[2][2], 0.0 },
        {        0.0,        0.0,        0.0, 1.0 }
    };

    return (Ray) {
        .pos = Mat4_mult_Vec3(self, ray.pos),
        .dpos = Mat4_mult_Vec3(temp, ray.dpos)
    };
}

/* Add rotations and the like */
/* static */ void Mat4_make_transformation(Mat4 self, Vec3 pos, Dim3 dim)
{
    assert(self != NULL);

    self[0][0] = dim.width;
    self[0][1] = 0.0;
    self[0][2] = 0.0;
    self[0][3] = pos.x;

    self[1][0] = 0.0;
    self[1][1] = dim.height;
    self[1][2] = 0.0;
    self[1][3] = pos.y;

    self[2][0] = 0.0;
    self[2][1] = 0.0;
    self[2][2] = dim.depth;
    self[2][3] = pos.z;

    self[3][0] = 0.0;
    self[3][1] = 0.0;
    self[3][2] = 0.0;
    self[3][3] = 1.0;
}

double Vec3_mag(Vec3 self)
{
    return sqrt(self.x * self.x + self.y * self.y + self.z * self.z);
}

double Vec3_dot(Vec3 self, Vec3 vec)
{
    return self.x * vec.x + self.y * vec.y + self.z * vec.z;
}

Vec3 Vec3_cross(Vec3 self, Vec3 vec)
{
    return (Vec3) {
        .x = self.y * vec.z - self.z * vec.y,
        .y = self.z * vec.x - self.x * vec.z,
        .z = self.x * vec.y - self.y * vec.x
    };
}

/*For debugging purposes */
void Mat4_println(const Mat4 self)
{
    assert(self != NULL);

    printf("[ %10g %10g %10g %10g ]\n", self[0][0], self[0][1], self[0][2], self[0][3]);
    printf("[ %10g %10g %10g %10g ]\n", self[1][0], self[1][1], self[1][2], self[1][3]);
    printf("[ %10g %10g %10g %10g ]\n", self[2][0], self[2][1], self[2][2], self[2][3]);
    printf("[ %10g %10g %10g %10g ]\n", self[3][0], self[3][1], self[3][2], self[3][3]);

    puts("");
}

/*For debugging purposes */
void Ray_println(Ray self)
{
    printf("[ <%g %g %g> <%g %g %g> ]\n", self.pos.x, self.pos.y, self.pos.z, self.dpos.x, self.dpos.y, self.dpos.z);
}

typedef struct Solid Solid;
struct Solid {
    union {
        struct { Mat4 to, from; } scene_transform; /* TODO: `to` and `from` should probably be `const` */
        struct { Solid *left, *right; };
    };
    double (*isintersect)(const Solid *, Ray);
    void (*print)(Solid *); /* For debugging purposes */
};

/* TODO: Make deletion functions */

Solid *Solid_primitive_new(Mat4 transformation, double (*primitive_isintersect)(const Solid *, Ray))
{
    assert(transformation != NULL);
    assert(primitive_isintersect != NULL);

    Solid *ret = malloc(sizeof *ret);

    if (ret == NULL)
        return NULL;

    memcpy(ret->scene_transform.to, transformation, sizeof ret->scene_transform.to);

    // Mat4_println(ret->scene_transform.to);

    memcpy(ret->scene_transform.from, ret->scene_transform.to, sizeof ret->scene_transform.from);
    Mat4_invert(ret->scene_transform.from);

    // Mat4_println(ret->scene_transform.from);

    ret->isintersect = primitive_isintersect;

    return ret;
}

Solid *Solid_composite_new(Solid *left, Solid *right)
{
    assert(left != NULL);
    assert(right != NULL);

    return NULL;
}

double Solid_ellipsoid_isintersect(const Solid *this, Ray ray)
{
    assert(this != NULL);
    assert(this->scene_transform.from != NULL);
    assert(this->scene_transform.to != NULL);

    const Ray tray = Mat4_mult_Ray(this->scene_transform.from, ray);

    const double a = tray.dpos.x * tray.dpos.x + tray.dpos.y * tray.dpos.y + tray.dpos.z * tray.dpos.z;
    const double b = 2 * (tray.pos.x * tray.dpos.x + tray.pos.y * tray.dpos.y + tray.pos.z * tray.dpos.z);
    const double c = tray.pos.x * tray.pos.x + tray.pos.y * tray.pos.y + tray.pos.z * tray.pos.z - 1.0;

    if (b * b - 4 * a * c < 0)
        return NAN;

    const double closestintersection = (-b - sqrt(b * b - 4 * a * c)) / (2.0 * a); /* TODO: Ensure that this is necessarily the closer of the roots, I think it is */

    const Vec3 intersectionpoint = {
        .x = tray.pos.x + tray.dpos.x * closestintersection,
        .y = tray.pos.y + tray.dpos.y * closestintersection,
        .z = tray.pos.z + tray.dpos.z * closestintersection
    };

    const Vec3 normal = intersectionpoint;

    const Vec3 tnormal = Mat4_mult_Vec3(this->scene_transform.to, normal);

    const double angle = acos(Vec3_dot(ray.dpos, tnormal) / (Vec3_mag(ray.dpos) * Vec3_mag(tnormal)));

    return angle / M_PI * 255.0;
}

double Solid_cuboid_isintersect(const Solid *this, Ray ray)
{
    assert(this != NULL);
    assert(this->scene_transform.from != NULL);
    assert(this->scene_transform.to != NULL);

    const Ray tray = Mat4_mult_Ray(this->scene_transform.from, ray);

    Vec3 normal;

    double closestintersection = NAN;

    /* x == 0 && 0 <= y <= 1 && 0 <= z <= 1 */
    if (-tray.pos.x / tray.dpos.x >= 0.0) {
        const double a = tray.pos.y + tray.dpos.y * -tray.pos.x / tray.dpos.x;
        const double b = tray.pos.z + tray.dpos.z * -tray.pos.x / tray.dpos.x;

        if (0.0 <= a && a <= 1.0 && 0.0 <= b && b <= 1.0) {
            closestintersection = -tray.pos.x / tray.dpos.x;
            normal = (Vec3) { .x = -1.0, .y = 0.0, .z = 0.0 };
        }
    }

    /* x == 1 && 0 <= y <= 1 && 0 <= z <= 1 */
    if ((1.0 - tray.pos.x) / tray.dpos.x >= 0.0) {
        const double a = tray.pos.y + tray.dpos.y * (1 - tray.pos.x) / tray.dpos.x;
        const double b = tray.pos.z + tray.dpos.z * (1 - tray.pos.x) / tray.dpos.x;

        if (0.0 <= a && a <= 1.0 && 0.0 <= b && b <= 1.0) {
            const double currentintersection = (1 - tray.pos.x) / tray.dpos.x;

            if (!isnan(closestintersection)) {
                if (currentintersection < closestintersection) {
                    closestintersection = currentintersection;
                    normal = (Vec3) { .x = 1.0, .y = 0.0, .z = 0.0 };
                }

                goto compute_luminosity;
            }

            closestintersection = currentintersection;
            normal = (Vec3) { .x = 1.0, .y = 0.0, .z = 0.0 };
        }
    }

    /*  0 <= x <= 1 && y == 0 && 0 <= z <= 1 */
    if (-tray.pos.y / tray.dpos.y >= 0.0) {
        const double a = tray.pos.x + tray.dpos.x * -tray.pos.y / tray.dpos.y;
        const double b = tray.pos.z + tray.dpos.z * -tray.pos.y / tray.dpos.y;

        if (0.0 <= a && a <= 1.0 && 0.0 <= b && b <= 1.0) {
            const double currentintersection = -tray.pos.y / tray.dpos.y;

            if (!isnan(closestintersection)) {
                if (currentintersection < closestintersection) {
                    closestintersection = currentintersection;
                    normal = (Vec3) { .x = 0.0, .y = -1.0, .z = 0.0 };
                }

                goto compute_luminosity;
            }

            closestintersection = currentintersection;
            normal = (Vec3) { .x = 0.0, .y = -1.0, .z = 0.0 };
        }
    }

    /*  0 <= x <= 1 && y == 1 && 0 <= z <= 1 */
    if ((1.0 - tray.pos.y) / tray.dpos.y >= 0.0) {
        const double a = tray.pos.x + tray.dpos.x * (1 - tray.pos.y) / tray.dpos.y;
        const double b = tray.pos.z + tray.dpos.z * (1 - tray.pos.y) / tray.dpos.y;

        if (0.0 <= a && a <= 1.0 && 0.0 <= b && b <= 1.0) {
            const double currentintersection = (1 - tray.pos.y) / tray.dpos.y;

            if (!isnan(closestintersection)) {
                if (currentintersection < closestintersection) {
                    closestintersection = currentintersection;
                    normal = (Vec3) { .x = 0.0, .y = 1.0, .z = 0.0 };
                }

                goto compute_luminosity;
            }

            closestintersection = currentintersection;
            normal = (Vec3) { .x = 0.0, .y = 1.0, .z = 0.0 };
        }
    }

    /*  0 <= x <= 1 && 0 <= y <= 1 && z == 0 */
    if (-tray.pos.z / tray.dpos.z >= 0.0) {
        const double a = tray.pos.x + tray.dpos.x * -tray.pos.z / tray.dpos.z;
        const double b = tray.pos.y + tray.dpos.y * -tray.pos.z / tray.dpos.z;

        if (0.0 <= a && a <= 1.0 && 0.0 <= b && b <= 1.0) {
            const double currentintersection = -tray.pos.z / tray.dpos.z;

            if (!isnan(closestintersection)) {
                if (currentintersection < closestintersection) {
                    closestintersection = currentintersection;
                    normal = (Vec3) { .x = 0.0, .y = 1.0, .z = -1.0 };
                }

                goto compute_luminosity;
            }

            closestintersection = currentintersection;
            normal = (Vec3) { .x = 0.0, .y = 1.0, .z = -1.0 };
        }
    }

    /*  0 <= x <= 1 && 0 <= y <= 1 && z == 1 */
    if ((1.0 - tray.pos.z) / tray.dpos.z >= 0.0) {
        const double a = tray.pos.x + tray.dpos.x * (1 - tray.pos.z) / tray.dpos.z;
        const double b = tray.pos.y + tray.dpos.y * (1 - tray.pos.z) / tray.dpos.z;

        if (0.0 <= a && a <= 1.0 && 0.0 <= b && b <= 1.0) {
            const double currentintersection = (1 - tray.pos.z) / tray.dpos.z;

            if (!isnan(closestintersection)) {
                if (currentintersection < closestintersection) {
                    closestintersection = currentintersection;
                    normal = (Vec3) { .x = 0.0, .y = 0.0, .z = 1.0 };
                }

                goto compute_luminosity;
            }

            closestintersection = currentintersection;
            normal = (Vec3) { .x = 0.0, .y = 0.0, .z = 1.0 };
        }
    }

    compute_luminosity: /* Make the luminosity calculuation be distance-based (inverse square law) */

    if (isnan(closestintersection))
        return NAN;

    const Vec3 tnormal = Mat4_mult_Vec3(this->scene_transform.to, normal);

    const double angle = acos(Vec3_dot(ray.dpos, tnormal) / (Vec3_mag(ray.dpos) * Vec3_mag(tnormal)));

    return angle / M_PI * 255.0;
}

double Solid_cone_isintersect(const Solid *this, Ray ray)
{
    assert(this != NULL);

    (void) ray;

    return false;
}

double Solid_cylinder_isintersect(const Solid *this, Ray ray)
{
    assert(this != NULL);

    (void) ray;

    return false;
}

double Solid_torus_isintersect(const Solid *this, Ray ray)
{
    assert(this != NULL);

    (void) ray;

    return false;
}

double Solid_union_isintersect(const Solid *this, Ray ray)
{
    assert(this != NULL);
    assert(this->left != NULL);
    assert(this->right != NULL);
    assert(this->left->isintersect != NULL);
    assert(this->right->isintersect != NULL);

    return this->left->isintersect(this->left, ray) || this->right->isintersect(this->right, ray);
}

double Solid_intersection_isintersect(const Solid *this, Ray ray)
{
    assert(this != NULL);
    assert(this->left != NULL);
    assert(this->right != NULL);
    assert(this->left->isintersect != NULL);
    assert(this->right->isintersect != NULL);

    return this->left->isintersect(this->left, ray) && this->right->isintersect(this->right, ray);
}

double Solid_difference_isintersect(const Solid *this, Ray ray)
{
    assert(this != NULL);
    assert(this->left != NULL);
    assert(this->right != NULL);
    assert(this->left->isintersect != NULL);
    assert(this->right->isintersect != NULL);

    return this->left->isintersect(this->left, ray) && !this->right->isintersect(this->right, ray);
}

void Solid_render(const Solid *this, Image *image)
{
    assert(this != NULL);
    assert(image != NULL);
    assert(this->isintersect != NULL);

    /* TODO: Consolidate these into a `Camera` object */

    const double focallength = 1000.0; /* TODO: Test to find the best focal length */

    Vec3 orig = { .x = 0.0, .y = 0.0, .z = -focallength };

    for (uint32_t x = 0; x < Image_width(image); x++) {
        for (uint32_t y = 0; y < Image_height(image); y++) {
            Ray ray = {
                .pos = orig,
                .dpos = (Vec3) { .x = (double) x - Image_width(image) / 2, .y = Image_height(image) / 2 - (double) y, .z = focallength }
            };

            double luminosity = this->isintersect(this, ray);

            if (!isnan(luminosity))
                Image_point(image, x, y, (Color) { .r = luminosity, .g = luminosity, .b = luminosity });
        }
    }
}

int main(void)
{
    Image *image = Image_new(1280, 720);

    Image_set_background(image, (Color) { .r = 0x80, .g = 0x00, .b = 0x40 });

    Mat4 mat;
    Mat4_make_transformation(mat,
        (Vec3) { .x = -200.0, .y = 100.0, .z = 200.0 },
        (Dim3) { .width = 100.0, .height = 200.0, .depth = 200.0 });

    Solid *primitive = Solid_primitive_new(mat, Solid_cuboid_isintersect);

    Solid_render(primitive, image);

    FILE *file = fopen("test.bmp", "wb");

    Image_to_BMP(image, file);

    fclose(file);

    Image_delete(&image);

    return 0;
}
