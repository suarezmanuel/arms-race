#include "../include/physics.h"
#include "../include/geometry.h"
#include "../include/raylib/raymath.h"
#include "../include/helpers.h"

bool IsColliding(Rectangle rec1, Rectangle rec2) {
    // separating axis theorem for rectangles
    return fmin(REC_RIGHT(rec1), REC_RIGHT(rec2)) -
                   fmax(REC_LEFT(rec1), REC_LEFT(rec2)) >
               SLOP &&
           fmin(REC_BOTTOM(rec1), REC_BOTTOM(rec2)) -
                   fmax(REC_TOP(rec1), REC_TOP(rec2)) >
               SLOP;
}

void MovePlayer(Player *player, Map *map) {
    Vector2 dp = Vector2Scale(player->vel, GetFrameTime());
    Rectangle collider;
    bool collided;
    float dx = 0.0f;
    float dy = 0.0f;
    // emualte movement, if only one axis was triggered, but the movement was in two directions. check in the non-moved one if there are blocks in the v direction that we can squish through

    player->rec.x += dp.x;
    for (int i = 0; i < map->size; i++) {
        Tile tile = map->tiles[i];
            
        float minx = fmin(REC_RIGHT(player->rec), REC_RIGHT(tile.rec));
        float maxx = fmax(REC_LEFT(player->rec), REC_LEFT(tile.rec));

        if (IsColliding(player->rec, tile.rec)) {
            // collider = tile.rec;
            int push_dir = SIGNUM(REC_CENTER_X(player->rec) - REC_CENTER_X(tile.rec));
            player->rec.x = SnapToPixel(player->rec.x + (minx-maxx) * push_dir);
            player->vel.x = 0;
            player->acc.x = 0;
            break;
        }
    }

    player->rec.y += dp.y;
    for (int i = 0; i < map->size; i++) {
        Tile tile = map->tiles[i];
        
        float miny = fmin(REC_BOTTOM(player->rec), REC_BOTTOM(tile.rec));
        float maxy = fmax(REC_TOP(player->rec), REC_TOP(tile.rec));

        if (IsColliding(player->rec, tile.rec)) {
            // collider = tile.rec;
            int push_dir = SIGNUM(REC_CENTER_Y(player->rec) - REC_CENTER_Y(tile.rec));
            player->rec.y = SnapToPixel(player->rec.y + (miny-maxy) * push_dir);
            player->vel.y = 0;
            player->acc.y = 0;
            break;
        }
    }

    // if (!dy && dx && dp.y > 0 && dp.y < METER) {
    //     if (SnapToGrid(player->rec.x + dx) == SnapToGrid(player->rec.x)) {
    //         return;
    //     }
    //     Rectangle squish = (Rectangle){SnapToGrid(player->rec.x + dx), player->rec.y + dp.y, player->rec.width, player->rec.height};
    //     bool colliding = false;
    //     for (int i=0; i < map->size; i++) {
    //         if (IsColliding(squish, map->tiles[i].rec)) {
    //             colliding = true;
    //         }
    //     }
    //     if (!colliding) {
    //         player->rec = squish;
    //     }
    //     return;
    // }

    // if (dx) {
        
    // }

    // if (dy) {
    //     player->rec.y = SnapToPixel(player->rec.y + dy);
    //     player->vel.y = 0;
    //     player->acc.y = 0;
    // }
}

void ApplyForces(Player *player, Map *map) {
    player->acc = Vector2Scale(player->SF, 1 / player->m);
    player->vel = Vector2Add(player->vel, Vector2Scale(player->acc, GetFrameTime()));
    MovePlayer(player, map);
}


void GenerateForces(Player *player) {
    Vector2 v_move = (Vector2){IsKeyDown(KEY_D) - IsKeyDown(KEY_A),
                               IsKeyDown(KEY_S) - IsKeyDown(KEY_W)};
    v_move = Vector2Normalize(v_move);
    player->SF = Vector2Add(player->SF, Vector2Scale(v_move, player->speed));

    float clamped_drag = fmin(player->drag_const, player->m / GetFrameTime());
    Vector2 f_drag = Vector2Scale(player->vel, -clamped_drag);
    player->SF = Vector2Add(player->SF, f_drag);
}
