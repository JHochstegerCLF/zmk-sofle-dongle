#include <zmk/display/status_screen.h>
#include <zmk/display/widgets/layer_status.h>
#include <zmk/display/widgets/peripheral_status.h>
#include <lvgl.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

static struct zmk_widget_layer_status layer_widget;
static struct zmk_widget_peripheral_status peripheral_widget;
static lv_obj_t *counter_label;
static uint32_t counter_val = 0;

static void update_counter(lv_timer_t *timer) {
    char buf[16];
    snprintf(buf, sizeof(buf), "Booted: %us", counter_val++);
    lv_label_set_text(counter_label, buf);
}

lv_obj_t *zmk_display_status_screen() {
    lv_obj_t *screen;
    screen = lv_obj_create(NULL); 
    
    // Header
    lv_obj_t *header = lv_label_create(screen);
    lv_label_set_text(header, "SOFLE DONGLE");
    lv_obj_set_style_text_color(header, lv_color_white(), 0);
    lv_obj_align(header, LV_ALIGN_TOP_MID, 0, 0);

    // Peripheral Status Widget (Shows Left/Right connected)
    zmk_widget_peripheral_status_init(&peripheral_widget, screen);
    lv_obj_align(zmk_widget_peripheral_status_obj(&peripheral_widget), LV_ALIGN_CENTER, 0, -10);

    // Layer Status Widget
    zmk_widget_layer_status_init(&layer_widget, screen);
    lv_obj_set_style_text_color(zmk_widget_layer_status_obj(&layer_widget), lv_color_white(), 0);
    lv_obj_align(zmk_widget_layer_status_obj(&layer_widget), LV_ALIGN_CENTER, 0, 10);

    // Simple Dynamic Uptime Counter
    counter_label = lv_label_create(screen);
    lv_label_set_text(counter_label, "Booting...");
    lv_obj_set_style_text_color(counter_label, lv_color_white(), 0);
    lv_obj_set_style_text_font(counter_label, &lv_font_montserrat_10, 0);
    lv_obj_align(counter_label, LV_ALIGN_BOTTOM_MID, 0, 0);

    lv_timer_create(update_counter, 1000, NULL);

    LOG_INF("Custom status screen with widgets initialized.");
    return screen;
}
