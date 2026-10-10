#include "../include/camera.h"
#include "../include/geometry.h"
#include "../include/helpers.h"

Vector2 FocusOnMapTiling(Camera2D *camera) {
    // i want the blocks on -50, 50 to be visible
    camera->target = (Vector2){0, 0};
    camera->zoom = fmin(SCREEN_WIDTH / (TILE_SIDE_LEN * MAX_MAP_TILE_WIDTH),
                        SCREEN_HEIGHT / (TILE_SIDE_LEN * MAX_MAP_TILE_HEIGHT));
    camera->offset = (Vector2){SnapToPixel(SCREEN_WIDTH / 2.0f), SnapToPixel(SCREEN_WIDTH / 2.0f)};
    return (Vector2){0,0};
}

Vector2 FocusOnTarget(Camera2D *camera, Vector2 target) {
    camera->target = (Vector2){SnapToPixel(target.x), SnapToPixel(target.y)};
    camera->offset = (Vector2){SnapToPixel(SCREEN_WIDTH / 2.0f),
                               SnapToPixel(SCREEN_HEIGHT / 2.0f)};
    return Vector2Subtract(camera->target, target);
}

Vector2 FocusCamera(Camera2D *camera, GameModeType modeType, Player *player) {
    switch (modeType) {
    case CONSOLE:
    case DEBUG:
    case FREEPLAY: {
        return FocusOnTarget(camera, REC_CENTER(player->rec));
        break;
    }
    case MAPEDIT: {
        return FocusOnMapTiling(camera);
        break;
    }
    }
}
