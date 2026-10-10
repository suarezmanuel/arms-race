#ifndef GEOMETRY_H
#define GEOMETRY_H

#define REC_CENTER(rec)   (Vector2){rec.x + rec.width * 0.5f, rec.y + rec.height * 0.5f}  
#define REC_CENTER_X(rec) (rec.x + rec.width * 0.5f) 
#define REC_CENTER_Y(rec) (rec.y + rec.height * 0.5f) 
#define REC_LEFT(rec)     (rec.x)
#define REC_RIGHT(rec)    (rec.x + rec.width)
#define REC_TOP(rec)      (rec.y)
#define REC_BOTTOM(rec)   (rec.y + rec.height)
#define REC_TL(rec)       (Vector2){rec.x, rec.y}
#define REC_SIZE(rec)     (Vector2){rec.width, rec.height}
#define REC_EQUAL(rec1,rec2) (rec1.x == rec2.x && rec1.y == rec2.y && rec1.width == rec2.width && rec1.height == rec2.height)
#define REC_MOVE(rec, offset) (Rectangle){rec.x + offset.x, rec.y + offset.y, rec.width, rec.height}

#endif