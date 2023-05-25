#include <ei_application.h>
#include "utils_draw.h"
#include "ei_event.h"
#include "utils_general.h"


ei_point_t *get_array_arc(ei_point_t centre, int rayon, struct ei_corner_t corner, int sens) {
    double d_theta = M_PI / (2 * PRECISION - 1);
    ei_point_t *tab = malloc(PRECISION * sizeof(ei_point_t));
    int k = 0;
    int x, y;
    int f_x = 2 * corner.right - 1;
    int f_y = 1 - 2 * corner.top;
    if (sens == 1) {
        while (k < PRECISION) {
            x = (int) (rayon * cos(k * d_theta) * f_x);
            y = (int) (rayon * sin(k * d_theta) * f_y);
            ei_point_t point = {centre.x + x, centre.y + y};
            tab[k] = point;
            k++;
        }
    } else {
        while (k < PRECISION) {
            x = (int) (rayon * cos(k * d_theta) * f_x);
            y = (int) (rayon * sin(k * d_theta) * f_y);
            ei_point_t point = {centre.x + x, centre.y + y};
            tab[PRECISION - k - 1] = point;
            k++;
        }
    }
    return tab;
}

ei_point_t *rounded_frame(ei_rect_t rect, int r, ei_shape_t shape, int clockwise) {
    ei_widget_t root_widget = ei_app_root_widget();
    if (rect.top_left.y + r > root_widget->screen_location.size.height) {
        return NULL;
    }
    if (rect.top_left.y + rect.size.height > root_widget->screen_location.size.height) {
        shape = top_rounded;
        rect.size.height = rect.size.height -
                           (rect.top_left.y + rect.size.height - root_widget->screen_location.size.height);
    }

    ei_point_t top_left_center = {rect.top_left.x + r,
                                  rect.top_left.y + r};
    ei_point_t top_right_center = {rect.top_left.x + rect.size.width - r,
                                   rect.top_left.y + r};

    ei_point_t bottom_right_center = {rect.top_left.x + rect.size.width - r,
                                      rect.top_left.y + rect.size.height - r};
    ei_point_t bottom_left_center = {rect.top_left.x + r,
                                     rect.top_left.y + rect.size.height - r};

    if (shape == top_rounded) {
        bottom_right_center.x += r;
        bottom_right_center.y += r;
        bottom_left_center.x -= r;
        bottom_left_center.y += r;
    }

    struct ei_corner_t corner = {1, 0};
    ei_point_t *tab_tl = get_array_arc(top_left_center, r, corner, 1 * clockwise);
    corner.right = 1;
    ei_point_t *tab_tr = get_array_arc(top_right_center, r, corner, -1 * clockwise);
    corner.top = 0;
    ei_point_t *tab_br;
    ei_point_t *tab_bl;
    if (shape == top_rounded) {
        tab_br = get_array_arc(bottom_right_center, 0, corner, 1 * clockwise);
        corner.right = 0;
        tab_bl = get_array_arc(bottom_left_center, 0, corner, -1 * clockwise);
    } else {
        tab_br = get_array_arc(bottom_right_center, r, corner, 1 * clockwise);
        corner.right = 0;
        tab_bl = get_array_arc(bottom_left_center, r, corner, -1 * clockwise);
    }

    ei_point_t *tab = malloc(4 * PRECISION * sizeof(ei_point_t));

    size_t tab_size = PRECISION * sizeof(ei_point_t);
    if (clockwise == 1) {
        memcpy(tab, tab_bl, tab_size);
        memcpy(tab + PRECISION, tab_tl, tab_size);
        memcpy(tab + 2 * PRECISION, tab_tr, tab_size);
        memcpy(tab + 3 * PRECISION, tab_br, tab_size);
    } else {
        memcpy(tab, tab_bl, tab_size);
        memcpy(tab + PRECISION, tab_br, tab_size);
        memcpy(tab + 2 * PRECISION, tab_tr, tab_size);
        memcpy(tab + 3 * PRECISION, tab_tl, tab_size);
    }

    free(tab_tl);
    free(tab_tr);
    free(tab_bl);
    free(tab_br);
    return tab;
}

void draw_round_rect(ei_surface_t surface, ei_surface_t pick_surface,
                     ei_rect_t *ptr_rect, ei_color_t color,
                     ei_color_t pick_color, ei_relief_t relief,
                     int r, int border_width,
                     ei_rect_t *clipper, ei_shape_t shape, ei_color_t *border_color) {
    ei_point_t *tab = rounded_frame(*ptr_rect, r, shape, 1);
    if (tab == NULL) return;

    const int HALF_PRECISION = PRECISION / 2;
    size_t tab_size = PRECISION * sizeof(ei_point_t);
    size_t half_tab_size = HALF_PRECISION * sizeof(ei_point_t);

    ei_color_t color_lighter = {min(20 + color.red, 255),
                                min(20 + color.green, 255),
                                min(20 + color.blue, 255),
                                color.alpha};

    ei_color_t color_darker = {max(color.red - 20, 0),
                               max(color.green - 20, 0),
                               max(color.blue - 20, 0),
                               color.alpha};
    if (relief == ei_relief_sunken) {
        ei_color_t temp = color_lighter;
        color_lighter = color_darker;
        color_darker = temp;
    }

    if (border_color != NULL) {
        color_lighter = *border_color;
        color_darker = *border_color;
    }


    ei_size_t size_inside = {ptr_rect->size.width - 2 * border_width, ptr_rect->size.height - 2 * border_width};
    ei_point_t top_left_inside = (ei_point_t) {ptr_rect->top_left.x + border_width,
                                               ptr_rect->top_left.y + border_width};
    ei_rect_t rect_inside = {top_left_inside, size_inside};
    ei_point_t *tab_inside = rounded_frame(rect_inside, r, shape, -1);
    if (tab_inside == NULL) return;

    if (relief != ei_relief_none) {
        ei_point_t *tab_sup = malloc((4 * PRECISION + 2) * sizeof(ei_point_t));
        memcpy(tab_sup, tab + HALF_PRECISION, half_tab_size);
        memcpy(tab_sup + HALF_PRECISION, tab + PRECISION, tab_size);
        memcpy(tab_sup + HALF_PRECISION + PRECISION, tab + 2 * PRECISION, half_tab_size + sizeof(ei_point_t));
        memcpy(tab_sup + 2 * PRECISION + 1, tab_inside + 2 * PRECISION + HALF_PRECISION - 1,
               half_tab_size + sizeof(ei_point_t));
        memcpy(tab_sup + 2 * PRECISION + HALF_PRECISION + 2, tab_inside + 3 * PRECISION, tab_size);
        memcpy(tab_sup + 3 * PRECISION + HALF_PRECISION + 2, tab_inside, half_tab_size);

        ei_point_t *tab_inf = malloc((4 * PRECISION + 2) * sizeof(ei_point_t));
        memcpy(tab_inf, tab + 2 * PRECISION + HALF_PRECISION, half_tab_size);
        memcpy(tab_inf + HALF_PRECISION, tab + 3 * PRECISION, tab_size);
        memcpy(tab_inf + HALF_PRECISION + PRECISION, tab, half_tab_size + sizeof(ei_point_t));
        memcpy(tab_inf + 2 * PRECISION + 1, tab_inside + HALF_PRECISION - 1, half_tab_size + sizeof(ei_point_t));
        memcpy(tab_inf + 2 * PRECISION + HALF_PRECISION + 2, tab_inside + PRECISION, tab_size);
        memcpy(tab_inf + 3 * PRECISION + HALF_PRECISION + 2, tab_inside + 2 * PRECISION, half_tab_size);

        ei_draw_polygon(surface, tab_sup, 4 * PRECISION,
                        color_lighter, clipper);
        ei_draw_polygon(pick_surface, tab_sup, 4 * PRECISION, pick_color, clipper);

        ei_draw_polygon(surface, tab_inf, 4 * PRECISION,
                        color_darker, clipper);
        ei_draw_polygon(pick_surface, tab_inf, 4 * PRECISION, pick_color, clipper);


        ei_draw_polygon(surface, tab_inside, 4 * PRECISION,
                        color, clipper);
        ei_draw_polygon(pick_surface, tab_inside, 4 * PRECISION, pick_color, clipper);


        free(tab_inf);
        free(tab_sup);
    } else {
        ei_draw_polygon(surface, tab,
                        4 * PRECISION, color, clipper);
        ei_draw_polygon(pick_surface, tab, 4 * PRECISION, pick_color, clipper);

    }

    free(tab);
    free(tab_inside);
}

void ei_draw_image(ei_surface_t surface,
                   const ei_point_t *where,
                   ei_const_string_t img_filename,
                   ei_surface_t img,
                   ei_rect_t *clipper) {
    ei_surface_t surface_img;
    if (img == NULL) {
        surface_img = hw_image_load(img_filename, surface);
    } else {
        surface_img = img;
    }

    ei_rect_t rect_surface = {*where,
                              {clipper->size.width, clipper->size.height}};

    ei_rect_t rect_img = {{0,                   0},
                          {clipper->size.width, clipper->size.height}};

    hw_surface_lock(surface_img);
    ei_copy_surface(surface, &rect_surface, surface_img, &rect_img, false);
    hw_surface_unlock(surface_img);
    if (img == NULL) {
        hw_surface_free(surface_img);
    }
}

ei_rect_t get_text_rect(ei_widget_t widget, char *text, ei_font_t font, ei_anchor_t text_anchor) {
    int x_top = widget->screen_location.top_left.x;
    int y_top = widget->screen_location.top_left.y;
    int text_width, text_height;
    int button_width = widget->screen_location.size.width;
    int button_height = widget->screen_location.size.height;
    hw_text_compute_size(text, font, &text_width, &text_height);

//    if (text_width > button_width || text_height > button_height) {
//        return (ei_rect_t) {widget->screen_location.top_left,
//                            {0, 0}};
//    }

    ei_point_t text_location = {x_top, y_top};
    switch (text_anchor) {
        case ei_anc_none:
            break;
        case ei_anc_center:
            text_location.x += (button_width - text_width) / 2;
            text_location.y += (button_height - text_height) / 2;
            break;
        case ei_anc_north:
            text_location.x += (button_width - text_width) / 2;
            break;
        case ei_anc_northeast:
            text_location.x += button_width - text_width;
            break;
        case ei_anc_east:
            text_location.x += button_width - text_width;
            text_location.y += (button_height - text_height) / 2;
            break;
        case ei_anc_southeast:
            text_location.x += button_width - text_width;
            text_location.y += button_height - text_height;
            break;
        case ei_anc_south:
            text_location.x += (button_width - text_width) / 2;
            text_location.y += button_height - text_height;
            break;
        case ei_anc_southwest:
            text_location.y += button_height - text_height;
            break;
        case ei_anc_west:
            text_location.y += (button_height - text_height) / 2;
            break;
        case ei_anc_northwest:
            break;
    }
    return (ei_rect_t) {text_location, {text_width, text_height}};
}

ei_point_t get_where_img(ei_point_t top_left, int widget_width, int widget_height,
                         int img_width, int img_height, ei_anchor_t anchor) {
    ei_point_t top_left_img = top_left;
    switch (anchor) {
        case ei_anc_none:
            break;
        case ei_anc_center:
            top_left_img.x += (widget_width - img_width) / 2;
            top_left_img.y += (widget_height - img_height) / 2;
            break;
        case ei_anc_north:
            top_left_img.x += (widget_width - img_width) / 2;
            break;
        case ei_anc_northeast:
            top_left_img.x += widget_width - img_width;
            break;
        case ei_anc_east:
            top_left_img.x += widget_width - img_width;
            top_left_img.y += (widget_height - img_height) / 2;
            break;
        case ei_anc_southeast:
            top_left_img.x += widget_width - img_width;
            top_left_img.y += widget_height - img_height;
            break;
        case ei_anc_south:
            top_left_img.x += (widget_width - img_width) / 2;
            top_left_img.y += widget_height - img_height;
            break;
        case ei_anc_southwest:
            top_left_img.y += widget_height - img_height;
            break;
        case ei_anc_west:
            top_left_img.y += (widget_height - img_height) / 2;
            break;
        case ei_anc_northwest:
            break;
    }
    return top_left_img;
}
