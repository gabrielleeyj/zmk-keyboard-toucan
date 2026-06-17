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

void draw_battery_peripheral_status(lv_obj_t *canvas, const struct status_state *state) {
    draw_level_peripheral(canvas, state);
    draw_percent_peripheral(canvas, state);
}
