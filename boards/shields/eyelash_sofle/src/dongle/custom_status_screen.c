#include <zmk/display/status_screen.h>
#include <zmk/display/widgets/layer_status.h>
#include <lvgl.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

static struct zmk_widget_layer_status layer_widget;
static lv_obj_t *counter_label;
static uint32_t counter_val = 0;

static void update_counter(lv_timer_t *timer) {
    char buf[16];
    LOG_INF("Updating counter to %u", counter_val);
    snprintf(buf, sizeof(buf), "Count: %u", counter_val++);
    lv_label_set_text(counter_label, buf);
}

lv_obj_t *zmk_display_status_screen() {
    LOG_INF("Custom status screen initializing...");

    lv_obj_t *screen;
    screen = lv_obj_create(NULL); 

    // Static Title
    lv_obj_t *title = lv_label_create(screen);
    lv_label_set_text(title, "SOFLE DONGLE");
    lv_obj_set_style_text_color(title, lv_color_white(), 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 5);

    // Layer Widget
    zmk_widget_layer_status_init(&layer_widget, screen);
    lv_obj_set_style_text_color(zmk_widget_layer_status_obj(&layer_widget), lv_color_white(), 0);
    lv_obj_align(zmk_widget_layer_status_obj(&layer_widget), LV_ALIGN_CENTER, 0, 0);

    // Simple Dynamic Component: A counter label
    counter_label = lv_label_create(screen);
    lv_label_set_text(counter_label, "Count: 0");
    lv_obj_set_style_text_color(counter_label, lv_color_white(), 0);
    lv_obj_align(counter_label, LV_ALIGN_BOTTOM_MID, 0, -5);

    // Create a timer to update the counter every 1000ms
    lv_timer_create(update_counter, 1000, NULL);

    LOG_INF("Custom status screen initialized.");
    return screen;
}
