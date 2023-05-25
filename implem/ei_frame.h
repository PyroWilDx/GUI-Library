#ifndef EI_FRAME_H
#define EI_FRAME_H

#include "ei_types.h"
#include "ei_draw.h"
#include "ei_widget.h"
#include "ei_implementation.h"
#include "utils_draw.h"

/**
 * \brief	Creates the only instance of widget_class describing the frame widget.
 *
 */
ei_widgetclass_t *ei_create_frame_wc();



/**
 * \brief	Allocate the memory needed for frame.
 *
 * @return  Return the widget associated to the frame created.
 */
ei_widget_t ei_alloc_frame();



/**
 * \brief	Free the memory allocated for the frame, and if the active widget is this frame, sets the active widget to NULL.
 */
void ei_release_frame(ei_widget_t widget);



/**
 * \brief	Draws the frame.
 *
 * @param	button 	The frame to draw.
 * @param	surface 	The surface on which to draw the button (it will be the \ref root_surface).
 * @param	pick_surface    The \ref pick_surface.
 * @param	clipper		If not NULL, the drawing is restricted within this rectangle.
 */
void ei_draw_frame(ei_widget_t widget,
                   ei_surface_t surface,
                   ei_surface_t pick_surface,
                   ei_rect_t *clipper);



/**
 * \brief	Sets the frame to its default values.
 *
 * @param	widget  The frame to set
 */
void ei_set_defaults_frame(ei_widget_t widget);




/**
 * \brief   Notifies the frame that its geometry has changed, and thus changes the geometries of its children.
 *
 * @param widget    The frame to notify.
 */
void ei_geomnotify_frame(ei_widget_t widget);




/**
 * \brief   Is called when the frame is handled by the user's event to change it's behaviour.
 *
 * @param widget    The frame handled.
 * @param event     The event handling the frame.
 * @return  Returns true if the frame needs to be updated.
 */
bool ei_handle_frame(ei_widget_t widget, struct ei_event_t *event);



/**
 *\brief    A structure that stores information about a frame.
 */
typedef struct ei_impl_frame_t {
    ei_impl_widget_t widget;

    char text[MAX_TEXT_LENGTH];
    ei_font_t text_font;
    ei_color_t text_color;
    ei_color_t color;
    ei_anchor_t text_anchor;
    ei_anchor_t img_anchor;
    ei_relief_t relief;
    char img_filename[MAX_TEXT_LENGTH];
    ei_surface_t img;
    ei_rect_t img_rect;
    int border_width;
} ei_impl_frame_t;

#endif
