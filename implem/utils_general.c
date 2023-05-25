#include "ei_utils.h"
#include "utils_general.h"


void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

ei_rect_t intersect_rect(ei_rect_t *rect1, ei_rect_t *rect2) {
    ei_rect_t rect;
    rect.top_left.x = max(rect1->top_left.x, rect2->top_left.x);
    rect.top_left.y = max(rect1->top_left.y, rect2->top_left.y);
    rect.size.width =
            min(rect1->top_left.x + rect1->size.width,
                rect2->top_left.x + rect2->size.width) - rect.top_left.x;
    rect.size.height =
            min(rect1->top_left.y + rect1->size.height,
                rect2->top_left.y + rect2->size.height) - rect.top_left.y;

    if (rect.size.width < 0 || rect.size.height < 0) {
        return ei_rect_zero();
    }
    return rect;
}

bool rect1_intersects_rect2(ei_rect_t *rect1, ei_rect_t *rect2) {
    int x1 = rect1->top_left.x;
    int y1 = rect1->top_left.y;
    int x2 = rect2->top_left.x;
    int y2 = rect2->top_left.y;
    int w1 = rect1->size.width;
    int h1 = rect1->size.height;
    int w2 = rect2->size.width;
    int h2 = rect2->size.height;

    if ((x1 + w1 <= x2) || (x1 >= x2 + w2)) return false;
    if ((y1 + h1 <= y2) || (y1 >= y2 + h2)) return false;

    return true;
}

bool rect1_in_rect2(ei_rect_t *rect1, ei_rect_t *rect2) {
    int x1 = rect1->top_left.x;
    int y1 = rect1->top_left.y;
    int x2 = rect2->top_left.x;
    int y2 = rect2->top_left.y;
    int w1 = rect1->size.width;
    int h1 = rect1->size.height;
    int w2 = rect2->size.width;
    int h2 = rect2->size.height;

    return (x1 >= x2 && (x1 + w1 <= x2 + w2)) &&
           (y1 >= y2 && (y1 + h1 <= y2 + h2));
}

bool rect_not_0(ei_rect_t *rect) {
    ei_size_t size = rect->size;
    return (size.width != 0 && size.height != 0);
}
