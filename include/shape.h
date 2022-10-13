/**************
 * 
 * shape.h
 * 
 **************/

#ifndef SHAPE_H
#define SHAPE_H

typedef struct Donut
{
    double r1;
    double r2;
    double viewer_distance;
    double object_distance;
    double delta;
    double a_radian;
    double light_source[3];
} Donut;

extern Donut *shape_new_donut(double r1, double r2, double viewer_distance,
                              double object_distance);
extern void shape_release_donut(Donut *donut);
extern void shape_prepare_next_donut(Donut *donut, char buf[]);

#endif
