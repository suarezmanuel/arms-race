#include "../include/camera.h"
#include "../include/helpers.h"

void focus_on_map_tiling(Camera2D *camera) {
    // i want the blocks on -50, 50 to be visible
    camera->target = (Vector2){0, 0};
    camera->zoom = fmin(SCREEN_WIDTH / (TILE_SIDE_LEN * MAX_MAP_TILE_WIDTH),
                        SCREEN_HEIGHT / (TILE_SIDE_LEN * MAX_MAP_TILE_HEIGHT));
    camera->offset = (Vector2){SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
}

void focus_on_player(Camera2D *camera, Vector2 target) {
    camera->target =
        (Vector2){snap_to_pixel(target.x), snap_to_pixel(target.y)};
    camera->offset = (Vector2){snap_to_pixel(SCREEN_WIDTH / 2.0f),
                               snap_to_pixel(SCREEN_HEIGHT / 2.0f)};
}

void focus_camera(Camera2D *camera, GameState *game) {
    switch (game->mode.type) {
    case CONSOLE:
    case DEBUG:
    case FREEPLAY: {
        focus_on_player(
            camera, (Vector2){game->player.pos.x + game->player.size.x / 2.0f,
                              game->player.pos.y + game->player.size.y / 2.0f});
        break;
    }
    case MAPEDIT: {
        focus_on_map_tiling(&game->camera);
        break;
    }
    }
}