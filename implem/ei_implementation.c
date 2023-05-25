#include "ei_implementation.h"
#include "hw_interface.h"

color_indexes indexes;

void init_color_indexes(ei_surface_t surface) {
    hw_surface_get_channel_indices(surface, &indexes.ir, &indexes.ig, &indexes.ib, &indexes.ia);
}

color_indexes get_color_indexes() {
    return indexes;
}
