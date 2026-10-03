#ifndef DRAWING_H
#define DRAWING_H

#include "structs.h"
#include "raylib/raymath.h"

void draw_world(Camera2D* camera, GameState *game);

void draw_ui(GameState *game);

#endif