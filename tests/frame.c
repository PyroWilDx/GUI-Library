#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget_configure.h"
#include "../implem/ei_implementation.h"
#include "../implem/utils_draw.h"


/*
 * ei_main --
 *
 *	Main function of the application.
 */
int main(int argc, char **argv) {
    ei_widget_t frame;

    /* Create the application and change the color of the background. */
    ei_app_create((ei_size_t) {600, 600}, false);
    ei_frame_set_bg_color(ei_app_root_widget(), (ei_color_t) {0x52, 0x7f, 0xb4, 0xff});

    /* Create, configure and place the frame on screen. */
    frame = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);

    ei_frame_configure(frame, &(ei_size_t) {300, 200},
                       &(ei_color_t) {0x88, 0x88, 0x88, 0xff},
                       &(int) {6},
                       &(ei_relief_t) {ei_relief_raised}, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_place_anchored_xy(frame, ei_anc_none, 500, 200);

    ei_widget_t frame0 = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
    ei_frame_configure(frame0, &(ei_size_t) {600, 600},
                       &(ei_color_t) {0, 100, 0, 20},
                       &(int) {6},
                       &(ei_relief_t) {ei_relief_sunken}, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
//    ei_place_xy(frame, 150, 200);
    ei_place_anchored_xy(frame0, ei_anc_none, 300, 300);

    /* Run the application's main loop. */

    ei_app_run();


    ei_surface_t root_window = ei_app_root_surface();
    hw_surface_lock(root_window);
    ei_rect_t dst_rect = {{50, 50},
                          {100, 200}};
    ei_rect_t src_rect = {{450,  250},
                          {100, 200}};
    ei_copy_surface(root_window, &dst_rect, root_window, &src_rect, true);

    ei_point_t where = {100, 100};
    ei_color_t color = {255, 255, 255, 255};
    const char *text = "?!@Aoqçà&é\\éé&";
    ei_draw_text(root_window, &where,text,NULL,color,NULL);

//    ei_point_t where2 = {200, 100};
//    ei_rect_t clipper = {where2, {100, 100}};
//    ei_draw_image(root_window, &where2, "/user/7/borgeta/Downloads/rat.jpg", NULL, &clipper);


    hw_surface_unlock(root_window);
    hw_surface_update_rects(root_window, NULL);

    getchar();


    /* We just exited from the main loop. Terminate the application (cleanup). */
    ei_app_free();

    return (EXIT_SUCCESS);
}
