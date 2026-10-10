#ifndef PHYSICS_H
#define PHYSICS_H

#include "structs.h"

void GenerateForces(Player *player);

void ApplyForces(Player *player, Map* map);

#endif