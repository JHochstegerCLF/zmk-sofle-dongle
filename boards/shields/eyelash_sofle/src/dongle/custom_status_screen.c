#include <zmk/display/status_screen.h>
#include <zmk/display/widgets/layer_status.h>
#include <zmk/display/widgets/output_status.h>
#include <zmk/display/widgets/battery_status.h>
#include <zmk/event_manager.h>
#include <zmk/events/battery_state_changed.h>
#include <zmk/ble.h>
#include <lvgl.h>

static struct zmk_widget_layer_status layer_widget;
static struct zmk_widget_output_status output_widget;
static struct zmk_widget_battery_status battery_widget;
static lv_obj_t *profile_label;
static lv_obj_t *heartbeat_label;
static uint32_t heartbeat_val = 0;

static int battery_listener(const zmk_event_t *eh) {
    // In some ZMK versions, the peripheral battery levels might come through this generic event
    // or be handled by the built-in widgets. We'll add logging if we had a console.
    return 0;
}

ZMK_LISTENER(battery_status, battery_listener);
ZMK_SUBSCRIPTION(battery_status, zmk_battery_state_changed);

static void update_status(lv_timer_t *timer) {
    // Update Profile
    uint8_t profile = zmk_ble_active_profile_index();
    char prof_buf[20];
    snprintf(prof_buf, sizeof(prof_buf), "Profile: %u", profile + 1);
    lv_label_set_text(profile_label, prof_buf);

    // Update Heartbeat (to confirm screen is alive and updating)
    char hb_buf[20];
    snprintf(hb_buf, sizeof(hb_buf), "Live: %u", heartbeat_val++);
    lv_label_set_text(heartbeat_label, hb_buf);
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

    // 3. Heartbeat (Center Top)
    heartbeat_label = lv_label_create(screen);
    lv_obj_set_style_text_color(heartbeat_label, lv_color_white(), 0);
    lv_obj_set_style_text_font(heartbeat_label, &lv_font_montserrat_12, 0);
    lv_obj_align(heartbeat_label, LV_ALIGN_TOP_MID, 0, 5);

    // 4. BLE Profile Number (Center)
    profile_label = lv_label_create(screen);
    lv_obj_set_style_text_color(profile_label, lv_color_white(), 0);
    lv_obj_set_style_text_font(profile_label, &lv_font_montserrat_12, 0);
    lv_obj_align(profile_label, LV_ALIGN_CENTER, 0, -10);
    
    update_status(NULL);
    lv_timer_create(update_status, 1000, NULL);

    // 5. Layer Status Widget (Bottom)
    zmk_widget_layer_status_init(&layer_widget, screen);
    lv_obj_set_style_text_color(zmk_widget_layer_status_obj(&layer_widget), lv_color_white(), 0);
    lv_obj_align(zmk_widget_layer_status_obj(&layer_widget), LV_ALIGN_CENTER, 0, 10);

    return screen;
}
