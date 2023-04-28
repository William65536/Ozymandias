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

typedef double Mat4[4][4];

void Mat4_invert(Mat4 self) /* TODO: Handle the case when a matrix is uninvertible */
{
    assert(self != NULL);
    assert(*self != NULL);
}

Ray Mat4_mult_ray(Mat4 self, Ray ray) /* TODO: Maybe `ray` should be modified to prevent needless copying */
{
    assert(self != NULL);
    assert(*self != NULL);

    Ray temp;

    return temp;
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

    Solid *ret = malloc(sizeof *ret);

    if (ret == NULL)
        return NULL;

    memcpy(ret->scene_transform.to, transformation, sizeof ret->scene_transform.to);

    memcpy(ret->scene_transform.from, ret->scene_transform.to, sizeof ret->scene_transform.from);
    Mat4_invert(ret->scene_transform.from);

    ret->isintersect = primitive_isintersect;

    return ret;
}

bool Solid_ellipsoid_isintersect(const Solid *this, Ray ray)
{
    assert(this != NULL);
    assert(this->scene_transform.from != NULL);
    assert(*this->scene_transform.from != NULL);

    Ray tray = Mat4_mult_ray(this->scene_transform.from, ray);

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

    return 0;
}
