#include "../include/terminal.h"
#include "../include/consts.h"
#include "../include/helpers.h"
#include "math.h"
#include "raylib/raygui.h"
#include "raylib/raylib.h"
#include "stdio.h"
#include "string.h"

#define MAX_LINES 16
#define FONT_SIZE 24
#define LEFT_MARGIN 40

char cmd[MAX_CMD_LEN];
int cmd_len = 0;

void ExecuteCommand(char *cmd) {
    void *values;

    char type_str[VAR_BUF_LEN];
    char name[VAR_BUF_LEN];
    char values_str[VAR_BUF_LEN];
    sscanf(cmd, "%" TOSTRING(VAR_BUF_LEN) "s ",
           "%" TOSTRING(VAR_BUF_LEN) "s = ", "%" TOSTRING(VAR_BUF_LEN) "[^\n]",
           type_str, name, values_str);

    StrToLower(type_str);
    StrToLower(name);
    StrToLower(values_str);

    VAR_TYPE type;
    for (int i = 0; i < VAR_TYPE_COUNT; i++) {
        if (!strcmp(type_str, var_type_names[i])) {
            type = var_type_names_mapping[i];
            break;
        }
    }

    switch (type) {
    case FLOAT: {
        float value;
        sscanf(values_str, "%f", &value);
        SetCvar(name, &value);
        break;
    }
    case VECTOR2: {
        float values[2];
        sscanf(values_str, "%f %f", &values[0], &values[1]);
        SetCvar(name, values);
        break;
    }
    case VAR_TYPE_COUNT: {
        break;
    }
    }
}

void TerminalHandleInputs() {
    int k;
    while ((k = GetKeyPressed()) && k != 0) {
        char c = GetCharPressed();
        if (k == KEY_ENTER) {
            ExecuteCommand(cmd);
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

void TerminalDraw(GameState *game) {
    int text_height = MeasureTextEx(GetFontDefault(), "A", FONT_SIZE, 5).y;
    Color terminal_color = (Color){BLACK.r, BLACK.g, BLACK.b, 220};
    DrawRectangle(0, 0, SCREEN_WIDTH, 2 * text_height, terminal_color);

    char line[MAX_CMD_LEN];
    snprintf(line, MAX_CMD_LEN, "> %s", cmd);
    DrawText(line, LEFT_MARGIN, text_height, FONT_SIZE, RAYWHITE);
}
