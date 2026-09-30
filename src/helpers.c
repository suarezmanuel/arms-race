#include "../include/helpers.h"
#include "../include/consts.h"
#include "ctype.h"
#include "math.h"

void str_to_lower(char* str) {
    for (char* p = str; *p; p++) { *p = tolower(*p); }
}

float snap_to_pixel(float p) {
    return truncf(p * METER) / METER;
}

Vector2 snap_vector_to_pixel(Vector2 vec) {
    return (Vector2){snap_to_pixel(vec.x), snap_to_pixel(vec.y)};
}