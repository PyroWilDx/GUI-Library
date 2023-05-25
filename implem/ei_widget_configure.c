#include "ei_application.h"
#include "ei_widget_configure.h"
#include "ei_frame.h"
#include "ei_button.h"
#include "ei_toplevel.h"
#include "utils_draw.h"


void ei_frame_configure(ei_widget_t widget,
                        ei_size_t *requested_size,
                        const ei_color_t *color,
                        int *border_width,
                        ei_relief_t *relief,
                        ei_string_t *text,
                        ei_font_t *text_font,
                        ei_color_t *text_color,
                        ei_anchor_t *text_anchor,
                        ei_surface_t *img,
                        ei_rect_ptr_t *img_rect,
                        ei_anchor_t *img_anchor) {
    ei_impl_frame_t *frame = (ei_impl_frame_t *) widget;

    if (requested_size != NULL) {
        frame->widget.requested_size = *requested_size;
        if (widget->parent != NULL) {
            widget->rel_width = (float) requested_size->width / (float) widget->parent->content_rect->size.width;
            widget->rel_height = (float) requested_size->height / (float) widget->parent->content_rect->size.height;
        }
        widget->screen_location.size = *requested_size;
    }
    if (color != NULL) frame->color = *color;
    if (border_width != NULL) frame->border_width = *border_width;
    if (relief != NULL) frame->relief = *relief;
    if (text != NULL) {
        if (*text == NULL) {
            strncpy(frame->text, "\0", MAX_TEXT_LENGTH);
        } else {
            strncpy(frame->text, *text, MAX_TEXT_LENGTH);
        }
    }
    if (text_font != NULL) frame->text_font = *text_font;
    if (text_color != NULL) frame->text_color = *text_color;
    if (text_anchor != NULL) frame->text_anchor = *text_anchor;
    ei_rect_t tmp_img_rect;
    ei_rect_t *ptr_tmp_img_rect;
    if (img != NULL && *img != NULL) {
        if (img_rect == NULL) {
            tmp_img_rect = hw_surface_get_rect(*img);
            ptr_tmp_img_rect = &tmp_img_rect;
            img_rect = &ptr_tmp_img_rect;
        }
        frame->img = hw_surface_create(ei_app_root_surface(), (*img_rect)->size, false);
        hw_surface_lock(frame->img);
        hw_surface_lock(*img);
        ei_rect_t rect_surface = {{0, 0}, (*img_rect)->size};
        ei_copy_surface(frame->img, &rect_surface, *img, *img_rect, false);
        hw_surface_unlock(frame->img);
        hw_surface_unlock(*img);
    }
    if (img_rect != NULL) frame->img_rect = **img_rect;
    if (img_anchor != NULL) frame->img_anchor = *img_anchor;

    ei_rect_t new_rect = widget->screen_location;
    ei_app_invalidate_rect(&new_rect);
}


void ei_button_configure(ei_widget_t widget,
                         ei_size_t *requested_size,
                         const ei_color_t *color,
                         int *border_width,
                         int *corner_radius,
                         ei_relief_t *relief,
                         ei_string_t *text,
                         ei_font_t *text_font,
                         ei_color_t *text_color,
                         ei_anchor_t *text_anchor,
                         ei_surface_t *img,
                         ei_rect_ptr_t *img_rect,
                         ei_anchor_t *img_anchor,
                         ei_callback_t *callback,
                         ei_user_param_t *user_param) {
    ei_impl_button_t *button = (ei_impl_button_t *) widget;

    if (requested_size != NULL) {
        button->widget.requested_size = *requested_size;
        if (widget->parent != NULL) {
            widget->rel_width = (float) requested_size->width / (float) widget->parent->content_rect->size.width;
            widget->rel_height = (float) requested_size->height / (float) widget->parent->content_rect->size.height;
        }
        widget->screen_location.size = *requested_size;
    }
    if (color != NULL) button->color = *color;
    if (text_color != NULL) button->text_color = *text_color;
    if (border_width != NULL) button->border_width = *border_width;
    if (relief != NULL) button->clicked = !(*relief == ei_relief_none || *relief == ei_relief_raised);
    if (text != NULL) {
        if (*text == NULL) {
            strncpy(button->text, "\0", MAX_TEXT_LENGTH);
        } else {
            strncpy(button->text, *text, MAX_TEXT_LENGTH);
        }
    }
    if (text_font != NULL) button->text_font = *text_font;
    if (text_color != NULL) button->text_color = *text_color;
    if (text_anchor != NULL) button->text_anchor = *text_anchor;
    if (img != NULL && *img == NULL) {
        if (button->img != NULL) {
            hw_surface_free(button->img);
        }
        button->img = NULL;
    }
    ei_rect_t tmp_img_rect;
    ei_rect_t *ptr_tmp_img_rect;
    if (img != NULL && *img != NULL) {
        if (img_rect == NULL) {
            tmp_img_rect = hw_surface_get_rect(*img);
            ptr_tmp_img_rect = &tmp_img_rect;
            img_rect = &ptr_tmp_img_rect;
        }
        button->img = hw_surface_create(ei_app_root_surface(), (*img_rect)->size, false);
        hw_surface_lock(button->img);
        hw_surface_lock(*img);
        ei_rect_t rect_surface = {{0, 0}, (*img_rect)->size};
        ei_copy_surface(button->img, &rect_surface, *img, *img_rect, false);
        hw_surface_unlock(button->img);
        hw_surface_unlock(*img);
    }
    if (img_rect != NULL) button->img_rect = **img_rect;
    if (img_anchor != NULL) button->img_anchor = *img_anchor;
    if (callback != NULL) button->callback = *callback;
    if (corner_radius != NULL) button->corner_radius = *corner_radius;
    if (user_param != NULL) widget->user_data = *user_param;

    ei_rect_t new_rect = widget->screen_location;
    ei_app_invalidate_rect(&new_rect);
}

void ei_toplevel_configure(ei_widget_t widget,
                           ei_size_t *requested_size,
                           ei_color_t *color,
                           int *border_width,
                           ei_string_t *title,
                           bool *closable,
                           ei_axis_set_t *resizable,
                           ei_size_ptr_t *min_size) {
    ei_impl_toplevel_t *toplevel = (ei_impl_toplevel_t *) widget;

    if (color != NULL) toplevel->color = *color;
    if (border_width != NULL) toplevel->border_width = *border_width;
    if (title != NULL) {
        if (*title == NULL) {
            strncpy(toplevel->title, "\0", MAX_TEXT_LENGTH);
        } else {
            strncpy(toplevel->title, *title, MAX_TEXT_LENGTH);
        }
    }
    if (closable != NULL) toplevel->closable = *closable;
    if (resizable != NULL) toplevel->resizable = *resizable;
    if (min_size != NULL) toplevel->min_size = *min_size;
    if (requested_size != NULL) {
        toplevel->widget.requested_size = *requested_size;
        if (widget->parent != NULL) {
            widget->rel_width = (float) requested_size->width / (float) widget->parent->content_rect->size.width;
            widget->rel_height = (float) requested_size->height / (float) widget->parent->content_rect->size.height;
        }

        int text_height, text_width;
        hw_text_compute_size(toplevel->title, ei_default_font, &text_width, &text_height);

        ei_size_t size_border;
        size_border.width = requested_size->width + 2 * toplevel->border_width;
        size_border.height = requested_size->height + 3 * toplevel->border_width + text_height;
        widget->screen_location.size = size_border;

        ei_rect_t screen_location = widget->screen_location;
        int square_length = toplevel->resize_square_location.size.width;
        toplevel->resize_square_location.top_left.x =
                screen_location.top_left.x + screen_location.size.width - square_length;
        toplevel->resize_square_location.top_left.y =
                screen_location.top_left.y + screen_location.size.height - square_length;

        ei_rect_t *rect_inside = malloc(sizeof(ei_rect_t));
        rect_inside->top_left.y = widget->screen_location.top_left.y + 2 * toplevel->border_width + text_height;
        rect_inside->top_left.x = widget->screen_location.top_left.x + toplevel->border_width;
        rect_inside->size = widget->requested_size;
        widget->content_rect = rect_inside;
    }

    ei_rect_t new_rect = widget->screen_location;
    ei_app_invalidate_rect(&new_rect);
}
