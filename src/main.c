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
#include <signal.h>

#include "canvas.h"
#include "shape.h"

static void exit_handler();
static void set_sigint_act();

int main(int argc, char *argv[])
{
    int    fps = 30;
    Donut *donut;

    printf("\x1b[2J");
    printf("\x1b[?25l");

    set_sigint_act();

    donut = shape_new_donut(10, 5, 15, 25);

    while (1)
    {
        char *buf = canvas_prepare_buffer();

        if (shape_prepare_next_donut(donut, buf) == 0)
            canvas_print(buf);
        else
            break;

        canvas_release_buffer(buf);

        usleep(1000000 / fps);
    }

    shape_release_donut(donut);
    printf("\x1b[?25h");

    return 0;
}

static void exit_handler()
{
    printf("\x1b[?25h");
    exit(0);
}

static void set_sigint_act()
{
    struct sigaction act, old_act;
    act.sa_handler = exit_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_NODEFER | SA_RESETHAND;
    sigaction(SIGINT, &act, &old_act);
}
