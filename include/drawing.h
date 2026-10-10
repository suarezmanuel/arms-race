#ifndef DRAWING_H
#define DRAWING_H

#include "structs.h"
#include "raylib/raymath.h"

void DrawWorld(Camera2D* camera, GameState *game);

void DrawUi(GameState *game);

#endif