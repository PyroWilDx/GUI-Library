#include <ei_widget_configure.h>
#include "ei_button.h"
#include "ei_application.h"
#include "ei_implementation.h"
#include "ei_event.h"
#include "utils_widget.h"
#include "utils_general.h"

ei_widgetclass_t *ei_create_button_wc() {
    ei_widgetclass_t *widget_class = malloc(sizeof(ei_widgetclass_t));
    strcpy(widget_class->name, "button");
    widget_class->allocfunc = &ei_alloc_button;
    widget_class->releasefunc = &ei_release_button;
    widget_class->drawfunc = &ei_draw_button;
    widget_class->setdefaultsfunc = &ei_set_defaults_button;
    widget_class->geomnotifyfunc = &ei_geomnotify_button;
    widget_class->handlefunc = &ei_handle_button;
    widget_class->next = NULL;
    return widget_class;
}

ei_widget_t ei_alloc_button() {
    ei_impl_button_t *button = calloc(1, sizeof(ei_impl_button_t));
    return (ei_widget_t) button;
}

void ei_release_button(ei_widget_t widget) {
    release_widget(widget);
    ei_impl_button_t *button = (ei_impl_button_t *) widget;
    if (button->img != NULL) {
        hw_surface_free(button->img);
    }
    free(widget);
}

void ei_draw_button(ei_widget_t widget,
                    ei_surface_t surface,
                    ei_surface_t pick_surface,
                    ei_rect_t *clipper) {
    ei_impl_button_t *button = (ei_impl_button_t *) widget;

    ei_relief_t relief;
    if (button->clicked) relief = ei_relief_sunken;
    else relief = ei_relief_raised;

    ei_rect_t screen_location = widget->screen_location;
    draw_round_rect(surface, pick_surface, &screen_location, button->color, *widget->pick_color, relief,
                    button->corner_radius, button->border_width, clipper,
                    everywhere_rounded, NULL);

    if (strcmp(button->img_filename, "\0") != 0 || button->img != NULL) {
        ei_size_t button_size = screen_location.size;
        ei_size_t img_size = button->img_rect.size;

        ei_point_t top_left_img = get_where_img(screen_location.top_left, button_size.width, button_size.height,
                                                img_size.width, img_size.height, button->img_anchor);

        ei_rect_t rect_dest_img = {screen_location.top_left, img_size};
        ei_rect_t clipper_img = get_text_img_intersect_rect(widget, &rect_dest_img, false);
        ei_draw_image(surface, &top_left_img, button->img_filename, button->img,
                      &clipper_img);
    }

    if (strcmp(button->text, "\0") != 0) {
        ei_rect_t text_rect = get_text_rect(widget, button->text, button->text_font,
                                            button->text_anchor);
        if (text_rect.size.width == 0) {
            return;
        }

        ei_point_t text_location = text_rect.top_left;
        int button_width = screen_location.size.width;
        int button_height = screen_location.size.height;
        int x_click_gap = button_width / 25;
        int y_click_gap = button_height / 25;

        if (button->clicked) {
            text_location.x += x_click_gap;
            text_location.y += y_click_gap;
        }

        ei_rect_t clipper_text = get_text_img_intersect_rect(widget, &text_rect, false);
        ei_draw_text(surface, &text_location,
                     button->text, button->text_font,
                     button->text_color, &clipper_text);
    }
}

void ei_set_defaults_button(ei_widget_t widget) {
    ei_impl_button_t *button = (ei_impl_button_t *) widget;
    strcpy(button->text, "");
    button->text_font = ei_default_font;
    button->text_color = ei_font_default_color;
    button->text_anchor = ei_anc_center;
    ei_color_t color = {200, 200, 200, 255};
    button->color = color;
    button->clicked = false;
    button->border_width = k_default_button_border_width;
    button->corner_radius = k_default_button_corner_radius;
    button->img = NULL;
    strcpy(button->img_filename, "\0");
    button->img_anchor = ei_anc_center;
    ei_rect_t img_rect = {{0, 0},
                          {0, 0}};
    button->img_rect = img_rect;
    button->outside = false;

    button->widget = *widget;

}

void ei_geomnotify_button(ei_widget_t widget) {
    ei_app_invalidate_rect(&widget->screen_location);
}

bool ei_handle_button(ei_widget_t widget, struct ei_event_t *event) {
    ei_impl_button_t *button = (ei_impl_button_t *) widget;
    if (event->type == ei_ev_mouse_buttondown) {
        button->clicked = true;
        ei_event_set_active_widget(widget);
        return true;

    } else if (event->type == ei_ev_mouse_buttonup) {
        button->clicked = false;
        ei_rect_t loc = widget->screen_location;

        ei_point_t where = event->param.mouse.where;
        if (point_is_in_clipper(loc.top_left.x, loc.top_left.y,
                                loc.top_left.x + loc.size.width,
                                loc.top_left.y + loc.size.height,
                                where.x, where.y)) {
            if (button->callback != NULL) {
                button->callback(widget, event, widget->user_data);
                if (ei_event_get_active_widget() == NULL) { // callback released button
                    return false;
                }
            }
        }
        ei_event_set_active_widget(NULL);
        return true;
    }

    return false;
}
