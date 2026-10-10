#ifndef CONSTS_H
#define CONSTS_H

#include "raylib/raylib.h"

#define MAX_MAP_SIZE 1024
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
#define TILE_SIDE_LEN 1.0 // 1m
#define METER 50.0f       // 50px = 1m
#define PIXEL 1 / METER
#define MAX_MAP_TILE_WIDTH 100
#define MAX_MAP_TILE_HEIGHT 100
#define BOUNDS_COLOR BROWN
#define MAX_CMD_LEN 257
#define VAR_BUF_LEN 64
#define MAX_NAME_LEN 32
#define DRAG_CONST 4
#define SPEED 70
#define SLOP 1 / (100 * METER)
#define INSTANT 0.001f
#endif
