#ifndef CAMERA_H
#define CAMERA_H

#include "../include/structs.h"

void focus_on_map_tiling(Camera2D *camera);

void focus_on_player(Camera2D *camera, Vector2 target);

void focus_camera(Camera2D *camera, GameModeType mode, Player *player);

#endif