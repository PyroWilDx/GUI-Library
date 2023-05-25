#include <string.h>

#include "ei_widgetclass.h"
#include "../implem/ei_implementation.h"

ei_widgetclass_t *root_wc = NULL;

size_t ei_widget_struct_size() {
    return sizeof(ei_impl_widget_t);
}

void ei_widgetclass_register(ei_widgetclass_t *widget_class) {
    if (widget_class == NULL) return;

    widget_class->next = root_wc;
    root_wc = widget_class;
}

ei_widgetclass_t *ei_widgetclass_from_name(ei_const_string_t name) {
    if (name == NULL) {
        return root_wc;
    }

    ei_widgetclass_t *tmp = root_wc;
    while (tmp != NULL) {
        if (strcmp(tmp->name, name) == 0) {
            return tmp;
        }
        tmp = tmp->next;
    }
    return NULL;
}
