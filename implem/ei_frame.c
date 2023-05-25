#include "ei_frame.h"
#include "ei_application.h"
#include "utils_widget.h"
#include "ei_event.h"

ei_widgetclass_t *ei_create_frame_wc() {
    ei_widgetclass_t *widget_class = malloc(sizeof(ei_widgetclass_t));
    strcpy(widget_class->name, "frame");
    widget_class->allocfunc = &ei_alloc_frame;
    widget_class->releasefunc = &ei_release_frame;
    widget_class->drawfunc = &ei_draw_frame;
    widget_class->setdefaultsfunc = &ei_set_defaults_frame;
    widget_class->geomnotifyfunc = &ei_geomnotify_frame;
    widget_class->handlefunc = &ei_handle_frame;
    widget_class->next = NULL;
    return widget_class;
}

ei_widget_t ei_alloc_frame() {
    ei_impl_frame_t *frame = calloc(1, sizeof(ei_impl_frame_t));
    return (ei_widget_t) frame;
}

void ei_release_frame(ei_widget_t widget) {
    release_widget(widget);
    ei_impl_frame_t *frame = (ei_impl_frame_t *) widget;
    if (frame->img != NULL) {
        hw_surface_free(frame->img);
    }
    free(widget);
}

void ei_draw_frame(ei_widget_t widget,
                   ei_surface_t surface,
                   ei_surface_t pick_surface,
                   ei_rect_t *clipper) {
    ei_impl_frame_t *frame = (ei_impl_frame_t *) widget;

    ei_rect_t screen_location = widget->screen_location;

    draw_round_rect(surface, pick_surface, &screen_location, frame->color, *widget->pick_color, frame->relief,
                    0, frame->border_width, clipper, everywhere_rounded, NULL);

    if (strcmp(frame->img_filename, "\0") != 0 || frame->img != NULL) {
        ei_size_t frame_size = screen_location.size;
        ei_size_t img_size = frame->img_rect.size;

        ei_point_t top_left_img = get_where_img(widget->screen_location.top_left, frame_size.width, frame_size.height,
                                                img_size.width, img_size.height, frame->img_anchor);

        ei_rect_t rect_dest_img = {screen_location.top_left, img_size};
        ei_rect_t clipper_img = get_text_img_intersect_rect(widget, &rect_dest_img, false);

        ei_draw_image(surface, &top_left_img, frame->img_filename, frame->img,
                      &clipper_img);
    }

    if (strcmp(frame->text, "\0") != 0) {
        ei_rect_t text_rect = get_text_rect(widget, frame->text, frame->text_font,
                                            frame->text_anchor);
        if (text_rect.size.width == 0) {
            return;
        }

        ei_rect_t clipper_text = get_text_img_intersect_rect(widget, &text_rect, false);
        ei_draw_text(surface, &text_rect.top_left,
                     frame->text, frame->text_font,
                     frame->text_color, &clipper_text);
    }

}

void ei_set_defaults_frame(ei_widget_t widget) {
    ei_impl_frame_t *frame = (ei_impl_frame_t *) widget;
    strcpy(frame->text, "\0");
    frame->text_font = ei_default_font;
    frame->text_color = ei_font_default_color;
    ei_color_t color = {110, 100, 100, 255};
    frame->color = color;
    frame->text_anchor = ei_anc_center;
    frame->img_anchor = ei_anc_center;
    frame->relief = ei_relief_none;
    strcpy(frame->img_filename, "\0");
    frame->img = NULL;
    ei_rect_t rect = {{0, 0},
                      {0, 0}};
    frame->img_rect = rect;
    frame->border_width = DEFAULT_BORDER_WIDTH;

    frame->widget = *widget;
}

void ei_geomnotify_frame(ei_widget_t widget) {
    ei_app_invalidate_rect(&widget->screen_location);
}

bool ei_handle_frame(ei_widget_t widget, struct ei_event_t *event) {
    return false;
}
