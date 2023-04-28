/* Ozymandias */

#include <stdio.h>
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

// typedef struct AnglePair { double theta, phi; } AnglePair;
// typedef struct Vec2 { double x, y; } Vec2;
// typedef struct Dim2 { double width, height; } Dim2;
typedef struct Vec3 { double x, y, z; } Vec3;
// typedef struct Dim3 { double width, height, depth; } Dim3;
// typedef struct Vec4 { double x, y, z, w; } Vec4;

typedef struct Ray {
    Vec3 pos, dpos;
} Ray;

enum Type {
    TYPE_ELLIPSOID,
    TYPE_CUBOID,
    TYPE_CONE,
    TYPE_CYLINDER,
    TYPE_TORUS,
    TYPE_UNION,
    TYPE_INTERSECTION,
    TYPE_DIFFERENCE
};

typedef struct Solid Solid;
struct Solid {
    union {
        struct { Mat4 to, from; } scene_transform;
        struct { Solid *left, *right; };
    };
    bool (*isintersect)(const Solid *, Ray);
    void (*print)(Solid *); /* For debugging purposes */
};

bool Ellipsoid_isintersect(const Solid *this, Ray ray)
{
    assert(this != NULL);

    (void) ray;

    return false;
}

bool Cuboid_isintersect(const Solid *this, Ray ray)
{
    assert(this != NULL);

    (void) ray;

    return false;
}

bool Cone_isintersect(const Solid *this, Ray ray)
{
    assert(this != NULL);

    (void) ray;

    return false;
}

bool Cylinder_isintersect(const Solid *this, Ray ray)
{
    assert(this != NULL);

    (void) ray;

    return false;
}

bool Torus_isintersect(const Solid *this, Ray ray)
{
    assert(this != NULL);

    (void) ray;

    return false;
}

Solid *Solid_primitive_new(Mat4 transformation, enum Type type)
{
    assert(transformation != NULL);
    assert(*transformation != NULL);

    Solid *ret = malloc(sizeof *ret);

    if (ret == NULL)
        return NULL;

    memcpy(ret->scene_transform.to, transformation, sizeof ret->scene_transform.to);

    memcpy(ret->scene_transform.from, ret->scene_transform.to, sizeof ret->scene_transform.from);
    Mat4_invert(ret->scene_transform.from);

    switch (type) {
        case TYPE_ELLIPSOID:
            ret->isintersect = Ellipsoid_isintersect;
            break;
        case TYPE_CUBOID:
            ret->isintersect = Cuboid_isintersect;
            break;
        case TYPE_CONE:
            ret->isintersect = Cone_isintersect;
            break;
        case TYPE_CYLINDER:
            ret->isintersect = Cylinder_isintersect;
            break;
        case TYPE_TORUS:
            ret->isintersect = Torus_isintersect;
            break;
        default:
            assert(0 && "Unreachable");
    }

    return ret;
}

bool Union_isintersect(const Solid *this, Ray ray)
{
    assert(this != NULL);
    assert(this->left != NULL);
    assert(this->right != NULL);
    assert(this->left->isintersect != NULL);
    assert(this->right->isintersect != NULL);

    return this->left->isintersect(this->left, ray) || this->right->isintersect(this->right, ray);
}

bool Intersection_isintersect(const Solid *this, Ray ray)
{
    assert(this != NULL);
    assert(this->left != NULL);
    assert(this->right != NULL);
    assert(this->left->isintersect != NULL);
    assert(this->right->isintersect != NULL);

    return this->left->isintersect(this->left, ray) && this->right->isintersect(this->right, ray);
}

bool Difference_isintersect(const Solid *this, Ray ray)
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
