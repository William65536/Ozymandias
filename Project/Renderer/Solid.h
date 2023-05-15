#ifndef SOLID_H
#define SOLID_H

#include "..\includes\LinearAlgebra.h"
#include "..\includes\Image.h"

typedef struct Solid Solid;
typedef struct Solid Ellipsoid, Cuboid, Cylinder, Cone, Torus;
typedef struct Solid Union, Intersection, Difference;

Ellipsoid *Ellipsoid_new(Vec3 pos, Dim3 dim, Rot3 rot);
// Cuboid *Cuboid_new(Vec3 pos, Dim3 dim, Rot3 rot);
// Cylinder *Cylinder_new(Vec3 pos, Dim3 dim, Rot3 rot);
// Cone *Cone_new(Vec3 pos, Dim3 dim, Rot3 rot);
// Torus *Torus_new(Vec3 pos, Dim3 dim, Rot3 rot);

Union *Union_new(Solid *left, Solid *right);
// Intersection *Intersection_new(const Solid *left, const Solid *right);
// Difference *Difference_new(const Solid *left, const Solid *right);

void Solid_render(const Solid *this, Image *image, Color backgroundcolor);

#endif
