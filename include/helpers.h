#ifndef HELPERS_H
#define HELPERS_H

#include "./raylib/raylib.h"

void str_to_lower(char* str);

float snap_to_pixel(float p);

Vector2 snap_vector_to_pixel(Vector2 vec);

#endif