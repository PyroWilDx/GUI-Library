#ifndef UTILS_WIDGET_H
#define UTILS_WIDGET_H

#include "ei_widget.h"


/**
 * \brief	releases a widget, browses through all the children of a widget and calls their releasefunc.
 *
 * @param	widget      The widget.
 */
void release_widget(ei_widget_t widget);


/**
 *\brief    the number of children of a widget.
 *
 * @param widget    The widget to calculate its children.
 * @return      The number of children.
 */
int get_children_number(ei_widget_t widget);


/**
 * \brief	calculates the clipper of a widget before drawing it. It does all the rectangle
 *          intersections that it should do.
 *
 * @param	widget    The widget.
 * @param	rect     An ei_rect_t to intersects the clipper with. (this rectangle should be
 *                   a rectangle that we added through \ref ei_app_invalidate_rect)
 *
 * @return	returns the calculated clipper.
 */
ei_rect_t get_widget_clipper(ei_widget_t widget, ei_rect_t *rect);


/**
 * \brief   calculates the final rect where to place the text/image linked to the widget.
 *
 * @param widget    The widget containing the text.
 * @param rect      The original rect of the entire text/image
 * @param outside   If the text/image is outside of the content rect of the widget.
 *
 * @return      returns the final rect of the text/image to display.
 */
ei_rect_t get_text_img_intersect_rect(ei_widget_t widget, ei_rect_t *rect, bool outside);


/**
 * \brief   Returns the first toplevel ancestor if there is one, else returns NULL.
 *
 * @param widget    The widget.
 */
ei_widget_t search_first_toplevel_parent(ei_widget_t widget);


/**
 * \brief   sets the toplevel in front of its siblings.
 *
 * @param widget    The toplevel.
 *
 * @return   returns the ei_rect_t to invalidate.
 */
ei_rect_t set_front_widget(ei_widget_t widget);

#endif
