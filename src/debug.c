#include "../include/debug.h"
#include "../include/consts.h"
#include "../include/geometry.h"
#include "../include/helpers.h"
#include "../include/drawing.h"
#include "../include/raylib/raylib.h"
#include "../include/raylib/raymath.h"
#include "stddef.h"
#include "string.h"
#include "stdio.h"

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
    bool to_world;
} DebugCircle;

typedef struct DebugLine {
    Vector2 start;
    Vector2 end;
    Color color;
    float duration;
    bool is_vector;
    bool to_world;
} DebugLine;

typedef struct DebugRectangle {
    Rectangle rec;
    Color color;
    float duration;
    bool to_world;
} DebugRectangle;

typedef struct DebugText {
    const char *text;
    Vector2 pos;
    float font_size;
    Color color;
    float duration;
    bool to_world;
} DebugText;


static DebugCircle debug_circles[MAX_DEBUG_CIRCLES];
static size_t debug_circles_count = 0;

static DebugLine debug_lines[MAX_DEBUG_LINES];
static size_t debug_lines_count = 0;

static DebugRectangle debug_rectangles[MAX_DEBUG_RECTANGLES];
static size_t debug_rectangles_count = 0;

static DebugText debug_texts[MAX_DEBUG_TEXTS];
static size_t debug_texts_count = 0;

bool DebugCircleEquals(void *pa, void *pb) {
    DebugCircle a = *(DebugCircle *)pa;
    DebugCircle b = *(DebugCircle *)pb;
    return Vector2Equals(a.center, b.center) && a.radius == b.radius &&
           ColorIsEqual(a.color, b.color) && a.to_world == b.to_world;
}

bool DebugLineEquals(void *pa, void *pb) {
    DebugLine a = *(DebugLine *)pa;
    DebugLine b = *(DebugLine *)pb;
    return Vector2Equals(a.start, b.start) && Vector2Equals(a.end, b.end) &&
           ColorIsEqual(a.color, b.color) && a.is_vector == b.is_vector && a.to_world == b.to_world;
}

bool DebugRectangleEquals(void *pa, void *pb) {
    DebugRectangle a = *(DebugRectangle *)pa;
    DebugRectangle b = *(DebugRectangle *)pb;
    return REC_EQUAL(a.rec, b.rec) && ColorIsEqual(a.color, b.color) &&
           a.to_world == b.to_world;
}

bool DebugTextEquals(void *pa, void *pb) {
    DebugText a = *(DebugText *)pa;
    DebugText b = *(DebugText *)pb;
    return Vector2Equals(a.pos, b.pos) && strcmp(a.text, b.text) &&
           ColorIsEqual(a.color, b.color) && a.font_size == b.font_size &&
           a.to_world == b.to_world;
}

void insert(void *arr, void *element, size_t size, size_t *count, size_t max,
            bool (*cmp)(void *, void *), int duration_offset) {
    if (*count >= max) {
        return;
    }
    void *dst = OFFSET_PTR(arr, *count * size);
    if (cmp(dst, element)) {
        float *dst_duration = (float *)OFFSET_PTR(dst, duration_offset);
        float *src_duration = (float *)OFFSET_PTR(element, duration_offset);
        *dst_duration = fmax(*dst_duration, *src_duration);
    } else {
        memcpy(dst, element, size);
        *count += 1;
    }
}

void RegisterDebugCircle(Vector2 center, float radius, Color color, float duration,
                     bool to_world) {
    DebugCircle dcirc =
        (DebugCircle){center, radius, color, duration, to_world};
    insert(debug_circles, (void *)&dcirc, sizeof(DebugCircle),
           &debug_circles_count, MAX_DEBUG_CIRCLES, DebugCircleEquals,
           offsetof(DebugCircle, duration));
}

void RegisterDebugLine(Vector2 start, Vector2 end, Color color, float duration, bool is_vector,
                   bool to_world) {
    DebugLine dline = (DebugLine){start, end, color, duration, is_vector, to_world};
    insert(debug_lines, (void *)&dline, sizeof(DebugLine), &debug_lines_count,
           MAX_DEBUG_LINES, DebugLineEquals, offsetof(DebugLine, duration));
}

void RegisterDebugRect(Rectangle rec, Color color, float duration, bool to_world) {
    DebugRectangle drect = (DebugRectangle){rec, color, duration, to_world};
    insert(debug_rectangles, (void *)&drect, sizeof(DebugRectangle),
           &debug_rectangles_count, MAX_DEBUG_RECTANGLES, DebugRectangleEquals,
           offsetof(DebugRectangle, duration));
}

void RegisterDebugText(const char *text, Vector2 pos, float font_size, Color color,
                   float duration, bool to_world) {
    DebugText dtext =
        (DebugText){_strdup(text), pos, font_size, color, duration, to_world};
    insert(debug_texts, (void *)&dtext, sizeof(DebugText), &debug_texts_count,
           MAX_DEBUG_TEXTS, DebugTextEquals, offsetof(DebugText, duration));
}

void decrement(void* arr, size_t* count, size_t element_size, int duration_offset) {
    size_t finished_count = 0;                                    
    float dt = GetFrameTime();                                    
             
    for (int i = *count; i >= 0; i--) {        
        void* element = OFFSET_PTR(arr, i * element_size);  
        float* duration = OFFSET_PTR(element, duration_offset);               
        *duration -= dt;   
                                                            
        if (*duration <= 0) {                               
            finished_count++;
            *count = fmax(0, *count-1);
        } else if (finished_count > 0) {   
            void* dst = OFFSET_PTR(arr, (i + 1) * element_size);
            void* src = OFFSET_PTR(arr, (i + finished_count + 1) * element_size);
            memcpy(dst, src, element_size * (*count - (i + finished_count)));
            finished_count = 0;                                   
        } 
    }                        
}


void DebugDraw(Camera2D *camera) {
    printf("%zu %zu %zu %zu\n", debug_circles_count, debug_lines_count, debug_rectangles_count, debug_texts_count);

    for (size_t i = 0; i < debug_circles_count; i++) {
        DebugCircle circle = debug_circles[i];
        DrawCircleV(VEC_TO_WORLD_IF(circle.center, circle.to_world, camera),
                    circle.radius * PIXEL, circle.color);
    }

    for (size_t i = 0; i < debug_lines_count; i++) {
        DebugLine line = debug_lines[i];
        if (line.is_vector) {
            DrawVector(camera, line.start, Vector2Subtract(line.end, line.start), line.color, line.to_world);
        } else {
            DrawLineV(VEC_TO_WORLD_IF(line.start, line.to_world, camera),
                      VEC_TO_WORLD_IF(line.end, line.to_world, camera), line.color);
        }
    }

    for (size_t i = 0; i < debug_rectangles_count; i++) {
        DebugRectangle drec = debug_rectangles[i];
        DrawRectangleV(VEC_TO_WORLD_IF(REC_TL(drec.rec), drec.to_world, camera),
                       Vector2Scale(REC_SIZE(drec.rec), PIXEL), drec.color);
    }

    for (size_t i = 0; i < debug_texts_count; i++) {
        DebugText text = debug_texts[i];
        DrawTextEx(GetFontDefault(), text.text,
                   VEC_TO_WORLD_IF(text.pos, text.to_world, camera),
                   10, 0.5, text.color);
    }

    decrement(debug_circles, &debug_circles_count, sizeof(DebugCircle), offsetof(DebugCircle, duration));
    decrement(debug_lines, &debug_lines_count, sizeof(DebugLine), offsetof(DebugLine, duration));
    decrement(debug_rectangles, &debug_rectangles_count, sizeof(DebugRectangle), offsetof(DebugRectangle, duration));
    decrement(debug_texts, &debug_texts_count, sizeof(DebugText), offsetof(DebugText, duration));

}
