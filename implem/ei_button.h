#ifndef EI_BUTTON_H
#define EI_BUTTON_H

#include "ei_types.h"
#include "ei_draw.h"
#include "ei_widget.h"
#include "ei_implementation.h"
#include "utils_draw.h"


/**
 * \brief	Creates the only instance of widget_class describing the button widget.
 */
ei_widgetclass_t *ei_create_button_wc();


/**
 * \brief	Allocate the memory needed for a button.
 * @return  Return the address of the button created.
 */
ei_widget_t ei_alloc_button();

/**
 * \brief	Free the memory allocated for the button, and if the active widget is this button, sets the active widget to NULL.
 */
void ei_release_button(ei_widget_t widget);


/**
 * \brief	Draws the button.
 *
 * @param	button 	The button to draw.
 * @param	surface 	The surface on which to draw the button (it will be the \ref root_surface usually).
 * @param	pick_surface The \ref pick_surface.
 * @param	clipper		To clip the button.
 */
void ei_draw_button(ei_widget_t button,
                    ei_surface_t surface,
                    ei_surface_t pick_surface,
                    ei_rect_t *clipper);


/**
 * \brief	Sets the button to its default values.
 *
 * @param	widget 	The button to set.
 */
void ei_set_defaults_button(ei_widget_t widget);


/**
 * \brief	Notifies the button that its geometry has changed, and thus changes the geometries of its children.
 *
 * @param	widget The button to notify.
 */
void ei_geomnotify_button(ei_widget_t widget);


/**
 * \brief	Is called when the button is handled by the user's event to change its behaviour.
 *
 * @param	widget The button handled.
 * @param	event The event handling the button.
 * @return Returns true if the button needs to be updated.
 */
bool ei_handle_button(ei_widget_t widget, struct ei_event_t *event);


/**
 * \brief	A structure that stores information about a button.
 */
typedef struct ei_impl_button_t {
    ei_impl_widget_t widget;

    char text[MAX_TEXT_LENGTH];
    ei_font_t text_font;
    ei_color_t text_color;
    ei_color_t color;
    bool clicked;
    int border_width;
    ei_anchor_t text_anchor;
    ei_anchor_t img_anchor;
    char img_filename[MAX_TEXT_LENGTH];
    ei_surface_t img;
    ei_rect_t img_rect;
    ei_callback_t callback;
    int corner_radius;
    bool outside;
} ei_impl_button_t;

#endif
