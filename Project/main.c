#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include "includes\Image.h"
#include "includes\LinearAlgebra.h"

/* TODO: Maybe use a LISP-like language internally or use a pseudo symbolic algebra range system to replace the current object-oriented system */
/* TODO: Add parameterized episolids (episterea) */

typedef struct Range {
    double near, far; /* NOTE: low <= high */
} Range;

typedef struct Luminosity {
    Range range; /* TODO: Make this accommodate tori when they're added */
    double angle;
} Luminosity;

typedef struct Solid Solid;
struct Solid {
    union {
        struct { Mat4 to, from; } scene_transform; /* TODO: `to` and `from` should probably be `const` */
        struct { Solid *left, *right; };
    };
    Luminosity (*luminosity)(const Solid *, Ray);
    void (*print)(Solid *); /* For debugging purposes */
};

/* TODO: Make deletion functions */

Solid *Solid_primitive_new(Vec3 pos, Dim3 dim, Rot3 rot, Luminosity (*primitive_luminosity)(const Solid *, Ray))
{
    assert(primitive_luminosity != NULL);

    Solid *ret = malloc(sizeof *ret);

    if (ret == NULL)
        return NULL;

    Mat4_make_transformation(ret->scene_transform.to, pos, dim, rot);

    memcpy(ret->scene_transform.from, ret->scene_transform.to, sizeof ret->scene_transform.from);
    Mat4_invert(ret->scene_transform.from);

    ret->luminosity = primitive_luminosity;

    return ret;
}

Solid *Solid_composite_new(Solid *left, Solid *right, Luminosity (*composite_luminosity)(const Solid *, Ray))
{
    assert(left != NULL);
    assert(right != NULL);
    assert(composite_luminosity);

    Solid *ret = malloc(sizeof *ret);

    if (ret == NULL)
        return NULL;

    ret->left = left;
    ret->right = right;
    ret->luminosity = composite_luminosity;

    return ret;
}

/* TODO: Fix the issues with rendering */
/* TODO: Make lighting distance-based */

Luminosity Solid_ellipsoid_luminosity(const Solid *this, Ray ray)
{
    assert(this != NULL);
    assert(this->scene_transform.from != NULL);
    assert(this->scene_transform.to != NULL);

    const Ray tray = Mat4_mult_Ray(this->scene_transform.from, ray);

    const double a = tray.dpos.x * tray.dpos.x + tray.dpos.y * tray.dpos.y + tray.dpos.z * tray.dpos.z;
    const double b = 2.0 * (tray.pos.x * tray.dpos.x + tray.pos.y * tray.dpos.y + tray.pos.z * tray.dpos.z);
    const double c = tray.pos.x * tray.pos.x + tray.pos.y * tray.pos.y + tray.pos.z * tray.pos.z - 1.0;

    if (b * b - 4.0 * a * c < 0.0)
        return (Luminosity) { .range = { .near = INFINITY } };

    const Range range = {
        .near = (-b - sqrt(b * b - 4.0 * a * c)) / (2.0 * a), /* TODO: Ensure that this is necessarily the closer of the roots, I think it is */
        .far = (-b + sqrt(b * b - 4.0 * a * c)) / (2.0 * a)
    };

    /* TODO: Find further restrictions --- reimplement the ones you've already made */

    const struct { Vec3 near; Vec3 far; } intersectionpoints = {
        .near = {
            .x = tray.pos.x + tray.dpos.x * range.near,
            .y = tray.pos.y + tray.dpos.y * range.near,
            .z = tray.pos.z + tray.dpos.z * range.near
        },
        .far = {
            .x = tray.pos.x + tray.dpos.x * range.far,
            .y = tray.pos.y + tray.dpos.y * range.far,
            .z = tray.pos.z + tray.dpos.z * range.far
        }
    };

    const Vec3 normal = intersectionpoints.near;

    const Vec3 tnormal = Mat4_mult_Raytip(this->scene_transform.to, normal);

    const double angle = acos(Vec3_dot(ray.dpos, tnormal) / (Vec3_mag(ray.dpos) * Vec3_mag(tnormal)));

    const Range trange = {
        .near = Vec3_mag(Mat4_mult_Vec3(this->scene_transform.to, intersectionpoints.near)),
        .far = Vec3_mag(Mat4_mult_Vec3(this->scene_transform.to, intersectionpoints.far))
    };

    return (Luminosity) { .angle = angle / M_PI, .range = trange };
}

#if 0

Luminosity Solid_cuboid_luminosity(const Solid *this, Ray ray)
{
    assert(this != NULL);
    assert(this->scene_transform.from != NULL);
    assert(this->scene_transform.to != NULL);

    const Ray tray = Mat4_mult_Ray(this->scene_transform.from, ray);

    Vec3 normal;

    double closestintersection = INFINITY;

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
        const double a = tray.pos.y + tray.dpos.y * (1.0 - tray.pos.x) / tray.dpos.x;
        const double b = tray.pos.z + tray.dpos.z * (1.0 - tray.pos.x) / tray.dpos.x;

        const double currentintersection = (1.0 - tray.pos.x) / tray.dpos.x;

        if (0.0 <= a && a <= 1.0 && 0.0 <= b && b <= 1.0 && (isinf(closestintersection) || currentintersection < closestintersection)) {
            closestintersection = currentintersection;
            normal = (Vec3) { .x = 1.0, .y = 0.0, .z = 0.0 };
        }
    }

    /*  0 <= x <= 1 && y == 0 && 0 <= z <= 1 */
    if (-tray.pos.y / tray.dpos.y >= 0.0) {
        const double a = tray.pos.x + tray.dpos.x * -tray.pos.y / tray.dpos.y;
        const double b = tray.pos.z + tray.dpos.z * -tray.pos.y / tray.dpos.y;

        const double currentintersection = -tray.pos.y / tray.dpos.y;

        if (0.0 <= a && a <= 1.0 && 0.0 <= b && b <= 1.0 && (isinf(closestintersection) || currentintersection < closestintersection)) {
            closestintersection = currentintersection;
            normal = (Vec3) { .x = 0.0, .y = -1.0, .z = 0.0 };
        }
    }

    /*  0 <= x <= 1 && y == 1 && 0 <= z <= 1 */
    if ((1.0 - tray.pos.y) / tray.dpos.y >= 0.0) {
        const double a = tray.pos.x + tray.dpos.x * (1.0 - tray.pos.y) / tray.dpos.y;
        const double b = tray.pos.z + tray.dpos.z * (1.0 - tray.pos.y) / tray.dpos.y;

        const double currentintersection = (1.0 - tray.pos.y) / tray.dpos.y;

        if (0.0 <= a && a <= 1.0 && 0.0 <= b && b <= 1.0 && (isinf(closestintersection) || currentintersection < closestintersection)) {
            closestintersection = currentintersection;
            normal = (Vec3) { .x = 0.0, .y = 1.0, .z = 0.0 };
        }
    }

    /*  0 <= x <= 1 && 0 <= y <= 1 && z == 0 */
    if (-tray.pos.z / tray.dpos.z >= 0.0) {
        const double a = tray.pos.x + tray.dpos.x * -tray.pos.z / tray.dpos.z;
        const double b = tray.pos.y + tray.dpos.y * -tray.pos.z / tray.dpos.z;

        const double currentintersection = -tray.pos.z / tray.dpos.z;

        if (0.0 <= a && a <= 1.0 && 0.0 <= b && b <= 1.0 && (isinf(closestintersection) || currentintersection < closestintersection)) {
            closestintersection = currentintersection;
            normal = (Vec3) { .x = 0.0, .y = 0.0, .z = -1.0 };
        }
    }

    /*  0 <= x <= 1 && 0 <= y <= 1 && z == 1 */
    if ((1.0 - tray.pos.z) / tray.dpos.z >= 0.0) {
        const double a = tray.pos.x + tray.dpos.x * (1.0 - tray.pos.z) / tray.dpos.z;
        const double b = tray.pos.y + tray.dpos.y * (1.0 - tray.pos.z) / tray.dpos.z;

        const double currentintersection = (1.0 - tray.pos.z) / tray.dpos.z;

        if (0.0 <= a && a <= 1.0 && 0.0 <= b && b <= 1.0 && (isinf(closestintersection) || currentintersection < closestintersection)) {
            closestintersection = currentintersection;
            normal = (Vec3) { .x = 0.0, .y = 0.0, .z = 1.0 };
        }
    }

    // compute_luminosity: /* Make the luminosity calculuation be distance-based (inverse square law) */

    if (isinf(closestintersection))
        return (Luminosity) { .distance = INFINITY };

    const Vec3 tnormal = Mat4_mult_Raytip(this->scene_transform.to, normal);

    const double angle = acos(Vec3_dot(ray.dpos, tnormal) / (Vec3_mag(ray.dpos) * Vec3_mag(tnormal)));

    const Vec3 intersectionpoint = {
        .x = tray.pos.x + tray.dpos.x * closestintersection,
        .y = tray.pos.y + tray.dpos.y * closestintersection,
        .z = tray.pos.z + tray.dpos.z * closestintersection
    };

    return (Luminosity) { .distance = Vec3_mag(Mat4_mult_Raytip(this->scene_transform.to, intersectionpoint)), .angle = angle / M_PI };
}

Luminosity Solid_cylinder_luminosity(const Solid *this, Ray ray)
{
    assert(this != NULL);

    const Ray tray = Mat4_mult_Ray(this->scene_transform.from, ray);

    Vec3 normal;

    double closestintersection = INFINITY;

    /* z == 0 && x * x + y * y <= 1 */
    if (-tray.pos.z / tray.dpos.z >= 0.0) {
        const double a = tray.pos.x + tray.dpos.x * -tray.pos.z / tray.dpos.z;
        const double b = tray.pos.y + tray.dpos.y * -tray.pos.z / tray.dpos.z;

        if (a * a + b * b <= 1.0) {
            closestintersection = -tray.pos.z / tray.dpos.z;
            normal = (Vec3) { .x = 0.0, .y = 0.0, .z = -1.0 };
        }
    }

    /* z == 1 && x * x + y * y <= 1 */
    if ((1.0 - tray.pos.z) / tray.dpos.z >= 0.0) {
        const double a = tray.pos.x + tray.dpos.x * (1.0 - tray.pos.z) / tray.dpos.z;
        const double b = tray.pos.y + tray.dpos.y * (1.0 - tray.pos.z) / tray.dpos.z;

        const double currentintersection = (1.0 - tray.pos.z) / tray.dpos.z;

        if (a * a + b * b <= 1.0 && (isinf(closestintersection) || currentintersection < closestintersection)) {
            closestintersection = currentintersection;
            normal = (Vec3) { .x = 0.0, .y = 0.0, .z = 1.0 };
        }
    }

    /* 0 < z < 1 && x * x + y * y == 1 */
    {
        const double a = tray.dpos.x * tray.dpos.x + tray.dpos.y * tray.dpos.y;
        const double b = 2.0 * (tray.pos.x * tray.dpos.x + tray.pos.y * tray.dpos.y);
        const double c = tray.pos.x * tray.pos.x + tray.pos.y * tray.pos.y - 1.0;

        if (b * b - 4.0 * a * c >= 0.0) {
            const double currentintersection = (-b - sqrt(b * b - 4.0 * a * c)) / (2.0 * a); /* TODO: Ensure that this is necessarily the closer of the roots, I think it is */

            if (currentintersection >= 0.0) {
                const double z = tray.pos.z + tray.dpos.z * currentintersection;

                if (0.0 < z && z < 1.0 && (isinf(closestintersection) || currentintersection < closestintersection)) {
                    closestintersection = currentintersection;
                    normal = (Vec3) {
                        .x = tray.pos.x + tray.dpos.x * closestintersection,
                        .y = tray.pos.y + tray.dpos.y * closestintersection,
                        .z = 0.0
                    };
                }
            }
        }
    }

    if (isinf(closestintersection))
        return (Luminosity) { .distance = INFINITY };

    const Vec3 tnormal = Mat4_mult_Raytip(this->scene_transform.to, normal);

    const double angle = acos(Vec3_dot(ray.dpos, tnormal) / (Vec3_mag(ray.dpos) * Vec3_mag(tnormal)));

    const Vec3 intersectionpoint = {
        .x = tray.pos.x + tray.dpos.x * closestintersection,
        .y = tray.pos.y + tray.dpos.y * closestintersection,
        .z = tray.pos.z + tray.dpos.z * closestintersection
    };

    return (Luminosity) { .distance = Vec3_mag(Mat4_mult_Raytip(this->scene_transform.to, intersectionpoint)), .angle = angle / M_PI };
}

/* TODO: Double-check this, I think the color is off */
Luminosity Solid_cone_luminosity(const Solid *this, Ray ray)
{
    assert(this != NULL);
    assert(this->scene_transform.from != NULL);
    assert(this->scene_transform.to != NULL);

    const Ray tray = Mat4_mult_Ray(this->scene_transform.from, ray);

    Vec3 normal;

    double closestintersection = INFINITY;

    /* z == 0 */
    if (tray.pos.z / tray.dpos.z >= 0)
        return (Luminosity) { .distance = INFINITY };

    /* z == 1 && x * x + y * y == 1 */
    if ((1.0 - tray.pos.z) / tray.dpos.z >= 0) {
        const double a = tray.pos.x + tray.dpos.x * (1.0 - tray.pos.z) / tray.dpos.z;
        const double b = tray.pos.y + tray.dpos.y * (1.0 - tray.pos.z) / tray.dpos.z;

        if (a * a + b * b <= 1.0) {
            closestintersection = (1.0 - tray.pos.z) / tray.dpos.z;
            normal = (Vec3) { .x = 0.0, .y = 0.0, .z = 1.0 };
        }
    }

    /* 0 < z < 1 && x * x + y * y == z * z */
    {
        const double a = tray.dpos.x * tray.dpos.x + tray.dpos.y * tray.dpos.y - tray.dpos.z * tray.dpos.z;
        const double b = 2.0 * (tray.pos.x * tray.dpos.x + tray.pos.y * tray.dpos.y - tray.pos.z * tray.dpos.z);
        const double c = tray.pos.x * tray.pos.x + tray.pos.y * tray.pos.y - tray.pos.z * tray.pos.z;

        if (b * b - 4.0 * a * c >= 0) {
            const double t = (-b - sqrt(b * b - 4.0 * a * c)) / (2.0 * a);

            if (t >= 0.0 && 0.0 < tray.pos.z + tray.dpos.z * t && tray.pos.z + tray.dpos.z * t < 1.0 && (isinf(closestintersection) || t < closestintersection)) {
                closestintersection = t;
                normal = (Vec3) {
                    .x = M_SQRT2 / 2.0 * (tray.pos.x + tray.dpos.x * closestintersection),
                    .y = M_SQRT2 / 2.0 * (tray.pos.y + tray.dpos.y * closestintersection),
                    .z = -M_SQRT2 / 2.0
                };
            }
        }
    }

    if (isinf(closestintersection))
        return (Luminosity) { .distance = INFINITY};

    const Vec3 tnormal = Mat4_mult_Raytip(this->scene_transform.to, normal);

    const double angle = acos(Vec3_dot(ray.dpos, tnormal) / (Vec3_mag(ray.dpos) * Vec3_mag(tnormal)));

    const Vec3 intersectionpoint = {
        .x = tray.pos.x + tray.dpos.x * closestintersection,
        .y = tray.pos.y + tray.dpos.y * closestintersection,
        .z = tray.pos.z + tray.dpos.z * closestintersection
    };

    return (Luminosity) { .distance = Vec3_mag(Mat4_mult_Raytip(this->scene_transform.to, intersectionpoint)), .angle = angle / M_PI };
}

#endif

/* TODO: Allow for whole tranformations on composite structures */
Luminosity Solid_union_luminosity(const Solid *this, Ray ray)
{
    assert(this != NULL);
    assert(this->left != NULL);
    assert(this->right != NULL);
    assert(this->left->luminosity != NULL);
    assert(this->right->luminosity != NULL);

    const Luminosity leftluminosity = this->left->luminosity(this->left, ray);
    const Luminosity rightluminosity = this->right->luminosity(this->right, ray);

    if (isinf(leftluminosity.range.near))
        return rightluminosity;

    if (isinf(rightluminosity.range.near))
        return leftluminosity;

    const double near = fmin(leftluminosity.range.near, rightluminosity.range.near);
    const double far = fmax(leftluminosity.range.far, rightluminosity.range.far);

    return (Luminosity) { .range = { .near = near, .far = far }, .angle = near == leftluminosity.range.near ? leftluminosity.angle : rightluminosity.angle };
}

#if 0
Luminosity Solid_intersection_luminosity(const Solid *this, Ray ray)
{
    assert(this != NULL);
    assert(this->left != NULL);
    assert(this->right != NULL);
    assert(this->left->luminosity != NULL);
    assert(this->right->luminosity != NULL);

    const Luminosity leftluminosity = this->left->luminosity(this->left, ray);
    const Luminosity rightluminosity = this->right->luminosity(this->right, ray);

    if (isinf(leftluminosity.range.near) || isinf(rightluminosity.range.near))
        return (Luminosity) { .range = { .near = INFINITY } };

    const double near = fmax(leftluminosity.range.near, rightluminosity.range.near);
    const double far = fmax(leftluminosity.range.far, rightluminosity.range.far);

    return (Luminosity) { .range = { .near = near, .far = far }, .angle = near == leftluminosity.range.near ? leftluminosity.angle : rightluminosity.angle };
}

Luminosity Solid_difference_luminosity(const Solid *this, Ray ray)
{
    assert(this != NULL);
    assert(this->left != NULL);
    assert(this->right != NULL);
    assert(this->left->luminosity != NULL);
    assert(this->right->luminosity != NULL);

    const Luminosity leftluminosity = this->left->luminosity(this->left, ray);
    const Luminosity rightluminosity = this->right->luminosity(this->right, ray);

    if (isinf(leftluminosity.range.near))
        return (Luminosity) { .range = { .near = INFINITY } };

    if (isinf(rightluminosity.range.near))
        return leftluminosity;

    return ;
}
#endif

static const Color backgroundcolor;

void Solid_render(const Solid *this, Image *image)
{
    assert(this != NULL);
    assert(image != NULL);
    assert(this->luminosity != NULL);

    /* TODO: Add clipping */

    /* TODO: Consolidate these into a `Camera` object */

    const double focallength = 1700.0; /* TODO: Test to find the best focal length */

    Vec3 orig = { .x = 0.0, .y = 0.0, .z = -focallength };

    for (uint32_t x = 0; x < Image_width(image); x++) {
        for (uint32_t y = 0; y < Image_height(image); y++) {
            Ray ray = {
                .pos = orig,
                .dpos = (Vec3) { .x = (double) x - Image_width(image) / 2, .y = Image_height(image) / 2 - (double) y, .z = focallength }
            };

            Luminosity luminosity = this->luminosity(this, ray);

            if (isfinite(luminosity.range.near)) {
                Image_point(image, x, y,
                    (Color) {
                        .r = ((luminosity.angle - 0.5) * 2.0 + 2.0) * backgroundcolor.r / 3.0,
                        .g = ((luminosity.angle - 0.5) * 2.0 + 2.0) * backgroundcolor.g / 3.0,
                        .b = ((luminosity.angle - 0.5) * 2.0 + 2.0) * backgroundcolor.b / 3.0
                    });
            }
        }
    }
}

static const Color backgroundcolor = { .r = 212, .g = 204, .b = 167 };

int main(void)
{
    Image *image = Image_new(1280, 720);
    {
        Image_clear(image, backgroundcolor);

        Solid *circles[2];

        circles[0] = Solid_primitive_new(
            (Vec3) { .x = 0.0, .y = -200.0, .z = 1000.0 },
            (Dim3) { .width = 100.0, .height = 100.0, .depth = 100.0 },
            (Rot3) { .alpha = 0.0, .beta = M_PI_2, .gamma = 0.0 },
            Solid_ellipsoid_luminosity);

        circles[1] = Solid_primitive_new(
            (Vec3) { .x = 150.0, .y = -200.0, .z = 1000.0 },
            (Dim3) { .width = 100.0, .height = 100.0, .depth = 100.0 },
            (Rot3) { .alpha = 0.0, .beta = M_PI_2, .gamma = 0.0 },
            Solid_ellipsoid_luminosity);

        Solid *solid = Solid_composite_new(circles[0], circles[1], Solid_union_luminosity);

        Solid_render(solid, image);

        FILE *file = fopen("test.bmp", "wb");
        {
            Image_to_BMP(image, file);
        }
        fclose(file);
    }
    Image_delete(&image);

    return 0;
}
