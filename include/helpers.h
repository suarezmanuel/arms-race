#ifndef HELPERS_H
#define HELPERS_H

#include "./raylib/raylib.h"

#define SIGNUM(number) (((number) > 0) - ((number) < 0))

#define INSERT_TO_ARRAY(arr, count, max, type, ...) \
    do {                                            \
        if (count == max) {                         \
            return;                                 \
        }                                           \
        arr[count++] = (type){__VA_ARGS__};         \
    } while (0)

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

void StrToLower(char* str);

float SnapToPixel(float p);

float SnapToGrid(float num);

Vector2 SnapVectorToPixel(Vector2 vec);

Vector2 SnapVectorToGrid(Vector2 vec);


#endif