#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdbool.h>
#include <stdint.h>

#define BLACK 0x000000
#define WHITE 0xFFFFFF
#define YELLOW 0xFFFF00

// Globals
extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern uint32_t *color_buffer;
extern SDL_Texture *color_buffer_texture;
// Screen width and height global vars
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;


bool initialize_window(void);
void setup(void);
bool process_input(void);
void update(void);
void clear_color_buffer(uint32_t color);
void render_color_buffer(void);
void draw_grid(uint32_t color);
void draw_rect(int cord_x, int cord_y, int width, int height, uint32_t color);
void draw_pixel(int x, int y, uint32_t color);
void render(void);
void destroy_window(void);


#endif // DISPLAY_H
