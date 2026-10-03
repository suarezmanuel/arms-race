#include "../include/helpers.h"
#include "../include/consts.h"
#include "ctype.h"
#include "math.h"

void str_to_lower(char *str) {
    for (char *p = str; *p; p++) {
        *p = tolower(*p);
    }
}

float snap_to_pixel(float p) { return truncf(p * METER) / METER; }

Vector2 snap_vector_to_pixel(Vector2 vec) {
    return (Vector2){snap_to_pixel(vec.x), snap_to_pixel(vec.y)};
}

float snap_to_grid(float num) {
    return floor(num / TILE_SIDE_LEN) * TILE_SIDE_LEN;
}

Vector2 snap_vector_to_grid(Vector2 vec) {
    return (Vector2){floor(vec.x / TILE_SIDE_LEN) * TILE_SIDE_LEN,
                     floor(vec.y / TILE_SIDE_LEN) * TILE_SIDE_LEN};
}