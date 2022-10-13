/**************
 * 
 * canvas.c
 * 
 **************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "canvas.h"

static void get_canvas_size();

unsigned int canvas_height;
unsigned int canvas_width;

void canvas_print(char buf[])
{
    unsigned int x, y;
    printf("\x1b[H");
    for (y = 0; y < canvas_height; ++y)
    {
        for (x = 0; x < canvas_width; ++x)
        {
            unsigned int idx = y * canvas_width + x;
            if (buf[idx] == '\0')
            {
                printf("  ");
            }
            else
            {
                printf("%c ", buf[idx]);
            }
        }
        printf("\n");
    }
}

char *canvas_prepare_buffer()
{
    char        *buf;
    unsigned int buf_size;

    get_canvas_size();
    buf_size = canvas_height * canvas_width;
    buf = (char *)malloc(buf_size * sizeof(char));
    memset(buf, '\0', buf_size);

    return buf;
}

void canvas_release_buffer(char *buf)
{
    free(buf);
}

static void get_canvas_size()
{
    struct winsize s;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &s);
    canvas_height = s.ws_row;
    canvas_width = s.ws_col / 2;
}
