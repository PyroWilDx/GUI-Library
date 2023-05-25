#include "ei_widget_configure.h"
#include "ei_toplevel.h"
#include "utils_draw.h"
#include "ei_application.h"
#include "ei_button.h"
#include "utils_general.h"
#include "utils_widget.h"

int close_button_x = 10;
int close_button_y = 5;
ei_size_t close_button_size = {20, 15};
int close_button_corner_radius = 7;
int close_button_border_width = 3;
ei_color_t close_button_color = {0xa0, 0x0, 0x0, 0xff};

static void close_button_callback(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param) {
    ei_widget_destroy(widget->parent);
}

static void place_close_button(ei_widget_t widget) {
    ei_rect_t toplevel_screen_location = widget->parent->screen_location;
    ei_impl_toplevel_t *toplevel = (ei_impl_toplevel_t *) widget->parent;

    ei_place_xy(widget,
                toplevel_screen_location.size.width - close_button_x - close_button_size.width - toplevel->border_width,
                -toplevel->border_width - close_button_size.height - close_button_y);
}


ei_widgetclass_t *ei_create_toplevel_wc() {
    ei_widgetclass_t *widget_class = malloc(sizeof(ei_widgetclass_t));
    strcpy(widget_class->name, "toplevel");
    widget_class->allocfunc = &ei_alloc_toplevel;
    widget_class->releasefunc = &ei_release_toplevel;
    widget_class->drawfunc = &ei_draw_toplevel;
    widget_class->setdefaultsfunc = &ei_set_defaults_toplevel;
    widget_class->geomnotifyfunc = &ei_geomnotify_toplevel;
    widget_class->handlefunc = &ei_handle_toplevel;
    widget_class->next = NULL;
    return widget_class;
}

ei_widget_t ei_alloc_toplevel() {
    ei_impl_toplevel_t *toplevel = calloc(1, sizeof(ei_impl_toplevel_t));
    return (ei_widget_t) toplevel;
}

void ei_release_toplevel(ei_widget_t widget) {
    release_widget(widget);
    free(widget);
}

void ei_draw_toplevel(ei_widget_t widget,
                      ei_surface_t surface,
                      ei_surface_t pick_surface,
                      ei_rect_t *clipper) {
    ei_impl_toplevel_t *toplevel = (ei_impl_toplevel_t *) widget;

    ei_color_t color_darker = {max(toplevel->color.red - 50, 0),
                               max(toplevel->color.green - 50, 0),
                               max(toplevel->color.blue - 50, 0),
                               255};
    ei_rect_t top_rect = {widget->screen_location.top_left, {widget->screen_location.size.width,
                                                             widget->content_rect->top_left.y -
                                                             widget->screen_location.top_left.y}};
    ei_rect_t right_rect = {{widget->screen_location.top_left.x, widget->content_rect->top_left.y},
                            {toplevel->border_width,             widget->content_rect->size.height}};
    ei_rect_t bottom_rect = {{widget->screen_location.top_left.x,
                                                                  widget->content_rect->top_left.y +
                                                                  widget->content_rect->size.height},
                             {widget->screen_location.size.width, toplevel->border_width}};
    ei_rect_t left_rect = {{widget->content_rect->top_left.x +
                            widget->content_rect->size.width, widget->content_rect->top_left.y},
                           {toplevel->border_width,           widget->content_rect->size.height}};

    draw_round_rect(surface, pick_surface, &top_rect, color_darker, *widget->pick_color, ei_relief_none,
                    k_default_button_corner_radius, 0, clipper, top_rounded, NULL);

    if (toplevel->border_width != 0) {
        draw_round_rect(surface, pick_surface, &left_rect, color_darker, *widget->pick_color, ei_relief_none,
                        0, 0, clipper, everywhere_rounded, NULL);
        draw_round_rect(surface, pick_surface, &bottom_rect, color_darker, *widget->pick_color, ei_relief_none,
                        0, 0, clipper, everywhere_rounded, NULL);
        draw_round_rect(surface, pick_surface, &right_rect, color_darker, *widget->pick_color, ei_relief_none,
                        0, 0, clipper, everywhere_rounded, NULL);
    }
    ei_rect_t rect_inside;
    int text_width, text_height;
    hw_text_compute_size(toplevel->title, ei_default_font, &text_width, &text_height);
    rect_inside.size.width = widget->screen_location.size.width - 2 * toplevel->border_width;
    rect_inside.size.height = widget->screen_location.size.height - 3 * toplevel->border_width - text_height;
    rect_inside.top_left.x = widget->screen_location.top_left.x + toplevel->border_width;
    rect_inside.top_left.y = widget->screen_location.top_left.y + 2 * toplevel->border_width + text_height;
    draw_round_rect(surface, pick_surface, &rect_inside, toplevel->color, *widget->pick_color, ei_relief_none, 0, 0,
                    clipper, everywhere_rounded, NULL);
    int text_x = widget->screen_location.top_left.x + toplevel->border_width;
    int text_y = widget->screen_location.top_left.y + toplevel->border_width;
    ei_point_t text_location = {text_x, text_y};

    if (toplevel->resizable) {
        draw_round_rect(surface, pick_surface, &toplevel->resize_square_location, color_darker, *widget->pick_color,
                        ei_relief_none, 0, 0, clipper, everywhere_rounded, NULL);
    }

    ei_rect_t text_rect = {text_location, {text_width, text_height}};
    ei_rect_t clipper_text = get_text_img_intersect_rect(widget, &text_rect, true);
    ei_draw_text(surface, &text_location,
                 toplevel->title, ei_default_font,
                 (ei_color_t) {0xFF, 0xFF, 0xFF, 0xFF},
                 &clipper_text);
}

void ei_set_defaults_toplevel(ei_widget_t widget) {
    ei_impl_toplevel_t *toplevel = (ei_impl_toplevel_t *) widget;
    strcpy(toplevel->title, "TopLevel");
    ei_color_t color = {255, 255, 255, 255};
    toplevel->color = color;
    toplevel->closable = true;
    toplevel->resizable = true;
    toplevel->border_width = DEFAULT_BORDER_WIDTH;
    ei_rect_t resize_rect = {{0,  0},
                             {26, 26}};
    toplevel->resize_square_location = resize_rect;
    toplevel->min_size = &toplevel->resize_square_location.size;

    ei_widget_t close_widget = ei_widget_create("button", widget, NULL, NULL);
    ei_impl_button_t *close_button = (ei_impl_button_t *) close_widget;
    close_button->outside = true;
    ei_button_configure(close_widget, &close_button_size, &close_button_color, &close_button_border_width,
                        &close_button_corner_radius, &(ei_relief_t) {ei_relief_raised}, NULL, NULL, NULL, NULL, NULL,
                        NULL, NULL,
                        &(ei_callback_t) {close_button_callback}, NULL);
    toplevel->close_button = close_widget;
    place_close_button(close_widget);

    toplevel->widget = *widget;
}

void ei_geomnotify_toplevel(ei_widget_t widget) {
    ei_impl_toplevel_t *toplevel = (ei_impl_toplevel_t *) widget;
    ei_rect_t *rect_inside = widget->content_rect;
    int text_height, text_width;
    hw_text_compute_size(toplevel->title, ei_default_font, &text_width, &text_height);
    rect_inside->top_left.y = widget->screen_location.top_left.y + 2 * toplevel->border_width + text_height;
    rect_inside->top_left.x = widget->screen_location.top_left.x + toplevel->border_width;
    rect_inside->size = widget->requested_size;
    ei_size_t size_border;
    size_border.width = rect_inside->size.width + 2 * toplevel->border_width;
    size_border.height = rect_inside->size.height + 3 * toplevel->border_width + text_height;
    widget->screen_location.size = size_border;
    widget->content_rect = rect_inside;

    ei_rect_t screen_location = widget->screen_location;
    ei_app_invalidate_rect(&screen_location);

    int square_length = toplevel->resize_square_location.size.width;
    toplevel->resize_square_location.top_left.x =
            screen_location.top_left.x + screen_location.size.width - square_length;
    toplevel->resize_square_location.top_left.y =
            screen_location.top_left.y + screen_location.size.height - square_length;

    // close button
    ei_widget_t close_button = toplevel->close_button;
    if (toplevel->closable) {
        if (close_button->placer_params != NULL) {
            close_button->placer_params = (struct ei_impl_placer_params_t *) &widget->screen_location.top_left;
        }
        place_close_button(close_button);
    } else {
        ei_placer_forget(close_button);
    }
}

int cpt = 0;

bool ei_handle_toplevel(ei_widget_t widget, struct ei_event_t *event) {
    ei_point_t where = event->param.mouse.where;
    ei_impl_toplevel_t *toplevel = (ei_impl_toplevel_t *) widget;
    if (event->type == ei_ev_mouse_buttondown) {
        if (where.y <= widget->content_rect->top_left.y) {
            toplevel->translating = true;
            toplevel->rel_mouse_pos.x = where.x - widget->screen_location.top_left.x;
            toplevel->rel_mouse_pos.y = where.y - widget->screen_location.top_left.y;
        } else if (where.x >= toplevel->resize_square_location.top_left.x &&
                   where.y >= toplevel->resize_square_location.top_left.y &&
                   toplevel->resizable != ei_axis_none) {
            toplevel->resizing = true;
            toplevel->rel_mouse_pos.x =
                    widget->content_rect->top_left.x + widget->requested_size.width - where.x;
            toplevel->rel_mouse_pos.y =
                    widget->content_rect->top_left.y + widget->requested_size.height - where.y;
        }
        ei_event_set_active_widget(widget);
        return false;

    } else if (event->type == ei_ev_mouse_buttonup) {
        toplevel->resizing = false;
        toplevel->translating = false;
        ei_event_set_active_widget(NULL);
        return false;

    } else if (event->type == ei_ev_mouse_move) {
        ei_widget_t parent = widget->parent;
        ei_rect_t loc = widget->screen_location;
        int children_number = get_children_number(widget);
        if (cpt > 2 * log(children_number + 1)) {
            cpt = 0;
            if (toplevel->translating) {
                int new_x = where.x - toplevel->rel_mouse_pos.x - parent->content_rect->top_left.x;
                int new_y = where.y - toplevel->rel_mouse_pos.y - parent->content_rect->top_left.y;

                if (new_x <= 0) new_x = 0;
                if (new_y <= 0) new_y = 0;

                ei_place_xy(widget, new_x, new_y);
            } else if (toplevel->resizing) {
                int new_width, new_height;
                ei_rect_t content_rect = *widget->content_rect;
                new_width = where.x + toplevel->rel_mouse_pos.x - content_rect.top_left.x;
                new_height = where.y + toplevel->rel_mouse_pos.y - content_rect.top_left.y;

                if (new_width < toplevel->min_size->width &&
                    new_height < toplevel->min_size->height) {
                    return false;
                }

                new_width = max(new_width, toplevel->min_size->width);
                new_height = max(new_height, toplevel->min_size->height);

                ei_point_t parent_location = parent->content_rect->top_left;
                ei_point_t rel_location;
                rel_location.x = loc.top_left.x - parent_location.x;
                rel_location.y = loc.top_left.y - parent_location.y;
                ei_place(widget, NULL, &rel_location.x, &rel_location.y, &new_width, &new_height, NULL,
                         NULL, NULL, NULL);
            }
        }
        cpt++;
        return false;
    }

    return false;
}
