/*
 * Copyright (c) 2024 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/kernel.h>
#include <zephyr/bluetooth/services/bas.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#include <zmk/battery.h>
#include <zmk/ble.h>
#include <zmk/display.h>
#include <zmk/events/battery_state_changed.h>
#include <zmk/events/usb_conn_state_changed.h>
#include <zmk/event_manager.h>
#include <zmk/usb.h>

#include "battery_status.h"

#if IS_ENABLED(CONFIG_ZMK_DONGLE_DISPLAY_DONGLE_BATTERY)
    #define SOURCE_OFFSET 1
#else
    #define SOURCE_OFFSET 0
#endif

// Sofle has 2 peripherals
#ifndef ZMK_SPLIT_BLE_PERIPHERAL_COUNT
#  define ZMK_SPLIT_BLE_PERIPHERAL_COUNT 2
#endif

#define MAX_BATTERY_SOURCES (ZMK_SPLIT_BLE_PERIPHERAL_COUNT + SOURCE_OFFSET)

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);

struct battery_state {
    uint8_t source;
    uint8_t level;
    bool usb_present;
};

struct battery_object {
    lv_obj_t *symbol;
    lv_obj_t *label;
} battery_objects[MAX_BATTERY_SOURCES];
    
static lv_color_t battery_image_buffer[MAX_BATTERY_SOURCES][40];

static void draw_battery(lv_obj_t *canvas, uint8_t level, bool usb_present) {
    lv_canvas_fill_bg(canvas, lv_color_black(), LV_OPA_COVER);
    
    // Top "nib" of the battery
    lv_canvas_set_px(canvas, 0, 0, lv_color_white());
    lv_canvas_set_px(canvas, 4, 0, lv_color_white());

    int height = 0;
    if (level > 90 && !usb_present) height = 0;
    else if (level > 70) height = 1;
    else if (level > 50) height = 2;
    else if (level > 30) height = 3;
    else if (level > 10) height = 4;
    else height = 5;

    for(int x = 1; x <= 3; x++) {
        for(int y = 2 + height; y <= 6; y++) {
            lv_canvas_set_px(canvas, x, y, lv_color_white());
        }
    }
}

static void set_battery_symbol(lv_obj_t *widget, struct battery_state state) {
    if (state.source >= MAX_BATTERY_SOURCES) {
        return;
    }
    
    lv_obj_t *symbol = battery_objects[state.source].symbol;
    lv_obj_t *label = battery_objects[state.source].label;

    draw_battery(symbol, state.level, state.usb_present);
    
    char buf[10];
    snprintf(buf, sizeof(buf), "%u%%", state.level);
    lv_label_set_text(label, buf);
    
    lv_obj_clear_flag(symbol, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(label, LV_OBJ_FLAG_HIDDEN);
}

// Renamed to avoid collision with built-in ZMK widget
void dongle_battery_status_update_cb(struct battery_state state) {
    struct zmk_widget_dongle_battery_status *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) { set_battery_symbol(widget->obj, state); }
}

static struct battery_state peripheral_battery_status_get_state(const zmk_event_t *eh) {
    const struct zmk_peripheral_battery_state_changed *ev = as_zmk_peripheral_battery_state_changed(eh);
    return (struct battery_state){
        .source = ev->source + SOURCE_OFFSET,
        .level = ev->state_of_charge,
    };
}

static struct battery_state central_battery_status_get_state(const zmk_event_t *eh) {
    const struct zmk_battery_state_changed *ev = as_zmk_battery_state_changed(eh);
    return (struct battery_state) {
        .source = 0,
        .level = (ev != NULL) ? ev->state_of_charge : zmk_battery_state_of_charge(),
        .usb_present = zmk_usb_is_powered(),
    };
}

// Renamed to avoid collision
static struct battery_state dongle_battery_status_get_state(const zmk_event_t *eh) { 
    if (as_zmk_peripheral_battery_state_changed(eh) != NULL) {
        return peripheral_battery_status_get_state(eh);
    } else {
        return central_battery_status_get_state(eh);
    }
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_dongle_battery_status, struct battery_state,
                            dongle_battery_status_update_cb, dongle_battery_status_get_state)

ZMK_SUBSCRIPTION(widget_dongle_battery_status, zmk_peripheral_battery_state_changed);

#if IS_ENABLED(CONFIG_ZMK_DONGLE_DISPLAY_DONGLE_BATTERY)
ZMK_SUBSCRIPTION(widget_dongle_battery_status, zmk_battery_state_changed);
ZMK_SUBSCRIPTION(widget_dongle_battery_status, zmk_usb_conn_state_changed);
#endif

int zmk_widget_dongle_battery_status_init(struct zmk_widget_dongle_battery_status *widget, lv_obj_t *parent) {
    widget->obj = lv_obj_create(parent);
    lv_obj_set_size(widget->obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_style_bg_opa(widget->obj, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_opa(widget->obj, LV_OPA_TRANSP, 0);

    for (int i = 0; i < MAX_BATTERY_SOURCES; i++) {
        lv_obj_t *image_canvas = lv_canvas_create(widget->obj);
        lv_obj_t *battery_label = lv_label_create(widget->obj);

        lv_canvas_set_buffer(image_canvas, battery_image_buffer[i], 5, 8, LV_IMG_CF_TRUE_COLOR);

        lv_obj_align(image_canvas, LV_ALIGN_TOP_RIGHT, 0, i * 12);
        lv_obj_set_style_text_color(battery_label, lv_color_white(), 0);
        lv_obj_set_style_text_font(battery_label, &lv_font_montserrat_12, 0);
        lv_obj_align_to(battery_label, image_canvas, LV_ALIGN_OUT_LEFT_MID, -2, 0);

        lv_obj_add_flag(image_canvas, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(battery_label, LV_OBJ_FLAG_HIDDEN);
        
        battery_objects[i] = (struct battery_object){
            .symbol = image_canvas,
            .label = battery_label,
        };
    }

    sys_slist_append(&widgets, &widget->node);
    widget_dongle_battery_status_init();
    return 0;
}

lv_obj_t *zmk_widget_dongle_battery_status_obj(struct zmk_widget_dongle_battery_status *widget) {
    return widget->obj;
}
