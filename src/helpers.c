#include "../include/helpers.h"
#include "../include/consts.h"
#include "ctype.h"
#include "math.h"

void StrToLower(char *str) {
    for (char *p = str; *p; p++) {
        *p = tolower(*p);
    }
}

float SnapToPixel(float p) { return truncf(p * METER) / METER; }

Vector2 SnapVectorToPixel(Vector2 vec) {
    return (Vector2){SnapToPixel(vec.x), SnapToPixel(vec.y)};
}

float SnapToGrid(float num) {
    return floor(num / TILE_SIDE_LEN) * TILE_SIDE_LEN;
}

Vector2 SnapVectorToGrid(Vector2 vec) {
    return (Vector2){floor(vec.x / TILE_SIDE_LEN) * TILE_SIDE_LEN,
                     floor(vec.y / TILE_SIDE_LEN) * TILE_SIDE_LEN};
}