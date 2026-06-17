#include <zephyr/kernel.h>
#include <stdio.h>
#include "battery.h"
#include "../assets/custom_fonts.h"

LV_IMG_DECLARE(bolt);
LV_IMG_DECLARE(l_battery_100);
LV_IMG_DECLARE(l_battery_90);
LV_IMG_DECLARE(l_battery_75);
LV_IMG_DECLARE(l_battery_50);
LV_IMG_DECLARE(l_battery_25);
LV_IMG_DECLARE(l_battery_10);


static void draw_level(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_img_dsc_t img_dsc_l;
    lv_draw_img_dsc_init(&img_dsc_l);

    uint8_t level = state->battery;
    if (level > 90) {
        lv_canvas_draw_img(canvas, 8, 10, &l_battery_100, &img_dsc_l);
    } else if (level > 75) {
        lv_canvas_draw_img(canvas, 8, 10, &l_battery_90, &img_dsc_l);
    } else if (level > 50) {
        lv_canvas_draw_img(canvas, 8, 10, &l_battery_75, &img_dsc_l);
    } else if (level > 25) {
        lv_canvas_draw_img(canvas, 8, 10, &l_battery_50, &img_dsc_l);
    } else if (level > 10) {
        lv_canvas_draw_img(canvas, 8, 10, &l_battery_25, &img_dsc_l);
    } else if (level > 1) {
        lv_canvas_draw_img(canvas, 8, 10, &l_battery_10, &img_dsc_l);
    }

}

static void draw_percent(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_label_dsc_t label_dsc;
    init_label_dsc(&label_dsc, LVGL_FOREGROUND, &quinquefive_8, LV_TEXT_ALIGN_CENTER);

    char buf[8];
    snprintf(buf, sizeof(buf), "%d%%", state->battery);
    lv_canvas_draw_text(canvas, 8, 30, 60, &label_dsc, buf);
}

// Small lightning bolt drawn to the left of the percentage when USB power is
// present. ZMK only exposes "USB powered", so this means plugged in / charging
// and cannot distinguish a full battery from one still charging.
static void draw_charging(lv_obj_t *canvas, const struct status_state *state) {
    if (!state->charging) {
        return;
    }

    lv_draw_rect_dsc_t bolt_dsc;
    init_rect_dsc(&bolt_dsc, LVGL_FOREGROUND);

    static const lv_point_t bolt[] = {
        {17, 29}, {14, 35}, {16, 35}, {15, 40}, {19, 33}, {17, 33},
    };
    lv_canvas_draw_polygon(canvas, bolt, 6, &bolt_dsc);
}

void draw_battery_status(lv_obj_t *canvas, const struct status_state *state) {
    draw_level(canvas, state);
    draw_percent(canvas, state);
    draw_charging(canvas, state);
}