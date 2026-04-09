#include <zmk/display/status_screen.h>
#include <zmk/display/widgets/layer_status.h>
#include <zmk/display/widgets/output_status.h>
#include <zmk/display/widgets/battery_status.h>

#include "widgets/heartbeat.h"
#include "widgets/profile_status.h"
#include "widgets/peripheral_battery.h"

#include <lvgl.h>

static struct zmk_widget_layer_status layer_widget;
static struct zmk_widget_output_status output_widget;
static struct zmk_widget_battery_status battery_widget;

lv_obj_t *zmk_display_status_screen() {
    lv_obj_t *screen;
    screen = lv_obj_create(NULL); 
    
    lv_obj_set_style_bg_color(screen, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);

    // 1. Dongle Battery (Top Left)
    zmk_widget_battery_status_init(&battery_widget, screen);
    lv_obj_set_style_text_color(zmk_widget_battery_status_obj(&battery_widget), lv_color_white(), 0);
    lv_obj_align(zmk_widget_battery_status_obj(&battery_widget), LV_ALIGN_TOP_LEFT, 5, 5);

    // 2. Output Status (Top Right)
    zmk_widget_output_status_init(&output_widget, screen);
    lv_obj_set_style_text_color(zmk_widget_output_status_obj(&output_widget), lv_color_white(), 0);
    lv_obj_align(zmk_widget_output_status_obj(&output_widget), LV_ALIGN_TOP_RIGHT, -5, 5);

    // 3. Heartbeat (Modular)
    heartbeat_init(screen);

    // 4. Peripheral Battery (Modular)
    peripheral_battery_init(screen);

    // 5. BLE Profile (Modular)
    profile_status_init(screen);

    // 6. Layer Status Widget (Bottom)
    zmk_widget_layer_status_init(&layer_widget, screen);
    lv_obj_set_style_text_color(zmk_widget_layer_status_obj(&layer_widget), lv_color_white(), 0);
    lv_obj_align(zmk_widget_layer_status_obj(&layer_widget), LV_ALIGN_CENTER, 0, 15);

    return screen;
}
