#include <zmk/display/status_screen.h>
#include <lvgl.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

static lv_obj_t *heartbeat_label;
static uint32_t heartbeat_val = 0;

static void update_status(lv_timer_t *timer) {
    char hb_buf[20];
    snprintf(hb_buf, sizeof(hb_buf), "Live: %u", heartbeat_val++);
    lv_label_set_text(heartbeat_label, hb_buf);
}

lv_obj_t *zmk_display_status_screen() {
    lv_obj_t *screen;
    screen = lv_obj_create(NULL); 
    
    lv_obj_set_style_bg_color(screen, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);

    // ONLY the Heartbeat label, centered
    heartbeat_label = lv_label_create(screen);
    lv_obj_set_style_text_color(heartbeat_label, lv_color_white(), 0);
    lv_obj_set_style_text_font(heartbeat_label, &lv_font_montserrat_12, 0);
    lv_obj_align(heartbeat_label, LV_ALIGN_CENTER, 0, 0);
    
    lv_timer_create(update_status, 1000, NULL);

    return screen;
}
