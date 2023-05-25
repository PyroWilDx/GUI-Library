#ifndef UTILS_DRAW_H
#define UTILS_DRAW_H

#include "ei_types.h"
#include "ei_draw.h"
#include "ei_widget.h"
#include "ei_implementation.h"

// must be even!!!!!!!!!!!
#define PRECISION 40

#define MAX_TEXT_LENGTH 512
#define DEFAULT_BORDER_WIDTH 6


/**
 * \brief	a structure describing the corner of a rectangle.
 */
struct ei_corner_t {
    bool top;
    bool right;
};


/**
 * \brief	an enumeration type for the shape of a rounded rectangle (only the top corners or each corners are rounded) .
 */
typedef enum ei_shape_t {
    top_rounded,
    everywhere_rounded
} ei_shape_t;


/**
 * \brief	return the array of points of an arc.
 *
 * @param	centre The centre of the arc.
 * @param	ray The ray of the arc.
 * @param	corner Which corner of the rectangle the arc is.
 * @param	sense  Equals 1 if the array is in the trigonometric sense and 0 else.
 */
ei_point_t *get_array_arc(ei_point_t centre, int ray, struct ei_corner_t corner, int sense);


/**
 * \brief	return the array of points of a rounded rectangle.
 *
 * @param	rect The centre of the arc.
 * @param	ray The ray of the arc.
 * @param	shape The shape of the rounded frame.
 * @param	clockwise  The order of the corners in the array.
 */
ei_point_t *rounded_frame(ei_rect_t rect, int ray, ei_shape_t shape, int clockwise);


/**
 * \brief	can draw anything you can dream of thanks to its unlimited arguments
 *          (rounded rectangles (only the top corners can be rounded or each or none),
 *          with a border or not, and a relief). ❤️ :)️.
 *
 * @param	surface     The surface on which to draw the thing.
 * @param	pick_surface    The pick_surface
 * @param	ptr_rect    The address of the array containing the 4 points of the rectangle.
 * @param	color       The (main) color of the rectangle to draw.
 * @param	pick_color  The pick color of the widget of which the rectangle is a part.
 * @param	relief  The relief of the rectangle to draw.
 * @param	radius     The radius of the arcs.
 * @param	border_width    The width of the border.
 * @param	clipper     To clip the drawing.
 * @param	shape   The shape (only the top corners are rounded or each corners are (or none if radius==0)).
 * @param   border_color    The color of the border. If not NULL, the default border color is overwritten.
 */
void draw_round_rect(ei_surface_t surface, ei_surface_t pick_surface,
                     ei_rect_t *ptr_rect, ei_color_t color, ei_color_t pick_color,
                     ei_relief_t relief, int radius, int border_width,
                     ei_rect_t *clipper, ei_shape_t shape, ei_color_t *border_color);


/**
 * \brief	draws an image on a surface with \ref ei_copy_surface. The image can be from a filename
 *          or directly a surface containing the image.
 *
 * @param   surface     The surface.
 * @param	where    The top left point of where the image should be drawn of the surface.
 * @param	img_filename    The filename of the image (path to the image).
 * @param	img    The surface containing the image. If NULL, then \ref img_filename should have a valid
 *                 path to an image. Else we take this image and we ignore \ref img_filename.
 * @param	clipper     The clipper to clip the image.
 */
void ei_draw_image(ei_surface_t surface,
                   const ei_point_t *where,
                   ei_const_string_t img_filename,
                   ei_surface_t img,
                   ei_rect_t *clipper);


/**
 *\brief    returns the exact point in the widget where the image must be displayed.
 *
 * @param top_left      The widget's top_left
 * @param widget_width  The width of the widget containing the image.
 * @param widget_height     The height of the widget containing the image
 * @param img_width     The image's width
 * @param img_height    The image's height
 * @param anchor    The image anchor
 * @return      ei_point_t where to display the image
 */
ei_point_t get_where_img(ei_point_t top_left, int widget_width, int widget_height, int img_width, int img_height,
                         ei_anchor_t anchor);


/**
 *\brief    returns the most little rectangle containing a text anchored on the widget.
 *
 * @param widget    The widget on which to anchor text.
 * @param text  The text.
 * @param font  The font.
 * @param text_anchor   The anchor.
 */
ei_rect_t get_text_rect(ei_widget_t widget, char *text, ei_font_t font, ei_anchor_t text_anchor);


/**
 *\brief    returns the address of a point on a surface,
 *          knowing the coordinates of the point and the address of the first pixel of the surface.
 *
 * @param pixel_0    The address of the first pixel of the surface.
 * @param x     The abscissa of the point.
 * @param y     The ordinate of the point.
 * @param width The width of the considered surface.
 */
inline static int *get_pixel_from_coordinates(int *pixel_0, int x, int y, int width) {
    int *ptr_pixel = pixel_0 + x + y * width;
    return ptr_pixel;
}

#endif
