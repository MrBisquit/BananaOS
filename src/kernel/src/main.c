#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
//#include <limine.h>
#include <limine.h>

// Flanterm
#include <flanterm/flanterm.h>
#include <flanterm/backends/fb.h>

// Graphics
#include <graphics.h>

// Set the base revision to 2, this is recommended as this is the latest
// base revision described by the Limine boot protocol specification.
// See specification for further info.

__attribute__((used, section(".requests")))
static volatile LIMINE_BASE_REVISION(2);

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, usually, they should
// be made volatile or equivalent, _and_ they should be accessed at least
// once or marked as used with the "used" attribute as done here.

__attribute__((used, section(".requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

// Finally, define the start and end markers for the Limine requests.
// These can also be moved anywhere, to any .c file, as seen fit.

__attribute__((used, section(".requests_start_marker")))
static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".requests_end_marker")))
static volatile LIMINE_REQUESTS_END_MARKER;

// GCC and Clang reserve the right to generate calls to the following
// 4 functions even if they are not directly called.
// Implement them as the C specification mandates.
// DO NOT remove or rename these functions, or stuff will eventually break!
// They CAN be moved to a different .c file.

void *memcpy(void *dest, const void *src, size_t n) {
    uint8_t *pdest = (uint8_t *)dest;
    const uint8_t *psrc = (const uint8_t *)src;

    for (size_t i = 0; i < n; i++) {
        pdest[i] = psrc[i];
    }

    return dest;
}

void *memset(void *s, int c, size_t n) {
    uint8_t *p = (uint8_t *)s;

    for (size_t i = 0; i < n; i++) {
        p[i] = (uint8_t)c;
    }

    return s;
}

void *memmove(void *dest, const void *src, size_t n) {
    uint8_t *pdest = (uint8_t *)dest;
    const uint8_t *psrc = (const uint8_t *)src;

    if (src > dest) {
        for (size_t i = 0; i < n; i++) {
            pdest[i] = psrc[i];
        }
    } else if (src < dest) {
        for (size_t i = n; i > 0; i--) {
            pdest[i-1] = psrc[i-1];
        }
    }

    return dest;
}

int memcmp(const void *s1, const void *s2, size_t n) {
    const uint8_t *p1 = (const uint8_t *)s1;
    const uint8_t *p2 = (const uint8_t *)s2;

    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] < p2[i] ? -1 : 1;
        }
    }

    return 0;
}

// Halt and catch fire function.
static void hcf(void) {
    asm ("cli");
    for (;;) {
        asm ("hlt");
    }
}

int ft_ctx_init = 0; // Defines if it's initialised or not, no point in attempting to use it without it.
struct flanterm_context *ft_ctx_public;

// The following will be our kernel's entry point.
// If renaming _start() to something else, make sure to change the
// linker script accordingly.
void _start(void) {
    // Ensure the bootloader actually understands our base revision (see spec).
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        hcf();
    }

    // Ensure we got a framebuffer.
    if (framebuffer_request.response == NULL
     || framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }

    // Fetch the first framebuffer.
    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

    // Note: we assume the framebuffer model is RGB with 32-bit pixels.
    for (size_t i = 0; i < 100; i++) {
        volatile uint32_t *fb_ptr = framebuffer->address;
        fb_ptr[i * (framebuffer->pitch / 4) + i] = 0xffffff;
    }

    struct flanterm_context *ft_ctx = flanterm_fb_init(
        NULL, NULL,
        framebuffer->address, framebuffer->width, framebuffer->height, framebuffer->pitch,
        framebuffer->red_mask_size, framebuffer->red_mask_shift,
        framebuffer->green_mask_size, framebuffer->green_mask_shift,
        framebuffer->blue_mask_size, framebuffer->blue_mask_shift,
        NULL,
        NULL, NULL,
        NULL, NULL,
        NULL, NULL,
        NULL, 0, 0, 1,
        0, 0,
        0
    );

    ft_ctx_public = ft_ctx;
    ft_ctx_init = 1;

    //const char msg[] = "Hello, world!\nThis is a test of the flanterm terminal emulator.\n";
    const char msg[] = "Welcome to BananaOS, initialising...\n";

    flanterm_write(ft_ctx, msg, sizeof(msg));

    drawFilledRectangle(ft)

    //panic("Idk", "SOMETHING_WENT_WRONG");

    /*unsigned int currentColour = 0x000000;

    int x = 0;
    int y = 0;

    for (size_t k = 0; k < 3 * 100; k++)
    {
        for (size_t i = 0; i < 256; i++)
        {
            for (size_t j = 0; j < 256; j++)
            {

                volatile uint32_t* fb_ptr = framebuffer->address;
                fb_ptr[(i + x + 1) + (j + y + 1) * framebuffer->width] = 0x000000;
            }
        }

        for (size_t i = 0; i < 256; i++)
        {
            for (size_t j = 0; j < 256; j++)
            {
                currentColour += 0x000001;
                //currentColour += 0x000110;

                const char msga[] = "\rRender                       Rendering...";
                //flanterm_write(ft_ctx, msga, sizeof(msga));

                volatile uint32_t* fb_ptr = framebuffer->address;
                fb_ptr[(i + x) + (j + y) * framebuffer->width] = currentColour;
            }

            currentColour += 0x001000;
        }

        for (size_t i = 0; i < 60; i++)
        {
            ;
            ;
            ;
            ;
            ;
            ;
        }

        currentColour += 0x010000;
        currentColour *= 0x000020;

        x += 1;
        y += 1;

        if(x == 100) {
            x = 0;
            y = 0;
        }

        //k--;
    }

    for (size_t k = 0; k < (3 * 100) - 3 * 100 + 1; k++)
    {
        for (size_t i = 0; i < 256; i++)
        {
            for (size_t j = 0; j < 256; j++)
            {

                volatile uint32_t* fb_ptr = framebuffer->address;
                //fb_ptr[i + j * framebuffer->width] = 0x000000;
            }
        }

        for (size_t i = 0; i < 256; i++)
        {
            for (size_t j = 0; j < 256; j++)
            {
                currentColour += 0x000001;
                //currentColour += 0x000110;

                const char msga[] = "\rRender                       Rendering...";
                flanterm_write(ft_ctx, msga, sizeof(msga));

                volatile uint32_t* fb_ptr = framebuffer->address;
                fb_ptr[i + j * framebuffer->width] = currentColour;
            }

            //currentColour += 0x001000;
        }

        for (size_t i = 0; i < 60; i++)
        {
            ;
            ;
            ;
            ;
            ;
            ;
        }

        currentColour += 0x010000;

        //k--;
    }

   for (size_t k = 0; k < 3 * 100; k++)
    {
        for (size_t i = 0; i < framebuffer->width; i++)
        {
            for (size_t j = 0; j < framebuffer->height; j++)
            {

                volatile uint32_t* fb_ptr = framebuffer->address;
                //fb_ptr[i + j * framebuffer->width] = 0x000000;
            }
        }

        for (size_t i = 0; i < framebuffer->width; i++)
        {
            for (size_t j = 0; j < framebuffer->height; j++)
            {
                currentColour += 0x000001;
                //currentColour += 0x000110;

                const char msga[] = "\rRender                       Rendering...";
                flanterm_write(ft_ctx, msga, sizeof(msga));

                volatile uint32_t* fb_ptr = framebuffer->address;
                fb_ptr[i + j * framebuffer->width] = currentColour;
            }

            //currentColour += 0x001000;
        }

        for (size_t i = 0; i < 60; i++)
        {
            ;
            ;
            ;
            ;
            ;
            ;
        }

        currentColour += 0x010000;

        //k--;
    }*/

    // We're done, just hang...
    hcf();
}

void panic(const char msg[], const char error_code[]) {
    if(ft_ctx_init == 0) hcf(); // Nothing you can do besides halting the CPU.

    ft_ctx_public->set_cursor_pos(ft_ctx_public, 0, 0);
    //const char write[] = "Kernel Panic!\nSystem halted: " + msg + "\n\nError code: " + error_code;
    const char write[] = "Kernel Panic!\nSystem halted:";
    flanterm_write(ft_ctx_public, write, sizeof(write));

    hcf();
}