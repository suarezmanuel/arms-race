#ifndef HANDLE_INPUT_H
#define HANDLE_INPUT_H

#include "raylib/raylib.h"
#include "raylib/raymath.h"
#include "structs.h"

GameModeType HandleInputFreeplay(Map* map, Camera2D* camera, GameModeType initialMode);
GameModeType HandleInputConsole(Map* map, Camera2D* camera, GameModeType initialMode);

void HandleInputs(Camera2D *camera, GameState *game);

#endif