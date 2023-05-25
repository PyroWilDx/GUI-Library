#ifndef LSC_H
#define LSC_H

#include "ei_draw.h"

/**
 * @brief	Simple linked list for tc.
 */
struct lsc {
    int y_max;
    int x_ymin;
    int dx;
    int dy;
    struct lsc *next;
};


/**
 * @brief	Simple linked list for tca.
 */
struct lsc_tca {
    int y_max;
    int x_inter;
    int dx;
    int dy;
    int e;
    int sign_dx;
    struct lsc_tca *next;
};


/**
 * \brief	creates a tc linked list cell.
 *
 * @param	y_max	The y_max of the new cell.
 * @param	x_ymin  The x_ymin of the new cell.
 * @param	dx  The dx of the new cell.
 * @param	dy  The dy of the new cell.
 *
 * @return	returns the created cell.
 */
struct lsc *create_lsc(int y_max, int x_ymin, int dx, int dy);


/**
 * \brief	inserts an element at the head of a tc linked list.
 *
 * @param	lsc     The tc linked list to insert the new cell.
 * @param	y_max	The y_max of the new cell.
 * @param	x_ymin  The x_ymin of the new cell.
 * @param	dx  The dx of the new cell.
 * @param	dy  The dy of the new cell.
 *
 * @return	returns the new head of the tc linked list.
 */
struct lsc *insert_lsc(struct lsc *lsc, int y_max, int x_ymin, int dx, int dy);


/**
 * \brief	creates an array linked list from a point array (the i-th element of the array is the linked list of
 * the sides whose minimum height is i).
 *
 * @param	point_array     The point array from which the tc linked list array is created.
 * @param	point_array_size    The number of elements in the point_array array.
 * @param	size    The size of the created array of tc linked list.
 *
 * @return	returns the created array of tc linked list.
 */
struct lsc **get_lsc_tc(ei_point_t *point_array,
                        size_t point_array_size, ei_size_t size);


/**
 * \brief	frees all elements of a tc linked list.
 *
 * @param	lsc     The tc linked list to free.
 */
void free_lsc(struct lsc *lsc);


/**
 * \brief	frees an array of tc linked list.
 *
 * @param	tc     The array of tc linked list to free.
 * @param	size    The size of the tc linked list array.
 */
void free_tc(struct lsc **tc, size_t size);


/**
 * \brief	inserts a new cell in an tca linked list at the index that it should be.
 *
 * @param	lsc_tca     The tca linked list in which the cell is inserted.
 * @param	lsc     A cell of a tc linked list that contains values to create the
 *                  new cell and determine the index for that cell in the tca linked list.
 *
 * @return	returns the head of the tca linked list with the new cell.
 */
struct lsc_tca *insert_lsc_tca(struct lsc_tca *lsc_tca, struct lsc *lsc);


/**
 * \brief	initializes the tca with the first(s) active(s) side(s) (The first(s) one(s) met when browsing the scanlines)
 *
 * @param	tc    The array of linked lists.
 * @param	y    The function stores in this address the height of the first active(s) side(s).
 * @param	max_height    The height not to cross.
 *
 * @return	Returns the tca initiated.
 */
struct lsc_tca *init_tca(struct lsc **tc, int *y, int max_height);


/**
 * \brief	removes a cell of a tca linked list, the cell has to be in the linked list.
 *
 * @param	tca     The tca linked list in which the element should be removed.
 * @param	tca_to_remove    The cell that should be removed from the tca linked list.
 *
 * @return	returns the head of the tca linked list with the element removed.
 */
struct lsc_tca *remove_lsc_tca(struct lsc_tca *tca, struct lsc_tca *tca_to_remove);


/**
 * \brief	frees all cells of a tca linked lit
 *
 * @param	tca     The tca linked list that should be freed.
 */
void free_lsc_tca(struct lsc_tca *tca);


/**
 * \brief	inserts a new cell at the head of a linked rect list.
 *
 * @param	lsc     The linked rect list in which the new cell is inserted.
 * @param	rect    An ei_rect_t that is used to create the new cell.
 *
 * @return	returns the new head of the linked rect list.
 */
ei_linked_rect_t *insert_linked_rect(ei_linked_rect_t *lsc, ei_rect_t *rect);


/**
 * \brief	checks if an ei_rect_t is in a linked rect list.
 *
 * @param	rect    The ei_rect_t to check.
 * @param	lsc     The linked rect list in which we check if \ref rect is in.
 *
 * @return	returns the new head of the linked rect list.
 */
bool rect_in_rect_of_linked_rect(ei_rect_t *rect, ei_linked_rect_t *lsc);


/**
 * \brief	frees all cells of a linked rect list, sets the value of the parameter to NULL.
 *
 * @param	lsc     A pointer to the address of the first cell of the linked rect list. *lsc is set to
 *                  NULL at the end of the function.
 */
void free_linked_rect(ei_linked_rect_t **lsc);

#endif
