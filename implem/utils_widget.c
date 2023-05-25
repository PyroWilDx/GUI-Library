#include "utils_widget.h"
#include "ei_event.h"
#include "ei_application.h"
#include "ei_button.h"
#include "utils_general.h"

void release_widget(ei_widget_t widget) {
    if (widget == ei_event_get_active_widget()) {
        ei_event_set_active_widget(NULL);
    }

    free(widget->pick_color);
    if (!rect1_equal_rect2(&widget->screen_location, widget->content_rect)) {
        free(widget->content_rect);
    }

    ei_rect_t rect = widget->screen_location;
    ei_app_invalidate_rect(&rect);

    if (widget->placer_params != NULL) {
        widget->placer_params = NULL;

        // release all children
        ei_widget_t first_child = widget->children_head;
        if (first_child != NULL) {
            first_child->placer_params = NULL;
            first_child->wclass->releasefunc(first_child);
        }

        // update parent children and current widget siblings
        ei_widget_t parent = widget->parent;
        if (parent->children_head == widget) {
            parent->children_head = widget->next_sibling;
            if (widget->next_sibling == NULL) {
                parent->children_tail = NULL;
            }
        } else {

            ei_widget_t tmp = parent->children_head;
            if (tmp != NULL) {
                while (tmp->next_sibling != NULL && tmp->next_sibling != widget) {
                    tmp = tmp->next_sibling;
                }
                tmp->next_sibling = widget->next_sibling;
                if (tmp->next_sibling == NULL) {
                    parent->children_tail = tmp;
                }
            }
        }

    } else {
        ei_widget_t next_sibling = widget->next_sibling;
        ei_widget_t first_child = widget->children_head;
        if (next_sibling != NULL) {
            next_sibling->placer_params = NULL;
            next_sibling->wclass->releasefunc(next_sibling);
        }
        if (first_child != NULL) {
            first_child->placer_params = NULL;
            first_child->wclass->releasefunc(first_child);
        }
    }
}

int get_children_number(ei_widget_t widget) {
    ei_widget_t child = widget->children_head;
    int nb = 1;
    while (child != NULL) {
        nb += get_children_number(child);
        child = child->next_sibling;
    }
    return nb;
}

ei_rect_t get_widget_clipper(ei_widget_t widget, ei_rect_t *rect) {
    ei_rect_t clipper = intersect_rect(rect, &widget->screen_location);

    ei_widget_t current_parent = widget->parent;

    if (strcmp(widget->wclass->name, "button") == 0) {
        struct ei_impl_button_t *button = (struct ei_impl_button_t *) widget;
        if (button->outside) {
            clipper = intersect_rect(&clipper, &current_parent->screen_location);
            current_parent = current_parent->parent;
        }
    }

    while (current_parent != NULL) {
        clipper = intersect_rect(&clipper, current_parent->content_rect);
        current_parent = current_parent->parent;
    }
    return clipper;
}

ei_rect_t get_text_img_intersect_rect(ei_widget_t widget, ei_rect_t *rect, bool outside) {
    ei_rect_t res_rect = intersect_rect(&widget->screen_location, rect);

    if (!outside) {
        res_rect = intersect_rect(&res_rect, widget->content_rect);
    } else {
        res_rect = intersect_rect(&res_rect, &widget->screen_location);
    }

    ei_widget_t current_parent = widget->parent;
    while (current_parent != NULL) {
        res_rect = intersect_rect(&res_rect, current_parent->content_rect);
        current_parent = current_parent->parent;
    }
    return res_rect;
}

ei_widget_t search_first_toplevel_parent(ei_widget_t widget) {
    if (strcmp(widget->wclass->name, "toplevel") == 0) return widget;
    ei_widget_t current_parent = widget->parent;
    while (current_parent != NULL) {
        if (strcmp(current_parent->wclass->name, "toplevel") == 0) return current_parent;
        current_parent = current_parent->parent;
    }
    return NULL;
}

ei_rect_t set_front_widget(ei_widget_t widget_tmp) {
    ei_rect_t invalidate_rect = {{0, 0}, {0, 0}};
    ei_widget_t widget = search_first_toplevel_parent(widget_tmp);
    while (widget != NULL) {
        ei_widget_t parent = widget->parent;
        if (parent->children_tail != widget) {
            if (widget == parent->children_head) {
                parent->children_head = widget->next_sibling;
            } else {
                ei_widget_t tmp_sibling = parent->children_head;
                while (tmp_sibling->next_sibling != widget) {
                    tmp_sibling = tmp_sibling->next_sibling;
                }
                tmp_sibling->next_sibling = widget->next_sibling;
            }
            parent->children_tail->next_sibling = widget;
            parent->children_tail = widget;
            widget->next_sibling = NULL;
            invalidate_rect = widget->screen_location;
        }
        widget = search_first_toplevel_parent(widget->parent);
    }
    return invalidate_rect;
}
