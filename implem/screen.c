#include "ei_types.h"
#include "ei_widgetclass.h"

ei_surface_t pick_surface = NULL;
int *pixel_0_pick = NULL;
int pick_surface_width = 0;

void init_pick_surface(ei_surface_t root_window, ei_size_t main_window_size) {
    pick_surface = hw_surface_create(root_window, main_window_size, false);
    hw_surface_lock(pick_surface);
    pixel_0_pick = (int *) hw_surface_get_buffer(pick_surface);
    hw_surface_unlock(pick_surface);
    pick_surface_width = main_window_size.width;
}

ei_surface_t get_pick_surface() {
    return pick_surface;
}

int *get_pixel_0_pick() {
    return pixel_0_pick;
}

int get_pick_surface_width() {
    return pick_surface_width;
}

void free_pick_surface() {
    hw_surface_free(pick_surface);
}
