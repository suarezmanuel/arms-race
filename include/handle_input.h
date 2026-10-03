#ifndef HANDLE_INPUT_H
#define HANDLE_INPUT_H

#include "raylib/raylib.h"
#include "raylib/raymath.h"
#include "structs.h"

GameModeType handle_input_freeplay(Map* map, Camera2D* camera, GameModeType initialMode);
GameModeType handle_input_console(Map* map, Camera2D* camera, GameModeType initialMode);

void handle_inputs(Camera2D *camera, GameState *game);

#endif