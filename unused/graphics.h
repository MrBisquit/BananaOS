#include <limine.h>

// Some basic functions for drawing things to the screen
void clearScreen(limine_framebuffer *framebuffer, unsigned int color);
void drawRectangle(limine_framebuffer *framebuffer, int x, int y, int width, int height, unsigned int color);
void drawFilledRectangle(limine_framebuffer *framebuffer, int x, int y, int width, int height, unsigned int color);
void drawPoint(limine_framebuffer *framebuffer, int x, int y, unsigned int color);
void drawLine(limine_framebuffer *framebuffer, int x1, int y1, int x2, int y2, unsigned int color);
unsigned int fetchPixel(limine_framebuffer *framebuffer, int x, int y);
int outOfBounds(limine_framebuffer *framebuffer, int x, int y);