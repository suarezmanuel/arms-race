#ifndef PHYSICS_H
#define PHYSICS_H

#include "structs.h"

void generate_forces(Player *player);

void apply_forces(Player *player, Map* map);

#endif