#include"ei_widget_attributes.h"
#include "ei_implementation.h"
#include "ei_widgetclass.h"
#include "utils_general.h"


ei_widgetclass_t *ei_widget_get_class(ei_widget_t widget) {
    return widget->wclass;
}

const ei_color_t *ei_widget_get_pick_color(ei_widget_t widget) {
    return widget->pick_color;
}

ei_widget_t ei_widget_get_parent(ei_widget_t widget) {
    return widget->parent;
}

ei_widget_t ei_widget_get_first_child(ei_widget_t widget) {
    return widget->children_head;
}

ei_widget_t ei_widget_get_last_child(ei_widget_t widget) {
    return widget->children_tail;
}

ei_widget_t ei_widget_get_next_sibling(ei_widget_t widget) {
    return widget->next_sibling;
}

void *ei_widget_get_user_data(ei_widget_t widget) {
    return widget->user_data;
}

const ei_rect_t *ei_widget_get_screen_location(ei_widget_t widget) {
    return &(widget->screen_location);
}

const ei_rect_t *ei_widget_get_content_rect(ei_widget_t widget) {
    return widget->content_rect;
}

void ei_widget_set_content_rect(ei_widget_t widget,
                                const ei_rect_t *content_rect) {

    if (rect1_equal_rect2(widget->content_rect, (ei_rect_t *) content_rect)) return;

    if (!rect1_equal_rect2(&widget->screen_location, widget->content_rect)) {
        free(widget->content_rect);
    }

    if (rect1_equal_rect2(&widget->screen_location, (ei_rect_t *) content_rect)) {
        widget->content_rect = &widget->screen_location;
    } else {
        ei_rect_t *rect = malloc(sizeof(ei_rect_t));
        *rect = *content_rect;
        widget->content_rect = rect;
    }
}
