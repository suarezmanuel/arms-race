#ifndef PHYSICS_H
#define PHYSICS_H

#include "structs.h"
#include "raymath.h"

void generate_forces(Player *player, float dt);

void apply_forces(Player *player, float dt);

bool is_colliding(Player *player, Tile tile);

int signum(float number);

void separate_player(Player *player, Tile tile, float dt);

void solve_collisions(Player *player, Map *map, float dt);

#endif