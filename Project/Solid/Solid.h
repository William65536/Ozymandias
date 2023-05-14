#ifndef SOLID_H
#define SOLID_H

#include "..\includes\LinearAlgebra.h"
#include "..\includes\Image.h"

typedef struct Range {
    double near, far; /* NOTE: low <= high */
} Range;

typedef struct Luminosity {
    Range range; /* TODO: Make this accommodate tori when they're added */
    double angle;
} Luminosity;

typedef struct Solid Solid;

typedef struct Solid Ellipsoid;
typedef struct Solid Cuboid;
typedef struct Solid Cylinder;
typedef struct Solid Cone;
typedef struct Solid Torus;

typedef struct Solid Union;
typedef struct Solid Intersection;
typedef struct Solid Difference;

Ellipsoid *Ellipsoid_new(Vec3 pos, Dim3 dim, Rot3 rot);
// Cuboid *Cuboid_new(Vec3 pos, Dim3 dim, Rot3 rot);
// Cylinder *Cylinder_new(Vec3 pos, Dim3 dim, Rot3 rot);
// Cone *Cone_new(Vec3 pos, Dim3 dim, Rot3 rot);
// Torus *Torus_new(Vec3 pos, Dim3 dim, Rot3 rot);

Union *Union_new(Solid *left, Solid *right);
// Intersection *Intersection_new(Solid *left, Solid *right);
// Difference *Difference_new(Solid *left, Solid *right);

void Solid_render(const Solid *this, Image *image, Color backgroundcolor);

#endif
