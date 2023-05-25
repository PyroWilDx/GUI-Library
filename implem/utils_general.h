#ifndef UTILS_GENERAL_H
#define UTILS_GENERAL_H

#include "ei_draw.h"
#include "ei_types.h"


/**
 * \brief   Swaps the elements.
 *
 * @param a     Address of the element to swap
 * @param b     with the other element at this address.
 */
void swap(int *a, int *b);


/**
 * \brief   returns the minimum between two integers.
 *
 * @param a     First integer to be compared
 * @param b     with the second one.
 *
 * @return      The minimum between a and b.
 */
static inline int min(int a, int b) {
    return a < b ? a : b;
}


/**
 * \brief   returns the maximum between two integers.
 *
 * @param a     First integer to be compared
 * @param b     with the second one.
 *
 * @return      The maximum between a and b.
 */
static inline int max(int a, int b) {
    return a < b ? b : a;
}


/**
 * \brief   returns the point with the lowest ordinate.
 *
 * @param p0    First point to be compared
 * @param p1    with the second one.
 *
 * @return      p0 if p0's ordinate is less than p1'sor p1's.
 */
static inline ei_point_t get_y_min(ei_point_t p0, ei_point_t p1) {
    return p0.y > p1.y ? p1 : p0;
}


/**
 * \brief   returns the point with the greatest ordinate.
 *
 * @param p0    First point to be compared
 * @param p1    with the second one.
 *
 * @return      p0 if p0's ordinate is greater than p1'sor p1's.
 */
static inline ei_point_t get_y_max(ei_point_t p0, ei_point_t p1) {
    return p0.y > p1.y ? p0 : p1;
}


/**
 *\brief    returns if a pixel is in the clipper.
 *
 * @param x_min     The left side of the clipper.
 * @param y_min     The top side of the clipper.
 * @param x_max     The right side of the clipper.
 * @param y_max     The bottom side of the clipper.
 * @param ptr_pixel     The pixel to test.
 * @param first_pixel   The first pixel of the surface in which this test is being made.
 * @param width     The width of the surface in which this test is being made.
 *
 * @return      1 if the pixel is in the clipper, 0 otherwise.
 */
inline static int pixel_is_in_clipper(int x_min, int y_min,
                                      int x_max, int y_max,
                                      const int *ptr_pixel,
                                      const int *first_pixel,
                                      int width) {
    int tmp = (int) (ptr_pixel - first_pixel);
    int x = tmp % width;
    int y = tmp / width;
    return (x >= x_min) && (x <= x_max) && (y >= y_min) && (y <= y_max);
}


/**
 * \brief   returns if a point is in the clipper
 *
 * @param x_min     The left side of the clipper.
 * @param y_min     The top side of the clipper.
 * @param x_max     The right side of the clipper.
 * @param y_max     The bottom side of the clipper.
 * @param x     The abscissa of the point to test.
 * @param y     The ordinate of the point to test.
 *
 * @return      1 if the point is in the clipper, 0 otherwise.
 */
inline static int point_is_in_clipper(int x_min, int y_min,
                                      int x_max, int y_max,
                                      int x, int y) {
    return (x >= x_min) && (x <= x_max) && (y >= y_min) && (y <= y_max);
}


/**
 *\brief    returns the intersection of two rects.
 *
 * @param rect1     First ei_rect_t address.
 * @param rect2     Second ei_rect_t address.
 *
 * @return      the ei_rect_t resulting from the intersection of rect1 and rect2.
 */
ei_rect_t intersect_rect(ei_rect_t *rect1, ei_rect_t *rect2);


/**
 * \brief   returns if two ei_rect_t intersect or not.
 *
 * @param rect1     First ei_rect_t address.
 * @param rect2     Second ei_rect_t address.
 *
 * @return      True if the intersection of rect1 and rect2 is not null.
 */
bool rect1_intersects_rect2(ei_rect_t *rect1, ei_rect_t *rect2);


/**
 * \brief   returns if the first ei_rect_t is included in the second ei_rect_t.
 *
 * @param rect1     First ei_rect_t address (the one to test if is included).
 * @param rect2     Second ei_rect_t address (the one to test if it contains the first one).
 *
 * @return      True if rect1 is in rect2, false otherwise.
 */
bool rect1_in_rect2(ei_rect_t *rect1, ei_rect_t *rect2);


/**
 * \brief   returns if the ei_rect_t is not a point.
 *
 * @param rect      The ei_rect_t address to test.
 *
 * @return      True if rect is not reduced to a point.
 */
bool rect_not_0(ei_rect_t *rect);


/**
 * \brief   returns if two rectangles are equal or not.
 *
 * @param rect1     The first ei_rect_t address to test.
 * @param rect2     The second ei_rect_t address to test.
 *
 * @return      True if rect1 equals rect2.
 */
inline static bool rect1_equal_rect2(ei_rect_t *rect1, ei_rect_t *rect2) {
    return (rect1->top_left.x == rect2->top_left.x && rect1->top_left.y == rect2->top_left.y &&
            rect1->size.width == rect2->size.width && rect1->size.height == rect2->size.height);
}

#endif
