#ifndef EI_ENTRY_H
#define EI_ENTRY_H

#include "ei_types.h"
#include "ei_draw.h"
#include "ei_widget.h"
#include "ei_implementation.h"
#include "utils_draw.h"


#define MAX_ENTRY_LENGTH 512

/**
 * \brief	Creates the only instance of widget_class describing the button widget.
 */
ei_widgetclass_t *ei_create_entry_wc();


/**
 * \brief	Allocate the memory needed for a button.
 *
 * @return  Return the address of the button created.
 */
ei_widget_t ei_alloc_entry();

/**
 * \brief	Free the memory allocated for the button, and if the active widget is this button, sets the active widget to NULL.
 */
void ei_release_entry(ei_widget_t widget);


/**
 * \brief	Draws the button.
 *
 * @param	button 	The button to draw.
 * @param	surface 	The surface on which to draw the button (it will be the \ref root_surface usually).
 * @param	pick_surface The \ref pick_surface.
 * @param	clipper		To clip the button.
 */
void ei_draw_entry(ei_widget_t widget,
                    ei_surface_t surface,
                    ei_surface_t pick_surface,
                    ei_rect_t *clipper);


/**
 * \brief	Sets the button to its default values.
 *
 * @param	widget 	The button to set.
 */
void ei_set_defaults_entry(ei_widget_t widget);


/**
 * \brief	Notifies the button that its geometry has changed, and thus changes the geometries of its children.
 *
 * @param	widget The button to notify.
 */
void ei_geomnotify_entry(ei_widget_t widget);


/**
 * \brief	Is called when the button is handled by the user's event to change its behaviour.
 *
 * @param	widget The button handled.
 * @param	event The event handling the button.
 *
 * @return   Returns true if the button needs to be updated.
 */
bool ei_handle_entry(ei_widget_t widget, struct ei_event_t *event);


void ei_entry_configure(ei_widget_t widget,
                        ei_size_t *requested_size,
                        ei_color_t *text_color,
                        int *border_width,
                        ei_font_t *text_font,
                        ei_user_param_t *user_param);


/**
 * \brief	 updates the active entry when the key TAB is pressed.
 */
void update_active_entry();


/**
 * \brief	sets the text for an entry.
 *
 * @param	widget  The entry.
 * @param   text    The text.
 *
 */
void set_entry_text(ei_widget_t widget, char *text);


/**
 * \brief	returns the text in the entry.
 *
 * @param	widget The entry.
 *
 * @return   returns the text in the entry.
 */
char *get_entry_text(ei_widget_t widget);


/**
 * \brief	A structure that stores information about a button.
 */
typedef struct ei_impl_entry_t {
    ei_impl_widget_t widget;

    char text[MAX_ENTRY_LENGTH];
    ei_font_t text_font;
    ei_color_t text_color;
    int border_width;
} ei_impl_entry_t;


/**
 * \brief	A linked list of entries.
 */
typedef struct active_entries_t {
    ei_widget_t widget;
    struct active_entries_t *next;
} active_entries_t;

#endif
