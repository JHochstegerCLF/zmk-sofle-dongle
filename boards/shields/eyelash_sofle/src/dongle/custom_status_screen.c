#include <zmk/display/status_screen.h>
#include <zmk/display/widgets/layer_status.h>
#include <zmk/display/widgets/output_status.h>
#include <zmk/display/widgets/battery_status.h>
#include <zmk/ble.h>
#include <lvgl.h>

// Forward declare ZMK functions that might not have headers available
extern uint8_t zmk_split_get_peripheral_battery_level(uint8_t index);

static struct zmk_widget_layer_status layer_widget;
static struct zmk_widget_output_status output_widget;
static struct zmk_widget_battery_status battery_widget;
static lv_obj_t *profile_label;
static lv_obj_t *peripheral_battery_label;

static void update_status(lv_timer_t *timer) {
    // 1. Update Profile
    uint8_t profile = zmk_ble_active_profile_index();
    char prof_buf[20];
    snprintf(prof_buf, sizeof(prof_buf), "Profile: %u", profile + 1);
    lv_label_set_text(profile_label, prof_buf);

    // 2. Update Peripheral Batteries
    // Index 0 is typically Left, 1 is Right
    uint8_t l_bat = zmk_split_get_peripheral_battery_level(0);
    uint8_t r_bat = zmk_split_get_peripheral_battery_level(1);
    
    char bat_buf[32];
    snprintf(bat_buf, sizeof(bat_buf), "L:%u%% R:%u%%", l_bat, r_bat);
    lv_label_set_text(peripheral_battery_label, bat_buf);
}

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

    // 3. Peripheral Battery (Top Center)
    peripheral_battery_label = lv_label_create(screen);
    lv_obj_set_style_text_color(peripheral_battery_label, lv_color_white(), 0);
    lv_obj_set_style_text_font(peripheral_battery_label, &lv_font_montserrat_12, 0);
    lv_obj_align(peripheral_battery_label, LV_ALIGN_TOP_MID, 0, 5);

    // 4. BLE Profile Number (Center)
    profile_label = lv_label_create(screen);
    lv_obj_set_style_text_color(profile_label, lv_color_white(), 0);
    lv_obj_set_style_text_font(profile_label, &lv_font_montserrat_12, 0);
    lv_obj_align(profile_label, LV_ALIGN_CENTER, 0, -10);
    
    update_status(NULL);
    lv_timer_create(update_status, 5000, NULL);

    // 5. Layer Status Widget (Bottom)
    zmk_widget_layer_status_init(&layer_widget, screen);
    lv_obj_set_style_text_color(zmk_widget_layer_status_obj(&layer_widget), lv_color_white(), 0);
    lv_obj_align(zmk_widget_layer_status_obj(&layer_widget), LV_ALIGN_CENTER, 0, 10);

    return screen;
}
