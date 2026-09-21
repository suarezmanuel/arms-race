#include "../include/physics.h"
#include "stdio.h"

void generate_forces(Player *player, float dt) {
    float clamped_drag = fmin(player->drag_const, player->m / dt);
    Vector2 f_drag = Vector2Scale(player->vel, -clamped_drag);
    player->SF = Vector2Add(player->SF, f_drag);
}

void apply_forces(Player *player, float dt) {
    player->acc = Vector2Scale(player->SF, 1 / player->m);
    player->vel = Vector2Add(player->vel, Vector2Scale(player->acc, dt));
    player->pos = Vector2Add(player->pos, Vector2Scale(player->vel, dt));
}

bool is_colliding(Player *player, Tile tile) {
    // separating axis theorem for rectangles
    return fmin(player->pos.x + player->size.x, tile.pos.x + tile.size.x) >
               fmax(player->pos.x, tile.pos.x) &&
           fmin(player->pos.y + player->size.y, tile.pos.y + tile.size.y) >
               fmax(player->pos.y, tile.pos.y);
}

int signum(float number) { return (number > 0) - (number < 0); }

void separate_player(Player *player, Tile tile, float dt) {
    Vector2 v_signum = (Vector2){signum(player->vel.x), signum(player->vel.y)};

    float minx = fmin(player->pos.x + player->size.x, tile.pos.x + tile.size.x);
    float maxx = fmax(player->pos.x, tile.pos.x);
    float miny = fmin(player->pos.y + player->size.y, tile.pos.y + tile.size.y);
    float maxy = fmax(player->pos.y, tile.pos.y);

    Vector2 dpos;
    if (minx - maxx < miny - maxy) {
        dpos = (Vector2){(minx - maxx) * -v_signum.x, 0};
        player->acc.x = 0;
        player->vel.x = 0;
    } else {
        dpos = (Vector2){0, (miny - maxy) * -v_signum.y};
        player->acc.y = 0;
        player->vel.y = 0;
    }

    player->pos = Vector2Add(player->pos, dpos);
}

void solve_collisions(Player *player, Map *map, float dt) {
    for (int i = 0; i < map->size; i++) {
        Tile tile = map->tiles[i];
        if (is_colliding(player, tile)) {
            separate_player(player, tile, dt);
        }
    }
}
