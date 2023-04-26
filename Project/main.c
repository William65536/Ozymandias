/* Ozymandias */

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdbool.h>
#define _USE_MATH_DEFINES
#include <math.h>

/* TODO: Maybe use a LISP-like language internally or use a pseudo symbolic algebra range system to replace the current object-oriented system */
/* TODO: Add parameterized episolids (episterea) */

typedef double Mat4[4][4];

typedef struct AnglePair { double theta, phi; } AnglePair;
// typedef struct Vec2 { double x, y; } Vec2;
// typedef struct Dim2 { double width, height; } Dim2;
typedef struct Vec3 { double x, y, z; } Vec3;
typedef struct Dim3 { double width, height, depth; } Dim3;
// typedef struct Vec4 { double x, y, z, w; } Vec4;

typedef struct Ray {
    int temp;
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

Solid *Ellipsoid_new(Vec3 position, AnglePair orientation, Dim3 dimensions)
{
    Solid *ret = malloc(sizeof *ret);

    if (ret == NULL)
        return NULL;

    *ret = (Solid) {};

    return ret;
}

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

