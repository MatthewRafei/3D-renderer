#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "include/display.h"
#include "include/vector.h"

// VECTORS
//const int N_POINTS = 9 * 9 * 9; // 729 different positions
#define N_POINTS (9 * 9 * 9)

vec3_t cube_points[N_POINTS];
vec2_t projected_points[N_POINTS];

// Globals
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
uint32_t *color_buffer = NULL;
SDL_Texture *color_buffer_texture = NULL;
float fov_factor = 250;

// Screen width and height global vars
int SCREEN_WIDTH = 1280;
int SCREEN_HEIGHT = 720;

bool initialize_window(void)
{
  // Initalize graphics, audio, input
  if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
    fprintf(stderr, "Error SDL init encountered an error\n");
    return false;
  }

  // Get Display information and set screen width/height
  // Fake fullscreen, borderless window?
  SDL_DisplayMode display_mode;
  SDL_GetCurrentDisplayMode(0, &display_mode);
  SCREEN_WIDTH = display_mode.w;
  SCREEN_HEIGHT = display_mode.h;
  
  // Create SDL window
  window = SDL_CreateWindow(
			    NULL,
			    SDL_WINDOWPOS_CENTERED,
			    SDL_WINDOWPOS_CENTERED,
			    SCREEN_WIDTH,
			    SCREEN_HEIGHT,
			    SDL_WINDOW_BORDERLESS
			    );
  
  if(!window){
    fprintf(stderr, "Encountered an error creating a SDL window\n");
    return false;
  }

  // Create SDL renderer
  renderer = SDL_CreateRenderer(window, -1, 0); // -1 get defaults, 0 no special flags

  if(!renderer){
    fprintf(stderr, "Error creating SDL renderer.\n");
    return false;
  }
  
  // Change video mode to real fullscreen
  SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
  
  return true;
}

void setup(void)
{
  // Alloc the required memory in bytes to hold the color buffer in mem
  color_buffer = (uint32_t *) malloc(sizeof(uint32_t) * SCREEN_WIDTH * SCREEN_HEIGHT);

  // Creating a SDL texture that is used to display the color buffer
  color_buffer_texture = SDL_CreateTexture(
					   renderer,
					   SDL_PIXELFORMAT_ARGB8888,
					   SDL_TEXTUREACCESS_STREAMING,
					   SCREEN_WIDTH,
					   SCREEN_HEIGHT
					   );
  int point_counter = 0;
  // Start loading array of vectors
  // from -1 to 1 (in 9x9x9 cube)
  for(float x = -1; x <= 1; x += 0.25){
    for(float y = -1; y <= 1; y += 0.25){
      for(float z = -1; z <= 1; z += 0.25){
	vec3_t new_point = { .x = x, .y = y, .z = z };
	cube_points[point_counter++] = new_point;
      }
    }
  }

  
}

bool process_input(void)
{
  SDL_Event event;
  SDL_PollEvent(&event);

  switch(event.type){
  case SDL_QUIT:
    return false;
    break;
  }

  const Uint8 *state = SDL_GetKeyboardState(NULL);
  
  if (state[SDL_SCANCODE_Q]) {
    return false;
  }

  return true;
}

// Orthographic Projection
vec2_t orth_project(vec3_t point)
{
  vec2_t projected_point = {
    .x = (fov_factor * point.x),
    .y = (fov_factor * point.y)
  };
  
  return projected_point;
}

void update(void)
{
  for(int i = 0; i < N_POINTS; i++){
    vec3_t point = cube_points[i];

    // Project the current point
    vec2_t projected_point = orth_project(point);

    // Save the projected 2D vector in the array of projected points
    projected_points[i] = projected_point;
  }
}

void clear_color_buffer(uint32_t color)
{
  for(int y = 0; y < SCREEN_HEIGHT; y++){
    for(int x = 0; x < SCREEN_WIDTH; x++){
      color_buffer[(SCREEN_WIDTH * y) + x] = color;
    }
  }
}

void render_color_buffer(void)
{
  SDL_UpdateTexture(
		    color_buffer_texture,
		    NULL,
		    color_buffer,
		    (SCREEN_WIDTH * sizeof(uint32_t))
		    );

  SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
  
}

void draw_grid(uint32_t color)
{
  for(int y = 0; y < SCREEN_HEIGHT; y += 20){
    for(int x = 0; x < SCREEN_WIDTH; x += 20){
      if(x % 50 == 0 || y % 50 == 0){
	color_buffer[(SCREEN_WIDTH * y) + x] = color;      
      }
    }
  }
}

/* void draw_rect(int cord_x, int cord_y, int width, int height, uint32_t color) */
/* { */
/*   for(int y = cord_y; y < height; y++){ */
/*     for(int x = cord_x; x < width; x++){ */
/*       draw_pixel(x, y, YELLOW); */
/*     } */
/*   } */
/* } */


void draw_rect(int x, int y, int width, int height, uint32_t color) {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            int current_x = x + i;
            int current_y = y + j;
            draw_pixel(current_x, current_y, color);
        }
    }
}

void draw_pixel(int x, int y, uint32_t color)
{
  if(x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT){
    color_buffer[(SCREEN_WIDTH * y) + x] = color;
  }
}

void render(void)
{
  //draw_grid(0xFFFFFF);
  
  //SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // RGBA // A = 255 = Full color
  //SDL_RenderClear(renderer);
  for(int i = 0; i < N_POINTS; i++){
    vec2_t projected_point = projected_points[i];
    draw_rect(projected_point.x + (SCREEN_WIDTH / 2), projected_point.y + (SCREEN_HEIGHT / 2), 4, 4, YELLOW);
  }
  
  
  //draw_rect(10, 10, 300, 150, WHITE);
  //draw_pixel(960, 540, YELLOW);

  
  render_color_buffer();
  clear_color_buffer(BLACK);

  SDL_RenderPresent(renderer);
}

void destroy_window(void)
{
  free(color_buffer);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
