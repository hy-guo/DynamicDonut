/**************
 * 
 * shape.h
 * 
 **************/

#ifndef SHAPE_H
#define SHAPE_H

typedef enum ShapeTag
{
    S_Invalid = 0,
    S_Donut,
    S_Heart
} ShapeTag;

typedef struct Shape
{
    ShapeTag type;
} Shape;

typedef struct Donut
{
    ShapeTag type;
    double   r1;
    double   r2;
    double   viewer_distance;
    double   object_distance;
    double   delta;
    double   a_radian;
    double   light_source[3];
} Donut;

typedef struct Heart
{
    ShapeTag type;
    double   a;
    double   b;
    double   c;
    double   multiplier;
    int      direction;
} Heart;

extern void shape_release_shape(Shape *shape);
extern int  shape_prepare_next_shape(Shape *shape, char buf[]);

extern Donut *shape_new_donut(double r1, double r2, double viewer_distance,
                              double object_distance);
extern void shape_release_donut(Donut *donut);
extern int  shape_prepare_next_donut(Donut *donut, char buf[]);

extern Heart *shape_new_heart(double a, double b, double c);
extern void   shape_release_heart(Heart *heart);
extern int    shape_prepare_next_heart(Heart *heart, char buf[]);

#endif
