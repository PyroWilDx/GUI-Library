#include <stdio.h>
#include <stdlib.h>

#include "ei_types.h"
#include "hw_interface.h"
#include "ei_draw.h"
#include "ei_implementation.h"
#include "ei_application.h"
#include "lsc.h"
#include "utils_general.h"
#include "utils_draw.h"

inline static void get_clipper(const ei_rect_t *clipper, int *x_min, int *y_min,
                               int *x_max, int *y_max, ei_size_t size) {
    if (clipper != NULL) {
        *x_min = clipper->top_left.x;
        *y_min = clipper->top_left.y;
        *x_max = *x_min + clipper->size.width;
        *y_max = *y_min + clipper->size.height;
    } else {
        *x_min = 0;
        *y_min = 0;
        *x_max = size.width;
        *y_max = size.height;
    }
}

inline static ei_color_t pixel_to_color(const int *ptr_pixel) {
    int val_color = *ptr_pixel;
    color_indexes indexes = get_color_indexes();
    int ir = indexes.ir;
    int ig = indexes.ig;
    int ib = indexes.ib;
    int ia = indexes.ia;
    ei_color_t color;
    color.red = (val_color >> (8 * ir)) & 0xFF;
    color.green = (val_color >> (8 * ig)) & 0xFF;
    color.blue = (val_color >> (8 * ib)) & 0xFF;
    color.alpha = (val_color >> (8 * ia)) & 0xFF;
    return color;
}

inline static void draw_pixel(int *ptr_pixel, ei_color_t *color) {
    if (color == NULL) {
        *ptr_pixel = 0;
        return;
    }

    color_indexes indexes = get_color_indexes();
    int ir = indexes.ir;
    int ig = indexes.ig;
    int ib = indexes.ib;
    int ia = indexes.ia;
    int old_color = *ptr_pixel;
    int alpha = color->alpha;
    int cpl_alpha = 255 - alpha;
    int old_sr = (old_color >> (8 * ir)) & 0xFF;
    int sr = (alpha * color->red + cpl_alpha * old_sr) / 255;
    int old_sg = (old_color >> (8 * ig)) & 0xFF;
    int sg = (alpha * color->green + cpl_alpha * old_sg) / 255;
    int old_sb = (old_color >> (8 * ib)) & 0xFF;
    int sb = (alpha * color->blue + cpl_alpha * old_sb) / 255;
    int val_color = (sr << (8 * ir)) + (sg << (8 * ig)) +
                    (sb << (8 * ib)) + (alpha << (8 * ia));
    *ptr_pixel = val_color;
}

void ei_fill(ei_surface_t surface,
             const ei_color_t *color_tmp,
             const ei_rect_t *clipper) {
    ei_color_t *color = (ei_color_t *) color_tmp;

    int *pixel_0 = (int *) hw_surface_get_buffer(surface);
    int *ptr_pixel = pixel_0;

    ei_size_t size = hw_surface_get_size(surface);
    int length = size.height * size.width;
    int *max = ptr_pixel + length;

    int x_min, y_min, x_max, y_max;
    get_clipper(clipper, &x_min, &y_min, &x_max, &y_max, size);

    while (ptr_pixel < max) {
        if (pixel_is_in_clipper(x_min, y_min, x_max, y_max, ptr_pixel, pixel_0, size.width)) {
            draw_pixel(ptr_pixel, color);
        }
        ptr_pixel++;
    }
}

static void ei_draw_line(int *pixel_0,
                         ei_size_t size,
                         ei_point_t p0, ei_point_t p1,
                         ei_color_t color,
                         int x_min, int y_min,
                         int x_max, int y_max) {
    int x0 = p0.x;
    int x1 = p1.x;
    int y0 = p0.y;
    int y1 = p1.y;

    int dx = x1 - x0;
    int dy = y1 - y0;

    if (dx < 0) {
        swap(&x0, &x1);
        swap(&y0, &y1);
        dx = -dx;
        dy = -dy;
    }

    int val_width = size.width;
    int increment_y = 1;
    if (dy < 0) {
        val_width = -val_width;
        dy = -dy;
        increment_y = -1;
    }

    if (!point_is_in_clipper(x_min, y_min, x_max, y_max, x0, y0) ||
        !point_is_in_clipper(x_min, y_min, x_max, y_max, x1, y1)) {

        if (x0 > x_max || y0 > y_max || x1 < x_min || y1 < y_min) return;

        int x_tmp = x0;
        int y_tmp = y0;
        int add_x = (dx == 0) ? 0 : 1;
        int incr_y_tmp = (int) round((double) (float) dy / dx);
        while (x_tmp < x_min || y_tmp < y_min) {
            x_tmp += add_x;
            y_tmp += incr_y_tmp;
        }
        if (x_tmp >= x_min) {
            x0 = x_tmp;
            y0 = y_tmp;
        }

        x_tmp = x1;
        y_tmp = y1;
        while (x_tmp > x_max || y_tmp > y_max) {
            x_tmp -= add_x;
            y_tmp -= incr_y_tmp;
        }
        if (x_tmp == x_max) {
            x1 = x_tmp;
            y1 = y_tmp;
        }
    }

    int e = 0;

    int *ptr_pixel = get_pixel_from_coordinates(pixel_0, x0, y0, size.width);
    if (dx == 0) {
        for (int j = y0; j != y1; j += increment_y) {
            draw_pixel(ptr_pixel, &color);
            ptr_pixel += val_width;
        }
        return;
    }

    int j = y0;
    int i = x0;
    ptr_pixel = get_pixel_from_coordinates(pixel_0, x0, y0, size.width);
    if (abs(dx) <= abs(dy)) {
        for (j = y0; j != y1; j += increment_y) {
            draw_pixel(ptr_pixel, &color);
            ptr_pixel += val_width;

            e += dx;
            if (2 * e > dy) {
                i++;
                ptr_pixel++;
                e -= dy;
            }
        }

    } else {
        for (i = x0; i < x1; i++) {
            draw_pixel(ptr_pixel, &color);
            ptr_pixel++;

            e += dy;
            if (2 * e > dx) {
                j += increment_y;
                ptr_pixel += val_width;
                e -= dx;
            }
        }
    }
}

void ei_draw_polyline(ei_surface_t surface,
                      ei_point_t *point_array,
                      size_t point_array_size,
                      ei_color_t color,
                      const ei_rect_t *clipper) {
    int *pixel_0 = (int *) hw_surface_get_buffer(surface);
    ei_size_t size = hw_surface_get_size(surface);
    if (point_array_size == 0) return;

    int x_min, y_min, x_max, y_max;
    get_clipper(clipper, &x_min, &y_min, &x_max, &y_max, size);

    if (point_array_size == 1) {
        ei_point_t p = point_array[0];

        int *ptr_pixel = pixel_0 + p.x + size.width * p.y;
        draw_pixel(ptr_pixel, &color);
        return;
    }

    size_t i = 0;
    while (i < point_array_size - 1) {
        ei_draw_line(pixel_0, size, point_array[i], point_array[i + 1],
                     color, x_min, y_min, x_max, y_max);
        i++;
    }
    ei_draw_line(pixel_0, size, point_array[point_array_size - 1], point_array[0],
                 color, x_min, y_min, x_max, y_max);
}

static struct lsc_tca *update_tca(struct lsc_tca *tca, int y, int *pixel_0, ei_size_t size,
                                  int x_min, int y_min,
                                  int x_max, int y_max,
                                  ei_color_t color) {
    bool draw = true;
    int x0, x1;
    ei_point_t p0, p1;
    struct lsc_tca *tmp;
    struct lsc_tca *current_lsc_tca = tca;
    while (current_lsc_tca != NULL) {
        if (draw && current_lsc_tca->next != NULL) {
            x0 = current_lsc_tca->x_inter > x_max ? x_max : current_lsc_tca->x_inter;
            x1 = current_lsc_tca->next->x_inter > x_max ? x_max : current_lsc_tca->next->x_inter;
            p0.x = x0;
            p0.y = y;
            p1.x = x1;
            p1.y = y;
            ei_draw_line(pixel_0, size, p0, p1, color, x_min, y_min, x_max, y_max);
        }
        current_lsc_tca->e += abs(current_lsc_tca->dx);

        if (2 * current_lsc_tca->e > current_lsc_tca->dy) {
            current_lsc_tca->e -= current_lsc_tca->dy * abs(current_lsc_tca->sign_dx);
            current_lsc_tca->x_inter += current_lsc_tca->sign_dx;
        }

        tmp = current_lsc_tca->next;

        if (current_lsc_tca->y_max == y + 1) {
            tca = remove_lsc_tca(tca, current_lsc_tca);
        }
        current_lsc_tca = tmp;
        draw = !draw;
    }
    return tca;
}

void ei_draw_polygon(ei_surface_t surface,
                     ei_point_t *point_array,
                     size_t point_array_size,
                     ei_color_t color,
                     const ei_rect_t *clipper) {
    if (point_array_size == 0) return;

    int *pixel_0 = (int *) hw_surface_get_buffer(surface);
    ei_size_t size = hw_surface_get_size(surface);

    int x_min, y_min, x_max, y_max;
    get_clipper(clipper, &x_min, &y_min, &x_max, &y_max, size);

    int y = 0;
    struct lsc **tc = get_lsc_tc(point_array, point_array_size, size);
    struct lsc_tca *tca = init_tca(tc, &y, size.height);
    struct lsc *lsc;

    while (y < size.height - 1) {
        tca = update_tca(tca, y, pixel_0, size, x_min, y_min, x_max, y_max, color);
        y++;
        lsc = tc[y];
        while (lsc != NULL) {
            tca = insert_lsc_tca(tca, lsc);
            lsc = lsc->next;
        }
    }

    free_tc(tc, size.height);
    free_lsc_tca(tca);
}

void ei_draw_text(ei_surface_t surface,
                  const ei_point_t *where,
                  ei_const_string_t text,
                  ei_font_t font,
                  ei_color_t color,
                  const ei_rect_t *clipper) {
    ei_font_t real_font = font;
    if (real_font == NULL) real_font = ei_default_font;

    ei_surface_t text_surface = hw_text_create_surface(text, real_font, color);

    ei_rect_t rect_surface = {*where,  {0, 0}};
    ei_rect_t rect_text = {{0, 0}, {0, 0}};
    if (clipper != NULL) {
        rect_surface.size = clipper->size;
        rect_text.size = clipper->size;
    } else {
        ei_size_t size = hw_surface_get_size(text_surface);
        rect_surface.size = size;
        rect_text.size = size;
    }
    hw_surface_lock(text_surface);
    ei_copy_surface(surface, &rect_surface, text_surface, &rect_text, true);
    hw_surface_unlock(text_surface);
    hw_surface_free(text_surface);
}

int ei_copy_surface(ei_surface_t destination,
                    const ei_rect_t *dst_rect,
                    ei_surface_t source,
                    const ei_rect_t *src_rect,
                    bool alpha) {
    if (dst_rect->size.width == 0 || dst_rect->size.height == 0) return 0;

    ei_rect_t rect_surface_dst = hw_surface_get_rect(destination);
    ei_rect_t rect_surface_src = hw_surface_get_rect(source);

    int dst_w = rect_surface_dst.size.width;
    int src_w = rect_surface_src.size.width;

    int *pixel_0_dst_surface = (int *) hw_surface_get_buffer(destination);
    int *pixel_0_src_surface = (int *) hw_surface_get_buffer(source);

    int x_min_dst, y_min_dst, x_max_dst, y_max_dst;
    if (dst_rect == NULL) {
        x_min_dst = rect_surface_dst.top_left.x;
        y_min_dst = rect_surface_dst.top_left.y;
        x_max_dst = x_min_dst + rect_surface_dst.size.width;
        y_max_dst = y_min_dst + rect_surface_dst.size.height;
    } else {
        x_min_dst = dst_rect->top_left.x;
        y_min_dst = dst_rect->top_left.y;
        x_max_dst = x_min_dst + dst_rect->size.width;
        y_max_dst = y_min_dst + dst_rect->size.height;
    }

    int x_min_src, y_min_src, x_max_src, y_max_src;
    if (src_rect == NULL) {
        x_min_src = rect_surface_src.top_left.x;
        y_min_src = rect_surface_src.top_left.y;
        x_max_src = x_min_src + rect_surface_src.size.width;
        y_max_src = y_min_src + rect_surface_src.size.height;
    } else {
        x_min_src = src_rect->top_left.x;
        y_min_src = src_rect->top_left.y;
        x_max_src = x_min_src + src_rect->size.width;
        y_max_src = y_min_src + src_rect->size.height;
    }

    if ((x_max_src - x_min_src) != (x_max_dst - x_min_dst) ||
        (y_max_src - y_min_src) != (y_max_dst - y_min_dst)) {
        return 1;
    }

    y_max_src = min(y_max_src, hw_surface_get_size(source).height);
    x_max_src = min(x_max_src, hw_surface_get_size(source).width);

    ei_size_t root_size = ei_app_root_widget()->screen_location.size;
    int max_width = root_size.width;
    int max_height = root_size.height;

    int y_dst = y_min_dst;
    int x_dst = x_min_dst;
    int *pixel_dst;
    int *pixel_src;
    for (int y_src = y_min_src; y_src < y_max_src; y_src++) {
        if (y_dst > y_max_dst) break;
        for (int x_src = x_min_src; x_src < x_max_src; x_src++) {
            if (x_dst > x_max_dst) break;
            pixel_dst = get_pixel_from_coordinates(pixel_0_dst_surface, x_dst, y_dst, dst_w);
            pixel_src = get_pixel_from_coordinates(pixel_0_src_surface, x_src, y_src, src_w);
            if (alpha) {
                ei_color_t color = pixel_to_color(pixel_src);
                draw_pixel(pixel_dst, &color);
            } else {
                *pixel_dst = *pixel_src;
            }
            x_dst++;

            if (x_dst == max_width) break;
        }
        x_dst = x_min_dst;
        y_dst++;
        if (y_dst == max_height) break;
    }

    return 0;
}