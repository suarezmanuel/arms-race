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

void str_to_lower(char* str);

float snap_to_pixel(float p);

float snap_to_grid(float num);

Vector2 snap_vector_to_pixel(Vector2 vec);

Vector2 snap_vector_to_grid(Vector2 vec);


#endif