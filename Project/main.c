/* Ozymandias */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <stdbool.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>

/* TODO: Maybe use a LISP-like language internally or use a pseudo symbolic algebra range system to replace the current object-oriented system */
/* TODO: Add parameterized episolids (episterea) */

typedef double Mat4[4][4]; /* TODO: Maybe make this a 3x4 matrix instead */

static void Mat4_invert(Mat4 self)
{
    assert(self != NULL);
    assert(*self != NULL);

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
// typedef struct Dim3 { double width, height, depth; } Dim3;
// typedef struct Vec4 { double x, y, z, w; } Vec4;

typedef struct Ray {
    Vec3 pos, dpos;
} Ray;

static Vec3 Mat4_mult_Vec3(const Mat4 self, Vec3 vec)
{
    assert(self != NULL);
    assert(*self != NULL);

    return (Vec3) {
        .x = self[0][0] * vec.x + self[0][1] * vec.y + self[0][2] * vec.z + self[0][3],
        .y = self[1][0] * vec.x + self[1][1] * vec.y + self[1][2] * vec.z + self[1][3],
        .z = self[2][0] * vec.x + self[2][1] * vec.y + self[2][2] * vec.z + self[2][3]
    };
}

static Ray Mat4_mult_Ray(const Mat4 self, Ray ray) /* TODO: Maybe `ray` should be modified to prevent needless copying */
{
    assert(self != NULL);
    assert(*self != NULL);

    return (Ray) {
        .pos = Mat4_mult_Vec3(self, ray.pos),
        .dpos = Mat4_mult_Vec3(self, ray.dpos)
    };
}

typedef struct Solid Solid;
struct Solid {
    union {
        struct { Mat4 to, from; } scene_transform;
        struct { Solid *left, *right; };
    };
    bool (*isintersect)(const Solid *, Ray);
    void (*print)(Solid *); /* For debugging purposes */
};

Solid *Solid_primitive_new(Mat4 transformation, bool (*primitive_isintersect)(const Solid *, Ray))
{
    assert(transformation != NULL);
    assert(*transformation != NULL);
    assert(primitive_isintersect != NULL);

    Solid *ret = malloc(sizeof *ret);

    if (ret == NULL)
        return NULL;

    memcpy(ret->scene_transform.to, transformation, sizeof ret->scene_transform.to);

    memcpy(ret->scene_transform.from, ret->scene_transform.to, sizeof ret->scene_transform.from);
    Mat4_invert(ret->scene_transform.from);

    ret->isintersect = primitive_isintersect;

    return ret;
}

Solid *Solid_composite_new(Solid *left, Solid *right)
{
    assert(left != NULL);
    assert(right != NULL);

    return NULL;
}

bool Solid_ellipsoid_isintersect(const Solid *this, Ray ray)
{
    assert(this != NULL);
    assert(this->scene_transform.from != NULL);
    assert(*this->scene_transform.from != NULL);

    Ray tray = Mat4_mult_Ray(this->scene_transform.from, ray);

    double b = 2 * (tray.pos.x * tray.dpos.x + tray.pos.y * tray.dpos.y + tray.pos.z * tray.dpos.z);
    b *= b;
    double a = tray.dpos.x * tray.dpos.x + tray.dpos.y * tray.dpos.y + tray.dpos.z * tray.dpos.z;
    double c = tray.pos.x * tray.pos.x + tray.pos.y * tray.pos.y + tray.pos.z * tray.pos.z;

    return b - 4 * a * c >= 0;
}

bool Solid_cuboid_isintersect(const Solid *this, Ray ray)
{
    assert(this != NULL);

    (void) ray;

    return false;
}

bool Solid_cone_isintersect(const Solid *this, Ray ray)
{
    assert(this != NULL);

    (void) ray;

    return false;
}

bool Solid_cylinder_isintersect(const Solid *this, Ray ray)
{
    assert(this != NULL);

    (void) ray;

    return false;
}

bool Solid_torus_isintersect(const Solid *this, Ray ray)
{
    assert(this != NULL);

    (void) ray;

    return false;
}

bool Solid_union_isintersect(const Solid *this, Ray ray)
{
    assert(this != NULL);
    assert(this->left != NULL);
    assert(this->right != NULL);
    assert(this->left->isintersect != NULL);
    assert(this->right->isintersect != NULL);

    return this->left->isintersect(this->left, ray) || this->right->isintersect(this->right, ray);
}

bool Solid_intersection_isintersect(const Solid *this, Ray ray)
{
    assert(this != NULL);
    assert(this->left != NULL);
    assert(this->right != NULL);
    assert(this->left->isintersect != NULL);
    assert(this->right->isintersect != NULL);

    return this->left->isintersect(this->left, ray) && this->right->isintersect(this->right, ray);
}

bool Solid_difference_isintersect(const Solid *this, Ray ray)
{
    assert(this != NULL);
    assert(this->left != NULL);
    assert(this->right != NULL);
    assert(this->left->isintersect != NULL);
    assert(this->right->isintersect != NULL);

    return this->left->isintersect(this->left, ray) && !this->right->isintersect(this->right, ray);
}

int main(void)
{
    Mat4 mat = {
        { 1.0, 0.0, 0.0, 5.0 },
        { 0.0, 1.0, 0.0, 0.0 },
        { 0.0, 0.0, 1.0, 0.0 },
        { 0.0, 0.0, 0.0, 1.0 }
    };

    Vec3 orig = { .x = 0.0, .y = 0.0, .z = -10.0 };

    Ray ray = {
        .pos = orig,
        .dpos = (Vec3) { .x = 0.0, .y = 0.0, .z = 0.0 }
    };

    Mat4_invert(mat);

    Ray tray = Mat4_mult_Ray(mat, ray);

    printf("[<%g %g %g> <%g %g %g>]\n", tray.pos.x, tray.pos.y, tray.pos.z, tray.dpos.x, tray.dpos.y, tray.dpos.z);

    printf("[ %5g %5g %5g %5g ]\n", mat[0][0], mat[0][1], mat[0][2], mat[0][3]);
    printf("[ %5g %5g %5g %5g ]\n", mat[1][0], mat[1][1], mat[1][2], mat[1][3]);
    printf("[ %5g %5g %5g %5g ]\n", mat[2][0], mat[2][1], mat[2][2], mat[2][3]);
    printf("[ %5g %5g %5g %5g ]\n", mat[3][0], mat[3][1], mat[3][2], mat[3][3]);


    return 0;
}
