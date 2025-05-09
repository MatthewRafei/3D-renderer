#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "include/display.h"
#include "include/vector.h"

bool is_running = false;

int main(void)
{
  /* Create SDL window */
  is_running = initialize_window();

  vec3_t myvec = {2.0, 3.0, -4.0};

  (void) myvec;
  
  setup();
  
  while(is_running){
    is_running = process_input();
    update();
    render();
  }
  
  destroy_window();
  
  return 0;
}
