/**************
 * 
 * canvas.h
 * 
 **************/

#ifndef CANVAS_H
#define CANVAS_H

extern unsigned int  canvas_height;
extern unsigned int  canvas_width;

extern int   canvas_print(char buf[]);
extern char *canvas_prepare_buffer();
extern void  canvas_release_buffer(char *buf);

#endif
