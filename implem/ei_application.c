#include "ei_event.h"
#include "ei_application.h"
#include "ei_implementation.h"
#include "ei_toplevel.h"
#include "ei_button.h"
#include "ei_frame.h"
#include "screen.h"
#include "ei_widgetclass.h"
#include "ei_types.h"
#include "utils_widget.h"
#include "lsc.h"
#include "utils_general.h"
#include "ei_entry.h"

ei_widget_t root_widget = NULL;
ei_surface_t root_window = NULL;

ei_linked_rect_t *lsc_rect = NULL;

bool app_running = true;

void ei_app_create(ei_size_t main_window_size, bool fullscreen) {
    hw_init();

    ei_widgetclass_t *toplevel_wc = ei_create_toplevel_wc();
    ei_widgetclass_register(toplevel_wc);
    ei_widgetclass_t *frame_wc = ei_create_frame_wc();
    ei_widgetclass_register(frame_wc);
    ei_widgetclass_t *entry_wc = ei_create_entry_wc();
    ei_widgetclass_register(entry_wc);
    ei_widgetclass_t *button_wc = ei_create_button_wc();
    ei_widgetclass_register(button_wc);

    ei_widget_t frame = frame_wc->allocfunc();
    frame_wc->setdefaultsfunc(frame);
    frame->wclass = frame_wc;
    frame->pick_id = 0;
    ei_color_t *pick_color = malloc(sizeof(ei_color_t));
    pick_color->red = 0;
    pick_color->green = 0;
    pick_color->blue = 0;
    pick_color->alpha = 255;
    frame->pick_color = pick_color;
    frame->user_data = NULL;
    frame->destructor = NULL;
    frame->parent = NULL;
    frame->children_head = NULL;
    frame->children_tail = NULL;
    frame->next_sibling = NULL;
    frame->requested_size = main_window_size;
    ei_rect_t screen_location = {{0, 0}, main_window_size};
    frame->screen_location = screen_location;
    frame->content_rect = &frame->screen_location;
    frame->placer_params = (struct ei_impl_placer_params_t *) &frame->screen_location.top_left;

    root_widget = frame;

    root_window = hw_create_window(main_window_size, fullscreen);

    init_color_indexes(root_window);
    init_pick_surface(root_window, main_window_size);

    ei_app_invalidate_rect(&root_widget->screen_location);
}

static void free_widgetclass() {
    ei_widgetclass_t *wc = ei_widgetclass_from_name("button");
    ei_widgetclass_t *tmp_next;
    while (wc != NULL) {
        tmp_next = wc->next;
        free(wc);
        wc = tmp_next;
    }
}

void ei_app_free(void) {
    hw_surface_free(root_window);
    free_pick_surface();
    hw_quit();
    free_linked_rect(&lsc_rect);
    ei_widget_t first_child = root_widget->children_head;
    if (first_child != NULL) {
        first_child->placer_params = NULL;
        first_child->wclass->releasefunc(first_child);
    }
    free(root_widget->pick_color);
    free(root_widget);
    free_widgetclass();
}

static void draw_rec(ei_widget_t widget) {
    if (widget == NULL) return;

    if (widget->placer_params != NULL) {
        ei_linked_rect_t *done_rect = NULL;
        ei_linked_rect_t *tmp = lsc_rect;
        ei_rect_t *rect1 = &widget->screen_location;
        while (tmp != NULL) {
            if (rect1_intersects_rect2(rect1, &tmp->rect)) {
                ei_rect_t clipper = get_widget_clipper(widget, &tmp->rect);
                if (rect_not_0(&clipper)) {
                    if (!rect_in_rect_of_linked_rect(&clipper, done_rect)) {
//                        printf("DRAW %s : ID=%d, x=%d, y=%d, w=%d, h=%d\n",
//                               widget->wclass->name, widget->pick_id,
//                               clipper.top_left.x, clipper.top_left.y,
//                               clipper.size.width, clipper.size.height);
                        widget->wclass->drawfunc(widget, root_window, get_pick_surface(), &clipper);
                        done_rect = insert_linked_rect(done_rect, &clipper);
                    }
                }
            }
            tmp = tmp->next;
        }
        free_linked_rect(&done_rect);
    }

    draw_rec(widget->children_head);
    draw_rec(widget->next_sibling);
}

void ei_app_run(void) {
    struct ei_event_t event;
    while (app_running) {
        if (lsc_rect != NULL) {
            ei_surface_t pick_surface = get_pick_surface();
            hw_surface_lock(root_window);
            hw_surface_lock(pick_surface);
            draw_rec(root_widget);
            hw_surface_unlock(root_window);
            hw_surface_unlock(pick_surface);
            hw_surface_update_rects(root_window, lsc_rect);
//        hw_surface_update_rects(pick_surface, lsc_rect);
            free_linked_rect(&lsc_rect);
        }

        hw_event_wait_next(&event);
        if (ei_event_get_default_handle_func() != NULL) {
            if (ei_event_get_default_handle_func()(&event)) {
                continue;
            }
        }
        ei_point_t where;
        ei_widget_t widget;
        switch (event.type) {
            case ei_ev_mouse_buttondown:
                widget = ei_event_get_active_widget();
                if (widget != NULL) {
                    if (strcmp(widget->wclass->name, "entry") == 0) {
                        ei_app_invalidate_rect(&widget->screen_location);
                        ei_event_set_active_widget(NULL);
                    }
                }

                where = event.param.mouse.where;
                widget = ei_widget_pick(&where);
                if (widget != NULL) {
                    if (widget->wclass->handlefunc(widget, &event)) {
                        ei_app_invalidate_rect(&widget->screen_location);
                    }

                    ei_rect_t rect = set_front_widget(widget);
                    ei_app_invalidate_rect(&rect);

                }
                break;

            case ei_ev_keydown:
                if (event.param.key.key_code == SDLK_TAB) {
                    update_active_entry();
                } else {
                    widget = ei_event_get_active_widget();
                    if (widget != NULL) {
                        if (widget->wclass->handlefunc(widget, &event)) {
                            ei_app_invalidate_rect(&widget->screen_location);
                        }
                    }
                }
                break;

            default:
                widget = ei_event_get_active_widget();
                if (widget != NULL) {
                    if (widget->wclass->handlefunc(widget, &event)) {
                        ei_app_invalidate_rect(&widget->screen_location);
                    }
                }
                break;
        }
//        app_running = false;
    }
}

void ei_app_invalidate_rect(const ei_rect_t *rect) {
    if (rect->size.width == 0 || rect->size.height == 0) return;

    ei_linked_rect_t *tmp = lsc_rect;
    ei_linked_rect_t *tmp_prc = NULL;
    ei_linked_rect_t *tmp_next;
    while (tmp != NULL) {
        tmp_next = tmp->next;

        if (rect1_in_rect2((ei_rect_t *) rect, &tmp->rect)) {
            return;
        }

        if (rect1_in_rect2(&tmp->rect, (ei_rect_t *) rect)) {
            if (tmp_prc == NULL) {
                lsc_rect = tmp_next;
            } else {
                tmp_prc->next = tmp_next;
            }
            free(tmp);
        } else {
            tmp_prc = tmp;
        }
        tmp = tmp_next;
    }
    ei_rect_t rect_copy = {rect->top_left, rect->size};
    ei_linked_rect_t *new_cell = malloc(sizeof(ei_linked_rect_t));
    new_cell->rect = rect_copy;
    new_cell->next = lsc_rect;
    lsc_rect = new_cell;
}

void ei_app_quit_request(void) {
    app_running = false;
}

ei_widget_t ei_app_root_widget(void) {
    return root_widget;
}

ei_surface_t ei_app_root_surface(void) {
    return root_window;
}
