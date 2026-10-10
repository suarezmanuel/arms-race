#ifndef DRAWING_H
#define DRAWING_H

#include "structs.h"
#include "raylib/raymath.h"

void DrawVector(Camera2D *camera, Vector2 start, Vector2 vec, Color color, bool to_world);

void DrawWorld(Camera2D* camera, GameState *game);

void DrawUi(GameState *game);

#endif