#include "../include/handle_input.h"
#include "../include/terminal.h"
#include "../include/helpers.h"

GameModeType HandleInputFreeplay(Map* map, Camera2D* camera, GameModeType initialMode) {
    camera->zoom = game_modes[FREEPLAY].zoom;
    Vector2 mouse_world =
        GetScreenToWorld2D(GetMousePosition(), *camera);
    Vector2 mouse_tile = SnapVectorToGrid(mouse_world);
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        AppendTile(map, (Tile){BROWN, (Rectangle){mouse_tile.x, mouse_tile.y, TILE_SIDE_LEN, TILE_SIDE_LEN}});
    }
    return initialMode;
}

GameModeType HandleInputConsole(Map* map, Camera2D* camera, GameModeType initialMode) {
    camera->zoom = game_modes[CONSOLE].zoom;
    // append all keys to text buffer
    TerminalHandleInputs();

    if (IsKeyDown(KEY_ESCAPE)) {
        return FREEPLAY;
    }
    return initialMode;
}

void HandleInputs(Camera2D* camera, GameState *game) {
    Vector2 mouse_world = GetScreenToWorld2D(GetMousePosition(), *camera);

    GameModeType newType = game->mode.handle_input_function(&game->map, camera, game->mode.type);
    game->mode = game_modes[newType];

    for (int i = 0; i < GAME_MODE_COUNT; i++) {
        if (IsKeyDown(game_modes[i].toggle_key)) {
            game->mode = game_modes[i];
            break;
        }
    }
}