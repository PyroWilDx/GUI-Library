#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget_configure.h"
#include "ei_widget.h"
#include "../implem/ei_implementation.h"
#include "../implem/ei_button.h"

/*
 * button_press --
 *
 *	Callback called when a user clicks on the button.
 */
void button_press(ei_widget_t widget, ei_event_t *event, ei_user_param_t user_param) {
    printf("Click !\n");
}

/*
 * process_key --
 *
 *	Callback called when any key is pressed by the user.
 *	Simply looks for the "Escape" key to request the application to quit.
 */
bool process_key(ei_event_t *event) {
    if ((event->type == ei_ev_close) ||
        ((event->type == ei_ev_keydown) && (event->param.key.key_code == SDLK_ESCAPE))) {
        ei_app_quit_request();
        return true;
    } else
        return false;
}

/*
 * ei_main --
 *
 *	Main function of the application.
 */
int main(int argc, char **argv) {
    /* Create the application and change the color of the background. */
    ei_app_create((ei_size_t) {600, 600}, false);
    ei_frame_set_bg_color(ei_app_root_widget(), (ei_color_t) {0x52, 0x7f, 0xb4, 0xff});
    ei_event_set_default_handle_func(process_key);

    /* Create, configure and place the button on screen. */
    ei_widget_t toplevel;
    toplevel = ei_widget_create("toplevel", ei_app_root_widget(), NULL, NULL);
    ei_size_t min_size = {100, 100};
    ei_size_ptr_t min_size_ptr = &min_size;
    ei_toplevel_configure(toplevel, &((ei_size_t) {200, 200}), &(ei_color_t) {0x20, 0x90, 0xb0, 0x50},
                          &((int) {5}),
                          &(ei_string_t) {"toplevel"},
                          &(bool) {false},
                          &(ei_axis_set_t) {ei_axis_both},
                          &min_size_ptr);
    ei_place_xy(toplevel, 10, 10);

    ei_widget_t button;
    button = ei_widget_create("button", toplevel, NULL, NULL);
    ei_button_configure(button, &((ei_size_t) {100, 50}),
                        &(ei_color_t) {0x88, 0x88, 0x88, 0xff},
                        &(int) {6},
                        &(int) {10},
                        &(ei_relief_t) {ei_relief_raised},
                        &(ei_string_t) {"b"}, NULL,
                        &(ei_color_t) {0x00, 0x00, 0x00, 0xff}, NULL, NULL, NULL, NULL,
                        &(ei_callback_t) {button_press}, NULL);
    ei_place_anchored_xy(button, ei_anc_southeast, 150, 150);

    ei_widget_t button2;
    button2 = ei_widget_create("button", ei_app_root_widget(), NULL, NULL);
    ei_rect_t img_rect = {{0,   0},
                          {200, 200}};
    ei_rect_ptr_t img_rect_ptr = &img_rect;
    ei_button_configure(button2, &((ei_size_t) {200, 133}),
                        &(ei_color_t) {0x88, 0x88, 0x88, 0xff},
                        &(int) {6},
                        &(int) {10},
                        &(ei_relief_t) {ei_relief_raised},
                        &(ei_string_t) {"\0"}, NULL,
                        &(ei_color_t) {0x00, 0x00, 0x00, 0xff}, NULL, NULL, &img_rect_ptr, NULL,
                        &(ei_callback_t) {button_press}, NULL);
    ei_place_xy(button2, 300, 300);
    ei_impl_button_t *tmp = (struct ei_impl_button_t *) button2;
    //strcpy(tmp->img_filename, "/user/9/linp/Downloads/rat.jpg");
    printf("STRING %s\n", tmp->img_filename);

    /* Run the application's main loop. */
    ei_app_run();

    /* We just exited from the main loop. Terminate the application (cleanup). */
    ei_app_free();

    return (EXIT_SUCCESS);
}
