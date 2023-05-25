#include "ei_widget.h"
#include "ei_implementation.h"
#include "screen.h"
#include "ei_application.h"
#include "utils_draw.h"

uint32_t current_pick_id = 1;

ei_widget_t ei_widget_create(ei_const_string_t class_name,
                             ei_widget_t parent,
                             ei_user_param_t user_data,
                             ei_widget_destructor_t destructor) {
    ei_widgetclass_t *widget_class = ei_widgetclass_from_name(class_name);
    ei_widget_t widget = widget_class->allocfunc();
    widget->wclass = widget_class;
    widget->user_data = user_data;
    widget->pick_id = current_pick_id;
    ei_color_t *pick_color = malloc(sizeof(ei_color_t));
    pick_color->alpha = 255;
    pick_color->red = current_pick_id;
    pick_color->green = current_pick_id >> 8;
    pick_color->blue = 0;
    widget->pick_color = pick_color;
    current_pick_id++;
    widget->destructor = destructor;

    if (parent->children_head == NULL) {
        parent->children_head = widget;
    } else {
        parent->children_tail->next_sibling = widget;
    }

    widget->parent = parent;
    parent->children_tail = widget;
    widget->next_sibling = NULL;
    widget->children_head = NULL;
    widget->children_tail = NULL;

    ei_size_t default_size = {0, 0};
    ei_rect_t default_loc = {{0, 0}, default_size};
    widget->requested_size = default_size;
    widget->screen_location = default_loc;
    widget->content_rect = &widget->screen_location;
    widget->rel_x = 0;
    widget->rel_y = 0;
    if (widget->parent != NULL) {
        widget->rel_width = (float) default_size.width / (float) widget->parent->content_rect->size.width;
        widget->rel_height = (float) default_size.height / (float) widget->parent->content_rect->size.height;
    }
    widget->placer_params = (struct ei_impl_placer_params_t *) &widget->screen_location.top_left;

    widget_class->setdefaultsfunc(widget);
    return widget;
}

void ei_widget_destroy(ei_widget_t widget) {
    if (widget->destructor != NULL) {
        widget->destructor(widget);
    }
    widget->wclass->releasefunc(widget);
}

bool ei_widget_is_displayed(ei_widget_t widget) {
    return (widget->placer_params != NULL);
}

static ei_widget_t find_from_id_rec(ei_widget_t widget, uint32_t pick_id) {
    if (widget == NULL) return NULL;

    if (widget->pick_id == pick_id) return widget;

    ei_widget_t tmp;
    tmp = find_from_id_rec(widget->next_sibling, pick_id);
    if (tmp == NULL) {
        tmp = find_from_id_rec(widget->children_head, pick_id);
    }
    return tmp;
}

ei_widget_t ei_widget_pick(ei_point_t *where) {
    ei_widget_t root_widget = ei_app_root_widget();
    int x = where->x;
    int y = where->y;
    int *pixel_0 = get_pixel_0_pick();
    int width = get_pick_surface_width();
    int *pick_color =  get_pixel_from_coordinates(pixel_0, x, y, width);
    color_indexes indexes = get_color_indexes();
    int pick_id = (*pick_color >> (8 * indexes.ir)) & 0xFF;
    pick_id += ((*pick_color >> (8 * indexes.ig)) & 0xFF) << 8;
//    printf("PICK_ID : %d, COORDS : (%d, %d)\n", pick_id, x, y);
    return find_from_id_rec(root_widget, pick_id);
}
