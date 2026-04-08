#include <zmk/display/status_screen.h>
#include <zmk/display/widgets/layer_status.h>
#include <lvgl.h>

static struct zmk_widget_layer_status layer_widget;

lv_obj_t *zmk_display_status_screen() {
    lv_obj_t *screen;
    screen = lv_obj_create(NULL); 

    // Static Title
    lv_obj_t *title = lv_label_create(screen);
    lv_label_set_text(title, "LAYER:");
    lv_obj_set_style_text_color(title, lv_color_white(), 0);
    lv_obj_align(title, LV_ALIGN_CENTER, 0, -10);

    // Built-in Layer Widget
    zmk_widget_layer_status_init(&layer_widget, screen);
    lv_obj_set_style_text_color(zmk_widget_layer_status_obj(&layer_widget), lv_color_white(), 0);
    lv_obj_align(zmk_widget_layer_status_obj(&layer_widget), LV_ALIGN_CENTER, 0, 10);

    return screen;
}
