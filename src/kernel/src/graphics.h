#include <limine.h>

#ifndef GRAPHICS_H
#define GRAPHICS_H

// Some basic functions for drawing things to the screen
void clear_screen(struct limine_framebuffer *framebuffer, unsigned int color);
void draw_rectangle(struct limine_framebuffer *framebuffer, int x, int y, int width, int height, unsigned int color);
void draw_filled_rectangle(struct limine_framebuffer *framebuffer, int x, int y, int width, int height, unsigned int color);
void draw_point(struct limine_framebuffer *framebuffer, int x, int y, unsigned int color);
void draw_line(struct limine_framebuffer *framebuffer, int x1, int y1, int x2, int y2, unsigned int color);
unsigned int fetch_pixel(struct limine_framebuffer *framebuffer, int x, int y);
int out_of_bounds(struct limine_framebuffer *framebuffer, int x, int y);

/*void clearScreen(void *framebuffer, unsigned int color);
void drawRectangle(void *framebuffer, int x, int y, int width, int height, unsigned int color);
void drawFilledRectangle(void *framebuffer, int x, int y, int width, int height, unsigned int color);
void drawPoint(void *framebuffer, int x, int y, unsigned int color);
void drawLine(void *framebuffer, int x1, int y1, int x2, int y2, unsigned int color);
unsigned int fetchPixel(void *framebuffer, int x, int y);
int outOfBounds(void *framebuffer, int x, int y);*/

#endif // GRAPHICS_H