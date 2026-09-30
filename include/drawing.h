#ifndef DRAWING_H
#define DRAWING_H

#include "structs.h"
#include "raylib/raymath.h"

void draw_tile(Tile *tile);

void draw_map(Camera2D *camera, Map *map);

void draw_grid(Camera2D *camera);

void draw_player(Player *player);

void draw_debug_info_player(Player *player);

#endif