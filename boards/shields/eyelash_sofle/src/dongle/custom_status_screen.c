#include <zmk/display/status_screen.h>
#include <zmk/display/widgets/layer_status.h>
#include <zmk/endpoints.h>
#include <lvgl.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

static struct zmk_widget_layer_status layer_widget;
static lv_obj_t *output_label;

static void update_output_status(lv_timer_t *timer) {
    // In ZMK v0.3.0, zmk_endpoints_get_active_endpoint returns an int/enum
    // We use a safe check here. 0 is usually USB, 1 is BLE.
    int endpoint = (int)zmk_endpoints_get_active_endpoint();
    
    if (endpoint == 0) { // ZMK_ENDPOINT_USB
        lv_label_set_text(output_label, "Output: USB");
    } else if (endpoint == 1) { // ZMK_ENDPOINT_BLE
        lv_label_set_text(output_label, "Output: BLE");
    } else {
        lv_label_set_text(output_label, "Output: Unknown");
    }
}

lv_obj_t *zmk_display_status_screen() {
    lv_obj_t *screen;
    screen = lv_obj_create(NULL); 
    
    // Set background to black
    lv_obj_set_style_bg_color(screen, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);

    // 1. Custom Dynamic Output Label (Text instead of icons)
    output_label = lv_label_create(screen);
    lv_obj_set_style_text_color(output_label, lv_color_white(), 0);
    lv_obj_set_style_text_font(output_label, &lv_font_montserrat_12, 0);
    lv_obj_align(output_label, LV_ALIGN_TOP_MID, 0, 5);
    
    // Initial update
    update_output_status(NULL);
    
    // Update every 1 second
    lv_timer_create(update_output_status, 1000, NULL);

    // 2. Layer Title
    lv_obj_t *header = lv_label_create(screen);
    lv_label_set_text(header, "LAYER:");
    lv_obj_set_style_text_color(header, lv_color_white(), 0);
    lv_obj_align(header, LV_ALIGN_CENTER, 0, 0);

    // 3. Layer Status Widget
    zmk_widget_layer_status_init(&layer_widget, screen);
    lv_obj_set_style_text_color(zmk_widget_layer_status_obj(&layer_widget), lv_color_white(), 0);
    lv_obj_align(zmk_widget_layer_status_obj(&layer_widget), LV_ALIGN_CENTER, 0, 15);

    return screen;
}
