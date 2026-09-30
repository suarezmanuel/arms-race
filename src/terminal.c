#include "../include/raylib/raylib.h"
#include "../include/raylib/raygui.h"
#include "../include/terminal.h"
#include "../include/consts.h"
#include "../include/helpers.h"
#include "math.h"
#include "stdio.h"
#include "string.h"

#define MAX_LINES 16
#define FONT_SIZE 24
#define LEFT_MARGIN 40

char cmd[MAX_CMD_LEN];
int cmd_len = 0;


// char cmds [MAX_LINES][MAX_CMD_LEN];
// size_t cmd_count = 0;

void execute_command(char* cmd) {
    void* values; 

    char type_str[VAR_BUF_LEN];
    char name[VAR_BUF_LEN];
    char values_str[VAR_BUF_LEN];
    // VAR_TYPE name = value value value
    sscanf_s(cmd, "%s %s = %[^\n]", type_str, VAR_BUF_LEN, name, VAR_BUF_LEN, values_str, VAR_BUF_LEN);

    str_to_lower(type_str);
    str_to_lower(name);
    str_to_lower(values_str);

    VAR_TYPE type;
    for (int i=0; i < VAR_TYPE_COUNT; i++) {
        if (!strcmp(type_str, var_type_names[i])) {
            type = var_type_names_mapping[i];
            break;
        }
    }

    switch(type) {
        case FLOAT: {
            float value;
            sscanf_s(values_str, "%f", &value);
            set_cvar(name, &value);
            break;
        }
        case VECTOR2: {
            float values[2];
            sscanf_s(values_str, "%f %f", &values[0], &values[1]);
            set_cvar(name, values);
            break;
        }
        case VAR_TYPE_COUNT: { break; }
    }
}


void terminal_handle_inputs() {
    int k;
    while ((k = GetKeyPressed()) && k != 0) {
        char c = GetCharPressed();
        if (k == KEY_ENTER) {
            execute_command(cmd);
            cmd_len = 0;
            cmd[0] = '\0';
            continue;
        }
        
        if (cmd_len + 1 == MAX_CMD_LEN) {
            break;
        }
        if (!c) {
            continue;
        }
        cmd[cmd_len++] = c;
        cmd[cmd_len] = '\0';
    }

    if (IsKeyDown(KEY_BACKSPACE)) {
        // TODO: add key repeater struct and whatever
        cmd_len = fmax(0, cmd_len - 1);
        cmd[cmd_len] = '\0';
    }
}


void terminal_draw(GameState* game) {
    int text_height = MeasureTextEx(GetFontDefault(), "A", FONT_SIZE, 5).y;
    Color terminal_color = (Color){BLACK.r, BLACK.g, BLACK.b, 220};
    DrawRectangle(0, 0, SCREEN_WIDTH, 2 * text_height, terminal_color);

    char line [MAX_CMD_LEN];
    snprintf(line, MAX_CMD_LEN, "> %s", cmd);
    DrawText(line, LEFT_MARGIN, text_height, FONT_SIZE, RAYWHITE);
}
