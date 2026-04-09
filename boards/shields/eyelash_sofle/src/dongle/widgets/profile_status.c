#include "profile_status.h"
#include <zmk/ble.h>
#include <stdio.h>

static lv_obj_t *label;

static void update_cb(lv_timer_t *timer) {
    uint8_t profile = zmk_ble_active_profile_index();
    char buf[20];
    snprintf(buf, sizeof(buf), "Profile: %u", profile + 1);
    lv_label_set_text(label, buf);
}

void profile_status_init(lv_obj_t *parent) {
    label = lv_label_create(parent);
    lv_obj_set_style_text_color(label, lv_color_white(), 0);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, -10);
    
    lv_timer_create(update_cb, 1000, NULL);
}
