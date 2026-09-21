#ifndef DRAWING_H
#define DRAWING_H

#include "structs.h"
#include "raymath.h"
#include "consts.h"

void draw_tile(Tile *tile);

void draw_map(Camera2D *camera, Map *map);

void draw_player(Player *player);

#endif