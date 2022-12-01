/**************
 * 
 * shape.c
 * 
 **************/

#include <stdlib.h>
#include <math.h>
#include <float.h>

#include "shape.h"
#include "canvas.h"

static double dot_product_3d(double a[3], double b[3]);

static double RESO = 0.05;
static char LUMINANCE_CHARS[12] =
    {'.', ',', '-', '~', ':', ';', '=', '!', '*', '#', '$', '@'};

int shape_prepare_next_shape(Shape *shape, char buf[])
{
    switch (shape->type)
    {
        case S_Donut:
            return shape_prepare_next_donut((Donut *)shape, buf);
            break;
        case S_Heart:
            return shape_prepare_next_heart((Heart *)shape, buf);
            break;
        default:
            return -1;
    }
}

void shape_release_shape(Shape *shape)
{
    switch (shape->type)
    {
        case S_Donut:
            shape_release_donut((Donut *)shape);
            break;
        case S_Heart:
            shape_release_heart((Heart *)shape);
            break;
        default:
            return;
    }
}

Donut *shape_new_donut(double r1, double r2, double viewer_distance,
                       double object_distance)
{
    Donut *donut = (Donut *)malloc(sizeof(Donut));
    donut->type = S_Donut;
    donut->r1 = r1;
    donut->r2 = r2;
    donut->viewer_distance = viewer_distance;
    donut->object_distance = object_distance;
    donut->delta = 0;
    donut->a_radian = 0;
    donut->light_source[0] = 1;
    donut->light_source[1] = 0;
    donut->light_source[2] = 1;
    return donut;
}

Heart *shape_new_heart(double a, double b, double c)
{
    Heart *heart = (Heart *)malloc(sizeof(Heart));
    heart->type = S_Heart;
    heart->a = a;
    heart->b = b;
    heart->c = c;
    heart->multiplier = 1;
    heart->direction = 1;
    return heart;
}

void shape_release_donut(Donut *donut)
{
    free(donut);
}

void shape_release_heart(Heart *heart)
{
    free(heart);
}

int shape_prepare_next_donut(Donut *donut, char buf[])
{
    double psi = 0;
    double theta = 0;
    double wid_offset = canvas_width / 2;
    double hei_offset = canvas_height / 2;

    double sin_d, cos_d;
    double sin_a, cos_a;
    double sin_p, cos_p;
    double sin_t, cos_t;

    double *z_buf;
    int     idx;

    if (donut == NULL || buf == NULL)
        return -1;

    z_buf = (double *)malloc((canvas_width * canvas_height) * sizeof(double));
    for (idx = 0; idx < canvas_width * canvas_height; ++idx)
        z_buf[idx] = -DBL_MAX;

    donut->delta += RESO;
    donut->a_radian += RESO;

    sin_d = sin(donut->delta);
    cos_d = cos(donut->delta);
    sin_a = sin(donut->a_radian);
    cos_a = cos(donut->a_radian);

    for (psi = 0; psi < 2 * M_PI; psi += RESO)
    {
        sin_p = sin(psi);
        cos_p = cos(psi);

        for (theta = 0; theta < 2 * M_PI; theta += RESO)
        {
            double       x, y, z;
            unsigned int xp, yp;
            double       z_apo;

            sin_t = sin(theta);
            cos_t = cos(theta);

            z = ((donut->r2 * cos_t + donut->r1) * sin_d * cos_p + donut->r2 * sin_t * cos_d)
                * cos_a + (donut->r2 * cos_t + donut->r1) * sin_a * sin_p;
            z_apo = donut->viewer_distance / (donut->object_distance - z);

            x = (donut->r2 * cos_t + donut->r1) * cos_d * cos_p - donut->r2 * sin_d * sin_t;
            y = ((donut->r2 * cos_t + donut->r1) * sin_d * cos_p + donut->r2 * sin_t * cos_d)
                * sin_a - (donut->r2 * cos_t + donut->r1) * sin_p * cos_a;

            xp = (x * z_apo + wid_offset);
            yp = (y * z_apo + hei_offset);

            if (xp >= canvas_width || yp >= canvas_height)
                continue;

            if (z > z_buf[yp * canvas_width + xp])
            {
                double       lumi;
                unsigned int lumi_idx;
                double       normal[3];

                z_buf[yp * canvas_width + xp] = z;

                normal[0] = -1 * sin_d * sin_t + cos_d * cos_p * cos_t;
                normal[1] = (sin_d * cos_p * cos_t + sin_t * cos_d) * sin_a - sin_p * cos_a * cos_t;
                normal[2] = (sin_d * cos_p * cos_t + sin_t * cos_d) * cos_a + sin_a * sin_p * cos_t;
                lumi = dot_product_3d(normal, donut->light_source);
                lumi_idx = lumi >= 0 ? lumi * 6.5 : 0;

                buf[yp * canvas_width + xp] = LUMINANCE_CHARS[lumi_idx];
            }
        }
    }

    return 0;
}

int shape_prepare_next_heart(Heart *heart, char buf[])
{
    double theta = 0;
    double wid_offset = canvas_width / 2;
    double hei_offset = canvas_height / 2;

    if (heart == NULL || buf == NULL)
        return -1;

    if (heart->direction == 1)
    {
        heart->multiplier = (heart->multiplier + 1.25 * heart->multiplier * RESO);
        if (heart->multiplier > 1.5)
            heart->direction = -1;
    }
    else
    {
        heart->multiplier = (heart->multiplier - 1.75 * heart->multiplier * RESO);
        if (heart->multiplier < 1)
            heart->direction = 1;
    }

    for (theta = 0; theta < 2 * M_PI; theta += RESO)
    {
        double       r, r_max;
        double       sin_t, cos_t;
        double       x, y;
        unsigned int xp, yp;

        sin_t = sin(theta);
        cos_t = cos(theta);

        r_max = (heart->multiplier) *
                ((heart->a) / sqrt((heart->b) + (heart->c) * sin_t * fabs(cos_t)));

        for (r = 0; r <= r_max; r += RESO)
        {
            unsigned int lumi_idx;

            x = r * cos_t;
            y = r * sin_t;

            xp = (x + wid_offset);
            yp = (y + hei_offset);

            if (xp >= canvas_width || yp >= canvas_height)
                continue;

            lumi_idx = (r / r_max) * 12;
            buf[yp * canvas_width + xp] = LUMINANCE_CHARS[lumi_idx];
        }
    }

    return 0;
}

static double dot_product_3d(double a[3], double b[3])
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}
