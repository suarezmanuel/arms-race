#include "../include/cvars.h"

void register_cvars(GameState *game) {
    register_cvar(VECTOR2, "player.vel", &game->player.vel);
    register_cvar(VECTOR2, "player.acc", &game->player.acc);
    register_cvar(VECTOR2, "player.SF", &game->player.SF);
    register_cvar(FLOAT, "player.drag_const", &game->player.drag_const);
    register_cvar(FLOAT, "player.speed", &game->player.speed);
    register_cvar(FLOAT, "player.m", &game->player.m);
    register_cvar(VECTOR2, "player.rec.x", &game->player.rec.x);
    register_cvar(VECTOR2, "player.rec.y", &game->player.rec.y);
    register_cvar(VECTOR2, "player.rec.width", &game->player.rec.width);
    register_cvar(VECTOR2, "player.rec.height", &game->player.rec.height);
}