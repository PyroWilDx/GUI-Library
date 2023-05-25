#ifndef SCREEN_H
#define SCREEN_H

#include "ei_types.h"
#include "ei_widgetclass.h"


/**
 * \brief	Creates a surface of the same size as the root window on which the pick colors will be drawn.
 *
 * @param	root_window The \ref root_window.
 * @param	main_window_size The size of the root_window.
 */
void init_pick_surface(ei_surface_t root_window, ei_size_t main_window_size);


/**
 * \brief	Returns the pick surface.
 *
 * @return Returns the pick surface.
 */
ei_surface_t get_pick_surface();


/**
 * \brief	Returns the address of the first pixel of the pick surface.
 *
 * @return     Returns the address of the first pixel of the pick surface.
 */
int *get_pixel_0_pick();


/**
 * \brief	Returns the width of the pick surface.
 *
 * @return     Returns the width of the pick surface.
 */
int get_pick_surface_width();


/**
 * \brief	Frees the memory allocated for the pick surface.
 */
void free_pick_surface();

#endif
