#include "heartbeat.h"
#include <stdio.h>

static lv_obj_t *label;
static uint32_t val = 0;

static void update_cb(lv_timer_t *timer) {
    char buf[20];
    snprintf(buf, sizeof(buf), "Live: %u", val++);
    lv_label_set_text(label, buf);
}

void heartbeat_init(lv_obj_t *parent) {
    label = lv_label_create(parent);
    lv_obj_set_style_text_color(label, lv_color_white(), 0);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 5);
    
    lv_timer_create(update_cb, 1000, NULL);
}
