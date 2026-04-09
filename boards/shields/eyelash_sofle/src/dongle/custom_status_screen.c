#include <zmk/display/status_screen.h>
#include <zmk/display/widgets/layer_status.h>
#include <zmk/display/widgets/battery_status.h>
#include <lvgl.h>

static struct zmk_widget_layer_status layer_widget;
static struct zmk_widget_battery_status battery_widget;

lv_obj_t *zmk_display_status_screen() {
    lv_obj_t *screen;
    screen = lv_obj_create(NULL); 
    
    // Set background to black
    lv_obj_set_style_bg_color(screen, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);

    // 1. Add the Battery Status widget
    zmk_widget_battery_status_init(&battery_widget, screen);
    lv_obj_align(zmk_widget_battery_status_obj(&battery_widget), LV_ALIGN_TOP_RIGHT, -5, 5);

    lv_obj_t *bat_label = lv_label_create(screen);
    lv_label_set_text(bat_label, "BAT");
    lv_obj_set_style_text_color(bat_label, lv_color_white(), 0);
    lv_obj_set_style_text_font(bat_label, &lv_font_montserrat_10, 0);
    lv_obj_align_to(bat_label, zmk_widget_battery_status_obj(&battery_widget), LV_ALIGN_OUT_LEFT_MID, -2, 0);

    // 2. Add a static title for the layer
    lv_obj_t *header = lv_label_create(screen);
    lv_label_set_text(header, "LAYER:");
    lv_obj_set_style_text_color(header, lv_color_white(), 0);
    lv_obj_align(header, LV_ALIGN_CENTER, 0, 0);

    // 3. Add the Layer Status widget below the title
    zmk_widget_layer_status_init(&layer_widget, screen);
    lv_obj_set_style_text_color(zmk_widget_layer_status_obj(&layer_widget), lv_color_white(), 0);
    lv_obj_align(zmk_widget_layer_status_obj(&layer_widget), LV_ALIGN_CENTER, 0, 15);

    return screen;
}
