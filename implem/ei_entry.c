#include <SDL2/SDL.h>
#include <SDL2/SDL_clipboard.h>

#include "ei_entry.h"
#include "ei_application.h"
#include "ei_event.h"
#include "utils_widget.h"
#include "utils_general.h"

#define MAX_KEY_CODE 256
char shifted_keys[MAX_KEY_CODE] = {1};
char alted_keys[MAX_KEY_CODE] = {1};

active_entries_t *active_entries = NULL;

ei_widgetclass_t *ei_create_entry_wc() {
    ei_widgetclass_t *widget_class = malloc(sizeof(ei_widgetclass_t));
    strcpy(widget_class->name, "entry");
    widget_class->allocfunc = &ei_alloc_entry;
    widget_class->releasefunc = &ei_release_entry;
    widget_class->drawfunc = &ei_draw_entry;
    widget_class->setdefaultsfunc = &ei_set_defaults_entry;
    widget_class->geomnotifyfunc = &ei_geomnotify_entry;
    widget_class->handlefunc = &ei_handle_entry;
    widget_class->next = NULL;
    return widget_class;
}

ei_widget_t ei_alloc_entry() {
    ei_impl_entry_t *entry = calloc(1, sizeof(ei_impl_entry_t));

    ei_widget_t widget = (ei_widget_t) entry;
    active_entries_t *new_cell = malloc(sizeof(active_entries_t));
    new_cell->widget = widget;
    new_cell->next = NULL;
    if (active_entries == NULL) {
        active_entries = new_cell;
    } else {
        active_entries_t *tmp = active_entries;
        while (tmp->next != NULL) {
            tmp = tmp->next;
        }
        tmp->next = new_cell;
    }

    return widget;
}

void ei_release_entry(ei_widget_t widget) {
    release_widget(widget);

    active_entries_t *tmp = active_entries;
    if (active_entries->widget == widget) {
        active_entries = active_entries->next;
        free(tmp);
    } else {
        while (tmp->next != NULL) {
            if (tmp->next->widget == widget) {
                active_entries_t *tmp_free = tmp->next;
                tmp->next = tmp->next->next;
                free(tmp_free);
                break;
            }
            tmp = tmp->next;
        }
    }

    free(widget);
}

void ei_draw_entry(ei_widget_t widget,
                   ei_surface_t surface,
                   ei_surface_t pick_surface,
                   ei_rect_t *clipper) {
    ei_impl_entry_t *entry = (ei_impl_entry_t *) widget;

    bool active = (widget == ei_event_get_active_widget());

    int border_width = entry->border_width;
    if (active) border_width += 6;

    ei_relief_t relief = ei_relief_raised;

    ei_color_t border_color = {100, 100, 100, 255};
    if (active) border_color = (ei_color_t) {30, 100, 255, 255};

    ei_rect_t screen_location = widget->screen_location;
    draw_round_rect(surface, pick_surface, &screen_location,
                    (ei_color_t) {255, 255, 255, 255},
                    *widget->pick_color, relief,
                    0, border_width, clipper,
                    everywhere_rounded, &border_color);


    if (strcmp(entry->text, "\0") != 0) {
        ei_rect_t text_rect = get_text_rect(widget, entry->text, entry->text_font,
                                            ei_anc_west);
        if (text_rect.size.width == 0) {
            return;
        }

        ei_point_t text_location = text_rect.top_left;
        text_location.x += border_width;

        ei_rect_t clipper_text = get_text_img_intersect_rect(widget, &text_rect, false);
        ei_draw_text(surface, &text_location,
                     entry->text, entry->text_font,
                     entry->text_color, &clipper_text);
    }
}

void ei_set_defaults_entry(ei_widget_t widget) {
    ei_impl_entry_t *entry = (ei_impl_entry_t *) widget;
    strcpy(entry->text, "");
    entry->text_font = ei_default_font;
    entry->text_color = ei_font_default_color;
    entry->border_width = 0;
    entry->widget = *widget;
}

void ei_geomnotify_entry(ei_widget_t widget) {
    ei_app_invalidate_rect(&widget->screen_location);
}

static bool modifier_pressed(unsigned int modifier_mask) {
    for (int i = 0; i <= ei_mod_ctrl_right; i++) {
        if (ei_has_modifier(modifier_mask, i)) {
            return true;
        }
    }
    return false;
}

static void init_shifted_keys() {
    shifted_keys['1'] = '&';
    shifted_keys['2'] = 'e';
    shifted_keys['3'] = '\"';
    shifted_keys['4'] = '\'';
    shifted_keys['5'] = '(';
    shifted_keys['6'] = '-';
    shifted_keys['7'] = 'e';
    shifted_keys['8'] = '_';
    shifted_keys['9'] = 'c';
    shifted_keys['0'] = 'a';
    shifted_keys[')'] = '^';
    shifted_keys['='] = '+';
    shifted_keys['$'] = '$';
    shifted_keys['%'] = '%';
    shifted_keys[';'] = '.';
    shifted_keys[':'] = '/';
    shifted_keys['!'] = '!';

    shifted_keys[0] = 0;
}

static void init_alted_keys() {
    alted_keys['1'] = '&';
    alted_keys['2'] = '~';
    alted_keys['3'] = '#';
    alted_keys['4'] = '{';
    alted_keys['5'] = '[';
    alted_keys['6'] = '|';
    alted_keys['7'] = '`';
    alted_keys['8'] = '\\';
    alted_keys['9'] = '^';
    alted_keys['0'] = '@';
    alted_keys[')'] = ']';
    alted_keys['='] = '}';

    alted_keys[0] = 0;
}

bool ei_handle_entry(ei_widget_t widget, struct ei_event_t *event) {
    ei_impl_entry_t *entry = (ei_impl_entry_t *) widget;
    if (event->type == ei_ev_mouse_buttondown) {
        ei_event_set_active_widget(widget);
        return true;

    } else if (event->type == ei_ev_keydown) {
        int len = (int) strlen(entry->text);
        int key_code = event->param.key.key_code;
        unsigned int modifier_mask = event->param.key.modifier_mask;
        bool ctrl_pressed = ei_has_modifier(modifier_mask, ei_mod_ctrl_left)
                            || ei_has_modifier(modifier_mask, ei_mod_ctrl_right);

        if (key_code == SDLK_BACKSPACE) {
            entry->text[len - 1] = '\0';
            return true;

        } else if (ctrl_pressed && key_code == SDLK_u) {
            entry->text[0] = '\0';
            return true;

        } else if (ctrl_pressed && key_code == SDLK_v) {
            if (SDL_HasClipboardText()) {
                char *clipboard_text = SDL_GetClipboardText();
                if (clipboard_text != NULL) {
                    strncat(entry->text, clipboard_text, MAX_ENTRY_LENGTH - 1 - len);
                    SDL_free(clipboard_text);
                    return true;
                }
            }

        } else {
            bool shift_pressed = ei_has_modifier(modifier_mask, ei_mod_shift_left)
                                 || ei_has_modifier(modifier_mask, ei_mod_shift_right);
            bool alt_right_pressed = ei_has_modifier(modifier_mask, ei_mod_alt_right);
            char str[2];
            if (!modifier_pressed(modifier_mask)) {
                if (strlen(entry->text) < MAX_ENTRY_LENGTH - 1) {
                    sprintf(str, "%c", key_code);
                }

            } else if (shift_pressed || alt_right_pressed) {
                if (key_code >= 'a' && key_code <= 'z') {
                    sprintf(str, "%c", toupper(key_code));
                } else {
                    if (shifted_keys[0] == 1) {
                        init_shifted_keys();
                    }
                    if (alted_keys[0] == 1) {
                        init_alted_keys();
                    }
                    if (key_code < MAX_KEY_CODE) {
                        if (shift_pressed) {
                            sprintf(str, "%c", shifted_keys[key_code]);
                        } else if (alt_right_pressed) {
                            sprintf(str, "%c", alted_keys[key_code]);
                        }
                    }
                }
            }

            strcat(entry->text, str);
            return true;
        }
    }

    return false;
}

void ei_entry_configure(ei_widget_t widget,
                        ei_size_t *requested_size,
                        ei_color_t *text_color,
                        int *border_width,
                        ei_font_t *text_font,
                        ei_user_param_t *user_param) {
    ei_impl_entry_t *entry = (ei_impl_entry_t *) widget;

    if (requested_size != NULL) {
        entry->widget.requested_size = *requested_size;
        if (widget->parent != NULL) {
            widget->rel_width = (float) requested_size->width / (float) widget->parent->content_rect->size.width;
            widget->rel_height = (float) requested_size->height / (float) widget->parent->content_rect->size.height;
        }
        widget->screen_location.size = *requested_size;
    }

    if (text_color != NULL) entry->text_color = *text_color;
    if (border_width != NULL) entry->border_width = *border_width;
    if (text_font != NULL) entry->text_font = *text_font;
    if (text_color != NULL) entry->text_color = *text_color;
    if (user_param != NULL) widget->user_data = *user_param;

    ei_rect_t new_rect = widget->screen_location;
    ei_app_invalidate_rect(&new_rect);
}


void update_active_entry() {
    ei_widget_t new_active = NULL;

    ei_widget_t widget = ei_event_get_active_widget();
    if (widget != NULL) {
        if (strcmp(widget->wclass->name, "entry") == 0) {
            active_entries_t *tmp = active_entries;
            while (tmp->widget != widget) {
                tmp = tmp->next;
            }
            if (tmp->next != NULL) {
                new_active = tmp->next->widget;
            } else {
                new_active = active_entries->widget;
            }
        }
    } else {
        new_active = active_entries->widget;
    }

    if (new_active != NULL) {
        ei_event_set_active_widget(new_active);
        ei_app_invalidate_rect(&new_active->screen_location);
        if (widget != NULL) {
            ei_app_invalidate_rect(&widget->screen_location);
        }
    }
}

void set_entry_text(ei_widget_t widget, char *text) {
    if (text != NULL) {
        ei_impl_entry_t *entry = (ei_impl_entry_t *) widget;
        strncpy(entry->text, text, MAX_ENTRY_LENGTH);
        ei_app_invalidate_rect(&widget->screen_location);
    }
}

char *get_entry_text(ei_widget_t widget) {
    ei_impl_entry_t *entry = (ei_impl_entry_t *) widget;
    return entry->text;
}
