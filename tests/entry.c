#include <stdlib.h>
#include <stdio.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget_configure.h"
#include "ei_widget.h"
#include "../implem/ei_implementation.h"
#include "../implem/ei_entry.h"

ei_widget_t entry;
ei_widget_t entry2;
ei_widget_t frame;

bool default_handler(ei_event_t *event) {
    if ((event->type == ei_ev_close) ||
        ((event->type == ei_ev_keydown) && (event->param.key.key_code == SDLK_ESCAPE))) {
        ei_app_quit_request();
        return true;

    } else if (event->type == ei_ev_keydown) {
        char res[1000];
        FILE *fp;
        if (event->param.key.key_code == SDLK_RETURN) {
            printf("executed : %s\n", get_entry_text(entry));
            fp = popen(get_entry_text(entry), "r");
            set_entry_text(entry, "");
            fscanf(fp, "%s", res);
            pclose(fp);
            ei_frame_set_text(frame, res);
            return true;
        }
        if (event->param.key.key_code == SDLK_KP_ENTER) {
            printf("in cmake, executed : %s\n", get_entry_text(entry2));
            char text[1000] = "cd cmake && ";
            strcat(text, get_entry_text(entry2));
            system(text);
            set_entry_text(entry2, "");
            return true;
        }
    }
    return false;
}

int main() {
    ei_app_create((ei_size_t) {1600, 800}, false);
    ei_frame_set_bg_color(ei_app_root_widget(), (ei_color_t) {0x52, 0x7f, 0xb4, 0xff});

    ei_widget_t toplevel;
    toplevel = ei_widget_create("toplevel", ei_app_root_widget(), NULL, NULL);
    ei_size_t min_size = {100, 100};
    ei_size_ptr_t min_size_ptr = &min_size;
    ei_toplevel_configure(toplevel, &((ei_size_t) {1500, 600}), &(ei_color_t) {0x20, 0x90, 0xb0, 0x50},
                          &((int) {5}),
                          &(ei_string_t) {"toplevel"},
                          &(bool) {true},
                          &(ei_axis_set_t) {ei_axis_both},
                          &min_size_ptr);
    ei_place_xy(toplevel, 10, 10);


    ei_size_t requested_size = {1600, 50};
    entry = ei_widget_create("entry", toplevel, NULL, NULL);
    ei_entry_configure(entry, &requested_size,
                       &(ei_color_t) {0, 0, 0, 255}, NULL, NULL, NULL);
    ei_place_xy(entry, 10, 10);

    entry2 = ei_widget_create("entry", toplevel, NULL, NULL);
    ei_entry_configure(entry2, &requested_size,
                       &(ei_color_t) {0, 0, 0, 255}, NULL, NULL, NULL);
    ei_place_xy(entry2, 10, 100);

    ei_widget_t entry3 = ei_widget_create("entry", toplevel, NULL, NULL);
    ei_entry_configure(entry3, &requested_size,
                       &(ei_color_t) {200, 0, 0, 255}, NULL, NULL, NULL);
    ei_place_xy(entry3, 10, 200);

    ei_widget_t entry4 = ei_widget_create("entry", toplevel, NULL, NULL);
    ei_entry_configure(entry4, &requested_size,
                       &(ei_color_t) {0, 200, 0, 255}, NULL, NULL, NULL);
    ei_place_xy(entry4, 10, 300);

    ei_widget_t entry5 = ei_widget_create("entry", toplevel, NULL, NULL);
    ei_entry_configure(entry5, &requested_size,
                       &(ei_color_t) {0, 0, 200, 255}, NULL, NULL, NULL);
    ei_place_xy(entry5, 10, 400);

    frame = ei_widget_create("frame", toplevel, NULL, NULL);
    ei_frame_configure(frame, &requested_size,
                       &(ei_color_t) {200, 200, 200, 255}, NULL, NULL, NULL, NULL,
                       NULL, &(ei_anchor_t){ei_anc_west}, NULL, NULL, NULL);
    ei_place_xy(frame, 10, 500);

    ei_event_set_default_handle_func(&default_handler);
    ei_app_run();
    ei_app_free();
    return 0;
}
