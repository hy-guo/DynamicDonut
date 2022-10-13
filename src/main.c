/***********************************************
 * 
 * Draw a 3D dynamic donut in the terminal.
 * 
 * https://github.com/hy-guo/DynamicDonut
 * 
 ***********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "canvas.h"
#include "shape.h"

int main(int argc, char *argv[])
{
    int    fps = 30;
    Donut *donut;

    donut = shape_new_donut(10, 5, 15, 25);
    printf("\x1b[2J");
    printf("\x1b[?25l");

    while (1)
    {
        char *buf = canvas_prepare_buffer();
        shape_prepare_next_donut(donut, buf);
        canvas_print(buf);
        canvas_release_buffer(buf);

        usleep(1000000 / fps);
    }

    shape_release_donut(donut);
    printf("\x1b[?25h");

    return 0;
}
