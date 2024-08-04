#include <limine.h>
#include <graphics.h>

void clear_screen(struct limine_framebuffer *framebuffer, unsigned int color)
{
    // Note: we assume the framebuffer model is RGB with 32-bit pixels.
    /*for (uint64_t i = 0; i < framebuffer->width; i++) {
        volatile uint32_t* fb_ptr = framebuffer->address;
        fb_ptr[i * (framebuffer->pitch / 4) + i] = color;
    }*/

    memset(framebuffer->address, color, framebuffer->height * framebuffer->width * 4); // @xrc2alt

    /*for (uint64_t y = 0; y < framebuffer->height; y++) {
        // Working from top to bottom
        
        for(uint64_t x = 0; x < framebuffer->width; x++) {
            // Working from left to right

            DrawPoint(x, y, color);
        }
    }*/
}

void draw_filled_rectangle(struct limine_framebuffer *framebuffer, int x, int y, int width, int height, unsigned int color)
{
    // This is not a rectangle lmao
    /*
    volatile uint32_t* fb_ptr = framebuffer->address;
    for (uint64_t i = x; i < framebuffer->width + x; i++)
    {
        for (uint64_t j = 0; j < framebuffer->height + y; j++)
        {
            fb_ptr[i+j*framebuffer->width] = color;
        }
    }
    */

   volatile uint32_t* fb_ptr = framebuffer->address;

   // Work out every pixel we need
   for (uint64_t _y = 0; _y < height; _y++) {
        // Work from top to bottom, makes it easier
        uint64_t posY = _y + y;

        for (uint64_t _x = 0; _x < width; _x++) {
            // Now work from left to right
            uint64_t posX = _x + x;

            fb_ptr[posX + posY * framebuffer->width] = color; // Draw the position on the screen
        }
   }
}

void draw_rectangle(struct limine_framebuffer *framebuffer, int x, int y, int width, int height, unsigned int color) {
    // Clearly my maths isn't right lol

    draw_line(framebuffer, x, y, x + width, y, color); // Top line
    draw_line(framebuffer, x, y, x, y + height, color); // Left down line
    draw_line(framebuffer, x, y + height, x + width, y + height, color); // Bottom line
    draw_line(framebuffer, x + width, y + height, x + width, y, color); // Right up line
}

void draw_point(struct limine_framebuffer *framebuffer, int x, int y, unsigned int color)
{
    volatile uint32_t* fb_ptr = framebuffer->address;
    fb_ptr[x + y * framebuffer->width] = color;
}

void draw_line(struct limine_framebuffer *framebuffer, int x1, int y1, int x2, int y2, unsigned int color) 
{
    int dx, dy, p, x, y;

    dx = x2 - x1;
    dy = y2 - y1;

    x = x1;
    y = y1;

    p = 2 * dy - dx;

    for (int i = 0; i < dx; i++) {
        if (p < 0) {
            draw_point(framebuffer, x, y, color);
            y = y + 1;
            p = p + 2 * dy;
        }
        else {
            draw_point(framebuffer, x, y, color);
            x = x + 1;
            y = y + 1;
            p = p + 2 * dy - 2 * dx;
        }
    }
}

unsigned int fetch_pixel(struct limine_framebuffer *framebuffer, int x, int y) {
    volatile uint32_t* fb_ptr = framebuffer->address;

    return fb_ptr[x + y * framebuffer->width];
}

int out_of_bounds(struct limine_framebuffer *framebuffer, int x, int y) {
    // Sees if the provided coordinates are in the bounds
    // (So we don't write to a part of memory we're not suppost to)

    int InBounds = 1; // 0 = No, 1 = Yes

    if(x > framebuffer->width || 0 > x
    || y > framebuffer->height || 0 > y) {
        InBounds = 0;
    }

    return InBounds;
}