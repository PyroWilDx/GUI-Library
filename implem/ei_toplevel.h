#ifndef EI_TOPLEVEL_H
#define EI_TOPLEVEL_H

#include "ei_types.h"
#include "ei_draw.h"
#include "ei_widget.h"
#include "ei_implementation.h"
#include "utils_draw.h"
#include "ei_event.h"


/**
 * \brief	Creates the only instance of widget_class describing the toplevel widget.
 */
ei_widgetclass_t *ei_create_toplevel_wc();


/**
 * \brief	Allocate the memory needed for a toplevel.
 * @return  Return the address of the toplevel created.
 */
ei_widget_t ei_alloc_toplevel();


/**
 * \brief	Free the memory allocated for the toplevel, and if the active widget is this toplevel, sets the active widget to NULL.
 */
void ei_release_toplevel(ei_widget_t widget);


/**
 * \brief	Draws the toplevel.
 *
 * @param	toplevel 	The toplevel to draw.
 * @param	surface 	The surface on which to draw the toplevel (it will be the \ref root_surface usually).
 * @param	pick_surface The \ref pick_surface.
 * @param	clipper		To clip the toplevel.
 */
void ei_draw_toplevel(ei_widget_t toplevel,
                      ei_surface_t surface,
                      ei_surface_t pick_surface,
                      ei_rect_t *clipper);


/**
 * \brief	Sets the toplevel to its default values.
 *
 * @param	widget 	The toplevel to set.
 */
void ei_set_defaults_toplevel(ei_widget_t widget);


/**
 * \brief	Notifies the toplevel that its geometry has changed, and thus changes the geometries of its children.
 *
 * @param	widget The toplevel to notify.
 */
void ei_geomnotify_toplevel(ei_widget_t widget);


/**
 * \brief	Is called when the toplevel is handled by the user's event to change its behaviour.
 *
 * @param	widget The toplevel handled.
 * @param	event The event handling the toplevel.
 * @return Returns true if the toplevel needs to be updated.
 */
bool ei_handle_toplevel(ei_widget_t widget, struct ei_event_t *event);


/**
 * \brief	A structure that stores information about a toplevel.
 */
typedef struct ei_impl_toplevel_t {
    ei_impl_widget_t widget;

    char title[MAX_TEXT_LENGTH];
    ei_color_t color;
    bool closable;
    ei_axis_set_t resizable;
    ei_size_t *min_size;
    int border_width;
    ei_point_t rel_mouse_pos;
    ei_rect_t resize_square_location;
    bool resizing;
    bool translating;
    ei_widget_t close_button;
} ei_impl_toplevel_t;

#endif
