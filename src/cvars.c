#include "../include/cvars.h"

void RegisterCvars(GameState *game) {
    RegisterCvar(VECTOR2, "player.vel", &game->player.vel);
    RegisterCvar(VECTOR2, "player.acc", &game->player.acc);
    RegisterCvar(VECTOR2, "player.SF", &game->player.SF);
    RegisterCvar(FLOAT, "player.drag_const", &game->player.drag_const);
    RegisterCvar(FLOAT, "player.speed", &game->player.speed);
    RegisterCvar(FLOAT, "player.m", &game->player.m);
    RegisterCvar(VECTOR2, "player.rec.x", &game->player.rec.x);
    RegisterCvar(VECTOR2, "player.rec.y", &game->player.rec.y);
    RegisterCvar(VECTOR2, "player.rec.width", &game->player.rec.width);
    RegisterCvar(VECTOR2, "player.rec.height", &game->player.rec.height);
}