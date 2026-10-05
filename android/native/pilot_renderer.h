#ifndef PILOT_RENDERER_H
#define PILOT_RENDERER_H
#include <stdbool.h>
bool pilot_renderer_start(int width,int height,int gles_major);
void pilot_renderer_stop(void);
void pilot_renderer_resize(int width,int height);
void pilot_renderer_drag(float x,float y);
bool pilot_renderer_press(float x,float y);
void pilot_renderer_draw(void);
void *pilot_renderer_save(unsigned long *size);
void pilot_renderer_restore(const void *data,unsigned long size);
#endif
