#include "ei_placer.h"
#include "ei_implementation.h"
#include "ei_widget_attributes.h"
#include "screen.h"
#include "ei_application.h"
#include "ei_toplevel.h"

void ei_place(ei_widget_t widget,
              ei_anchor_t *anchor,
              int *x,
              int *y,
              int *width,
              int *height,
              float *rel_x,
              float *rel_y,
              float *rel_width,
              float *rel_height) {

    ei_widget_t parent = ei_widget_get_parent(widget);
    ei_rect_t old_rect = widget->screen_location;
    ei_app_invalidate_rect(&old_rect);

    ei_anchor_t real_anchor = (anchor == NULL) ? ei_anc_northwest : *anchor;

    const ei_rect_t *parent_loc = parent->content_rect;

    widget->screen_location.top_left.x = parent_loc->top_left.x;
    widget->screen_location.top_left.y = parent_loc->top_left.y;
    widget->screen_location.size.width = 0;
    widget->screen_location.size.height = 0;

    if (rel_x != NULL) {
        widget->screen_location.top_left.x += (int) ((float) parent_loc->size.width * (*rel_x));
        widget->rel_x = *rel_x;
    }
    if (rel_y != NULL) {
        widget->screen_location.top_left.y += (int) ((float) parent_loc->size.height * (*rel_y));
        widget->rel_y = *rel_y;
    }
    if (x != NULL) {
        widget->screen_location.top_left.x += *x;
        widget->rel_x =
                (float) (widget->screen_location.top_left.x - parent_loc->top_left.x) / (float) parent_loc->size.width;
    }
    if (y != NULL) {
        widget->screen_location.top_left.y += *y;
        widget->rel_y =
                (float) (widget->screen_location.top_left.y - parent_loc->top_left.y) / (float) parent_loc->size.height;
    }
    if (rel_width != NULL) {
        int add_w = (int) ((float) parent_loc->size.width * (*rel_width));
        widget->screen_location.size.width += add_w;
        widget->rel_width = *rel_width;
    }
    if (rel_height != NULL) {
        int add_h = (int) ((float) parent_loc->size.height * (*rel_height));
        widget->screen_location.size.height += add_h;
        widget->rel_height = *rel_height;
    }
    if (width != NULL) {
        widget->screen_location.size.width += *width;
        widget->rel_width = (float) widget->screen_location.size.width / (float) parent->content_rect->size.width;
    }
    if (height != NULL) {
        widget->screen_location.size.height += *height;
        widget->rel_height = (float) widget->screen_location.size.height / (float) parent->content_rect->size.height;
    }

    if (rel_width == NULL && width == NULL) {
        widget->screen_location.size.width = widget->requested_size.width;
    }

    if (rel_height == NULL && height == NULL) {
        widget->screen_location.size.height = widget->requested_size.height;
    }

    widget->requested_size.width = widget->screen_location.size.width;
    widget->requested_size.height = widget->screen_location.size.height;

    switch (real_anchor) {
        case ei_anc_center:
            widget->screen_location.top_left.x -= widget->screen_location.size.width / 2;
            widget->screen_location.top_left.y -= widget->screen_location.size.height / 2;
            widget->rel_x -= widget->rel_width / 2;
            widget->rel_y -= widget->rel_height / 2;
            break;
        case ei_anc_north:
            widget->screen_location.top_left.x -= widget->screen_location.size.width / 2;
            widget->rel_x -= widget->rel_width / 2;
            break;
        case ei_anc_northeast:
            widget->screen_location.top_left.x -= widget->screen_location.size.width;
            widget->rel_x -= widget->rel_width;
            break;
        case ei_anc_east:
            widget->screen_location.top_left.x -= widget->screen_location.size.width;
            widget->screen_location.top_left.y -= widget->screen_location.size.height / 2;
            widget->rel_x -= widget->rel_width;
            widget->rel_y -= widget->rel_height / 2;
            break;
        case ei_anc_southeast:
            widget->screen_location.top_left.x -= widget->screen_location.size.width;
            widget->screen_location.top_left.y -= widget->screen_location.size.height;
            widget->rel_x -= widget->rel_width;
            widget->rel_y -= widget->rel_height;
            break;
        case ei_anc_south:
            widget->screen_location.top_left.x -= widget->screen_location.size.width / 2;
            widget->screen_location.top_left.y -= widget->screen_location.size.height;
            widget->rel_x -= widget->rel_width / 2;
            widget->rel_y -= widget->rel_height;
            break;
        case ei_anc_southwest:
            widget->screen_location.top_left.y -= widget->screen_location.size.height;
            widget->rel_y -= widget->rel_height;
            break;
        case ei_anc_west:
            widget->screen_location.top_left.y -= widget->screen_location.size.height / 2;
            widget->rel_y -= widget->rel_height / 2;
            break;
        default:
            break;
    }

    widget->wclass->geomnotifyfunc(widget);

    ei_widget_t child = widget->children_head;
    if (strcmp(widget->wclass->name, "toplevel") == 0) {
        child = child->next_sibling;
    }
    while (child != NULL) {
        if (strcmp(child->wclass->name, "toplevel") == 0 || strcmp(child->wclass->name, "button") == 0) {
        ei_place(child, NULL, NULL, NULL, NULL, NULL,
                 &child->rel_x, &child->rel_y,
                 NULL, NULL);
        } else {
            ei_place(child, NULL, NULL, NULL, NULL, NULL,
                     &child->rel_x, &child->rel_y,
                     &child->rel_width, &child->rel_height);
        }
        child = child->next_sibling;
    }
}

void ei_placer_forget(ei_widget_t widget) {
    widget->placer_params = NULL;
}
