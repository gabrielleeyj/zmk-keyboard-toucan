#include <zephyr/kernel.h>
#include <stdio.h>
#include "battery_peripheral.h"
#include "../assets/custom_fonts.h"

LV_IMG_DECLARE(bolt);
LV_IMG_DECLARE(r_battery_100);
LV_IMG_DECLARE(r_battery_90);
LV_IMG_DECLARE(r_battery_75);
LV_IMG_DECLARE(r_battery_50);
LV_IMG_DECLARE(r_battery_25);
LV_IMG_DECLARE(r_battery_10);


static void draw_level_peripheral(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_img_dsc_t img_dsc_r;
    lv_draw_img_dsc_init(&img_dsc_r);

    uint8_t level = state->battery_p;
    if (level > 90) {
        lv_canvas_draw_img(canvas, 80, 10, &r_battery_100, &img_dsc_r);
    } else if (level > 75) {
        lv_canvas_draw_img(canvas, 80, 10, &r_battery_90, &img_dsc_r);
    } else if (level > 50) {
        lv_canvas_draw_img(canvas, 80, 10, &r_battery_75, &img_dsc_r);
    } else if (level > 25) {
        lv_canvas_draw_img(canvas, 80, 10, &r_battery_50, &img_dsc_r);
    } else if (level > 10) {
        lv_canvas_draw_img(canvas, 80, 10, &r_battery_25, &img_dsc_r);
    } else if (level > 1) {
        lv_canvas_draw_img(canvas, 80, 10, &r_battery_10, &img_dsc_r);
    }
}

static void draw_percent_peripheral(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_label_dsc_t label_dsc;
    init_label_dsc(&label_dsc, LVGL_FOREGROUND, &quinquefive_8, LV_TEXT_ALIGN_CENTER);

    char buf[8];
    snprintf(buf, sizeof(buf), "%d%%", state->battery_p);
    lv_canvas_draw_text(canvas, 80, 30, 60, &label_dsc, buf);
}

// Lightning bolt drawn to the left of the peripheral percentage when charging is
// inferred from a rising battery trend (see screen.c). Mirrors the central bolt,
// shifted right to sit by the peripheral bar.
static void draw_charging_peripheral(lv_obj_t *canvas, const struct status_state *state) {
    if (!state->charging_p) {
        return;
    }

    lv_draw_rect_dsc_t bolt_dsc;
    init_rect_dsc(&bolt_dsc, LVGL_FOREGROUND);

    static const lv_point_t bolt[] = {
        {89, 29}, {86, 35}, {88, 35}, {87, 40}, {91, 33}, {89, 33},
    };
    lv_canvas_draw_polygon(canvas, bolt, 6, &bolt_dsc);
}

void draw_battery_peripheral_status(lv_obj_t *canvas, const struct status_state *state) {
    draw_level_peripheral(canvas, state);
    draw_percent_peripheral(canvas, state);
    draw_charging_peripheral(canvas, state);
}
