#include "ei_types.h"
#include "lsc.h"
#include "utils_general.h"


struct lsc *create_lsc(int y_max, int x_ymin, int dx, int dy) {
    struct lsc *lsc = malloc(sizeof(struct lsc));
    lsc->y_max = y_max;
    lsc->x_ymin = x_ymin;
    lsc->dx = dx;
    lsc->dy = dy;
    lsc->next = NULL;
    return lsc;
}

struct lsc *insert_lsc(struct lsc *lsc, int y_max, int x_ymin, int dx, int dy) {
    struct lsc *nv_lsc = create_lsc(y_max, x_ymin, dx, dy);
    nv_lsc->next = lsc;
    return nv_lsc;
}

struct lsc **get_lsc_tc(ei_point_t *point_array,
                        size_t point_array_size, ei_size_t size) {

    struct lsc **tc = calloc(size.height, sizeof(struct lsc *));
    ei_point_t point_y_min;
    int y_min;
    int y_max;
    ei_point_t p0;
    ei_point_t p1;
    size_t i = 0;
    while (i < point_array_size - 1) {
        p0 = point_array[i];
        p1 = point_array[i + 1];
        if (!(p0.x == p1.x && p0.y == p1.y)) {
            point_y_min = get_y_min(p0, p1);
            y_min = point_y_min.y;
            y_max = get_y_max(p0, p1).y;
            int dx = p1.x - p0.x;
            int dy = p1.y - p0.y;
            if (dy != 0) tc[y_min] = insert_lsc(tc[y_min], y_max, point_y_min.x, dx, dy);
        }
        i++;
    }
    p0 = point_array[i];
    p1 = point_array[0];
    if (!(p0.x == p1.x && p0.y == p1.y)) {
        point_y_min = get_y_min(p0, p1);
        y_min = point_y_min.y;
        y_max = get_y_max(p0, p1).y;
        int dx = p1.x - p0.x;
        int dy = p1.y - p0.y;
        if (dy != 0) tc[y_min] = insert_lsc(tc[y_min], y_max, point_y_min.x, dx, dy);
    }
    return tc;
}

void free_lsc(struct lsc *lsc) {
    struct lsc *tmp = lsc;
    struct lsc *next;
    while (tmp != NULL) {
        next = tmp->next;
        free(tmp);
        tmp = next;
    }
}

void free_tc(struct lsc **tc, size_t size) {
    for (size_t i = 0; i < size; i++) {
        free_lsc(tc[i]);
    }
    free(tc);
}

struct lsc_tca *insert_lsc_tca(struct lsc_tca *lsc_tca, struct lsc *lsc) {
    struct lsc_tca *nv_lsc_tca = malloc(sizeof(struct lsc_tca));
    nv_lsc_tca->y_max = lsc->y_max;
    nv_lsc_tca->x_inter = lsc->x_ymin;
    nv_lsc_tca->e = 0;
    nv_lsc_tca->sign_dx = 1;
    nv_lsc_tca->dx = lsc->dx;
    nv_lsc_tca->dy = lsc->dy;

    if (lsc->dx < 0 && lsc->dy < 0) {
        nv_lsc_tca->sign_dx = 1;
        nv_lsc_tca->dx = -lsc->dx;
        nv_lsc_tca->dy = -lsc->dy;
    } else if (lsc->dx >= 0 && lsc->dy < 0) {
        nv_lsc_tca->dy = -lsc->dy;
        nv_lsc_tca->dx = -lsc->dx;
        nv_lsc_tca->sign_dx = -1;
    } else if (lsc->dx < 0 && lsc->dy >= 0) {
        nv_lsc_tca->sign_dx = -1;
    }

    if (abs(nv_lsc_tca->dx) > nv_lsc_tca->dy) {
        if (nv_lsc_tca->dx >= 0) {
            nv_lsc_tca->sign_dx = ceil((double) nv_lsc_tca->dx / nv_lsc_tca->dy);
        } else {
            nv_lsc_tca->sign_dx = floor((double) nv_lsc_tca->dx / nv_lsc_tca->dy);
        }
    }

    if (lsc_tca == NULL) {
        nv_lsc_tca->next = NULL;
        return nv_lsc_tca;
    }

    if (lsc_tca->x_inter >= lsc->x_ymin) {
        nv_lsc_tca->next = lsc_tca;
        return nv_lsc_tca;
    }

    struct lsc_tca *tmp = lsc_tca;
    while (tmp->next != NULL && (tmp->next->x_inter < lsc->x_ymin)) {
        tmp = tmp->next;
    }

    if (tmp->next == NULL) {
        tmp->next = nv_lsc_tca;
        nv_lsc_tca->next = NULL;
        return lsc_tca;
    }

    nv_lsc_tca->next = tmp->next;
    tmp->next = nv_lsc_tca;
    return lsc_tca;
}

struct lsc_tca *init_tca(struct lsc **tc, int *y, int max_height) {
    while ((tc[*y] == NULL || tc[*y]->next == NULL) && *y < max_height) {
        (*y)++;
    }
    struct lsc_tca *tca = NULL;
    struct lsc *lsc = tc[*y];
    while (lsc != NULL) {
        tca = insert_lsc_tca(tca, lsc);
        lsc = lsc->next;
    }
    return tca;
}

struct lsc_tca *remove_lsc_tca(struct lsc_tca *tca, struct lsc_tca *tca_to_remove) {
    if (tca == tca_to_remove) {
        struct lsc_tca *next = tca->next;
        free(tca);
        return next;
    }

    struct lsc_tca *tmp = tca;

    while (tmp->next != tca_to_remove) {
        tmp = tmp->next;
    }
    struct lsc_tca *tmp_free = tmp->next;
    tmp->next = tmp->next->next;
    free(tmp_free);
    return tca;
}

void free_lsc_tca(struct lsc_tca *tca) {
    struct lsc_tca *tmp = tca;
    struct lsc_tca *next;
    while (tmp != NULL) {
        next = tmp->next;
        free(tmp);
        tmp = next;
    }
}

ei_linked_rect_t *insert_linked_rect(ei_linked_rect_t *lsc, ei_rect_t *rect) {
    ei_linked_rect_t *new_cell = malloc(sizeof(ei_linked_rect_t));
    new_cell->rect = *rect;
    new_cell->next = lsc;
    return new_cell;
}

bool rect_in_rect_of_linked_rect(ei_rect_t *rect, ei_linked_rect_t *lsc) {
    ei_linked_rect_t *tmp = lsc;
    while (tmp != NULL) {
        if (rect1_in_rect2(rect, &tmp->rect)) {
            return true;
        }
        tmp = tmp->next;
    }
    return false;
}

void free_linked_rect(ei_linked_rect_t **lsc) {
    ei_linked_rect_t *tmp = *lsc;
    ei_linked_rect_t *tmp_next;
    while (tmp != NULL) {
        tmp_next = tmp->next;
        free(tmp);
        tmp = tmp_next;
    }
    *lsc = NULL;
}
