#include "../include/handle_input.h"
#include "../include/terminal.h"

GameModeType handle_input_freeplay(Map* map, Camera2D* camera, GameModeType initialMode) {
    Vector2 mouse_world =
        GetScreenToWorld2D(GetMousePosition(), *camera);
    Vector2 mouse_tile =
        (Vector2){floor(mouse_world.x / TILE_SIDE_LEN) * TILE_SIDE_LEN,
                    floor(mouse_world.y / TILE_SIDE_LEN) * TILE_SIDE_LEN};
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        append_tile(map,
                    (Tile){BROWN, mouse_tile,
                            (Vector2){TILE_SIDE_LEN, TILE_SIDE_LEN}});
    }
    return initialMode;
}

GameModeType handle_input_console(Map* map, Camera2D* camera, GameModeType initialMode) {
    // append all keys to text buffer
    terminal_handle_inputs();

    if (IsKeyDown(KEY_ESCAPE)) {
        return FREEPLAY;
    }
    return initialMode;
}

void handle_inputs(GameState *game) {
    Vector2 mouse_world = GetScreenToWorld2D(GetMousePosition(), game->camera);
    game->camera.zoom = game->mode.zoom;

    GameModeType newType = game->mode.handle_input_function(&game->map, &game->camera, game->mode.type);
    game->mode = game_modes[newType];

    for (int i = 0; i < GAME_MODE_COUNT; i++) {
        if (IsKeyDown(game_modes[i].toggle_key)) {
            game->mode = game_modes[i];
            break;
        }
    }
}