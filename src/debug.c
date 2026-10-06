#include "../include/debug.h"
#include "../include/helpers.h"
#include "string.h"
#define MAX_DEBUG_CIRCLES 1024
#define MAX_DEBUG_LINES 1024
#define MAX_DEBUG_RECTANGLES 1024
#define MAX_DEBUG_TEXTS 1024
#define MAX_TEXT_LEN 64

typedef struct DebugCircle {
    Vector2 center;
    float radius;
    Color color;
    float duration;
} DebugCircle;

typedef struct DebugLine {
    Vector2 start;
    Vector2 end;
    Color color;
    float duration;
} DebugLine;

typedef struct DebugRectangle {
    Rectangle rec;
    Color color;
    float duration;
} DebugRectangle;

typedef struct DebugText {
    char text[MAX_TEXT_LEN];
    Vector2 pos;
    int font_size;
    Color color;
    float duration;
} DebugText;

static DebugCircle debug_circles[MAX_DEBUG_CIRCLES];
static int debug_circles_count = 0;

static DebugLine debug_lines[MAX_DEBUG_LINES];
static int debug_lines_count = 0;

static DebugRectangle debug_rectangles[MAX_DEBUG_RECTANGLES];
static int debug_rectangles_count = 0;

static DebugText debug_texts[MAX_DEBUG_TEXTS];
static int debug_texts_count = 0;

void register_debug_circle(Vector2 center, float radius, Color color,
                           float duration) {
    INSERT_TO_ARRAY(debug_circles, debug_circles_count, MAX_DEBUG_CIRCLES,
                    DebugCircle, center, radius, color, duration);
}

void register_debug_line(Vector2 start, Vector2 end, Color color,
                         float duration) {
    INSERT_TO_ARRAY(debug_lines, debug_lines_count, MAX_DEBUG_LINES, DebugLine,
                    start, end, color, duration);
}

void register_debug_rect(Rectangle rec, Color color, float duration) {
    INSERT_TO_ARRAY(debug_rectangles, debug_rectangles_count,
                    MAX_DEBUG_RECTANGLES, DebugRectangle, rec, color, duration);
}

void register_debug_text(const char *text, Vector2 pos, int font_size,
                         Color color, float duration) {
    if (debug_texts_count == MAX_DEBUG_TEXTS) {
        return;
    }
    DebugText *dtext = &debug_texts[debug_texts_count++];
    dtext->color = color;
    dtext->font_size = font_size;
    dtext->pos = pos;
    strncpy(dtext->text, text, MAX_TEXT_LEN);
    dtext->text[MAX_TEXT_LEN - 1] = '\0';
}

#define DECREMENT_DURATION(arr, count, type)                                   \
    do {                                                                       \
        size_t finished_count = 0;                                             \
        float dt = GetFrameTime();                                             \
                                                                               \
        for (size_t i = count; i >= 0; i--) {                                  \
            arr[i].duration -= dt;                                             \
                                                                               \
            if (arr[i].duration <= 0) {                                        \
                finished_count++;                                              \
            } else if (finished_count > 0) {                                   \
                memcpy(&arr[i + 1], &arr[i + finished_count + 1],              \
                       sizeof(type) * (count - (i + finished_count)));         \
                finished_count = 0;                                            \
            }                                                                  \
        }                                                                      \
    } while (0)

void debug_draw() {

    for (size_t i = 0; i < debug_circles_count; i++) {
        DebugCircle circle = debug_circles[i];
        DrawCircleV(circle.center, circle.radius, circle.color);
    }

    for (size_t i = 0; i < debug_lines_count; i++) {
        DebugLine line = debug_lines[i];
        DrawLineV(line.start, line.end, line.color);
    }

    for (size_t i = 0; i < debug_rectangles_count; i++) {
        DebugRectangle rec = debug_rectangles[i];
        DrawRectangleRec(rec.rec, rec.color);
    }

    for (size_t i = 0; i < debug_texts_count; i++) {
        DebugText text = debug_texts[i];
        DrawText(text.text, text.pos.x, text.pos.y, text.font_size, text.color);
    }

    DECREMENT_DURATION(debug_circles, debug_circles_count, DebugCircle);
    DECREMENT_DURATION(debug_lines, debug_lines_count, DebugLine);
    DECREMENT_DURATION(debug_rectangles, debug_rectangles_count,
                       DebugRectangle);
    DECREMENT_DURATION(debug_texts, debug_texts_count, DebugText);
}