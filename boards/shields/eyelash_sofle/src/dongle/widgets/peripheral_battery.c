#include "peripheral_battery.h"
#include <stdio.h>

// Note: Peripheral battery fetching is complex in raw C without specific headers.
// For now, we will provide a placeholder that we can wire up once the basic 
// modular build is confirmed working.
static lv_obj_t *label;

void peripheral_battery_init(lv_obj_t *parent) {
    label = lv_label_create(parent);
    lv_obj_set_style_text_color(label, lv_color_white(), 0);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 20);
    lv_label_set_text(label, "L:--% R:--%");
}
