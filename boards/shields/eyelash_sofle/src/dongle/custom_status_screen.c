#include <zmk/display/status_screen.h>
#include <zmk/display/widgets/layer_status.h>
#include <zmk/display/widgets/output_status.h>
#include <zmk/ble.h>
#include <lvgl.h>

static struct zmk_widget_layer_status layer_widget;
static struct zmk_widget_output_status output_widget;
static lv_obj_t *profile_label;
static lv_obj_t *left_bat_label;

lv_obj_t *zmk_display_status_screen() {
    lv_obj_t *screen;
    screen = lv_obj_create(NULL); 
    
    lv_obj_set_style_bg_color(screen, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);

    // 1. Output Status (Top Left)
    zmk_widget_output_status_init(&output_widget, screen);
    lv_obj_set_style_text_color(zmk_widget_output_status_obj(&output_widget), lv_color_white(), 0);
    lv_obj_align(zmk_widget_output_status_obj(&output_widget), LV_ALIGN_TOP_LEFT, 5, 5);

    // 2. Battery Label (Center Top) - Static for now to test typing
    left_bat_label = lv_label_create(screen);
    lv_label_set_text(left_bat_label, "L: --%");
    lv_obj_set_style_text_color(left_bat_label, lv_color_white(), 0);
    lv_obj_set_style_text_font(left_bat_label, &lv_font_montserrat_12, 0);
    lv_obj_align(left_bat_label, LV_ALIGN_TOP_MID, 0, 5);

    // 3. BLE Profile Number (Center)
    profile_label = lv_label_create(screen);
    lv_label_set_text(profile_label, "Profile: 1");
    lv_obj_set_style_text_color(profile_label, lv_color_white(), 0);
    lv_obj_set_style_text_font(profile_label, &lv_font_montserrat_12, 0);
    lv_obj_align(profile_label, LV_ALIGN_CENTER, 0, -10);

    // 4. Layer Status Widget (Bottom)
    zmk_widget_layer_status_init(&layer_widget, screen);
    lv_obj_set_style_text_color(zmk_widget_layer_status_obj(&layer_widget), lv_color_white(), 0);
    lv_obj_align(zmk_widget_layer_status_obj(&layer_widget), LV_ALIGN_CENTER, 0, 10);

    return screen;
}
