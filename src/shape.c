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

Donut *shape_new_donut(double r1, double r2, double viewer_distance,
                       double object_distance)
{
    Donut *donut = (Donut *)malloc(sizeof(Donut));
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

void shape_release_donut(Donut *donut)
{
    free(donut);
}

void shape_prepare_next_donut(Donut *donut, char buf[])
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

    z_buf = (double *)malloc((canvas_width * canvas_height) * sizeof(double));
    for (idx = 0; idx < canvas_width * canvas_height; ++idx)
        z_buf[idx] = -DBL_MAX;

    donut->delta += RESO;
    donut->a_radian += RESO;

    sin_d = sin(donut->delta);
    cos_d = cos(donut->delta);
    sin_a = sin(donut->a_radian);
    cos_a = cos(donut->a_radian);

    while (psi <= 2 * M_PI)
    {
        psi += RESO;
        sin_p = sin(psi);
        cos_p = cos(psi);
        while (theta <= 2 * M_PI)
        {
            double       x, y, z;
            unsigned int xp, yp;
            double       z_apo;

            theta += RESO;
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
        theta = 0;
    }
}

static double dot_product_3d(double a[3], double b[3])
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}
