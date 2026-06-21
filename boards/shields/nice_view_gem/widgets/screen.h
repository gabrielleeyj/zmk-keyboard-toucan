#pragma once

#include <lvgl.h>
#include <zephyr/kernel.h>
#include "util.h"

struct zmk_widget_screen {
    sys_snode_t node;
    lv_obj_t *obj;
    lv_color_t cbuf[SCREEN_WIDTH * SCREEN_HEIGHT];
    // cbuf2/cbuf3 are unused: only cbuf is bound to the canvas, and draw_top
    // ignores any passed buffer. Kept (commented) to reclaim ~48 KB static RAM
    // while preserving the original layout for reference.
    // lv_color_t cbuf2[SCREEN_WIDTH * SCREEN_HEIGHT];
    // lv_color_t cbuf3[SCREEN_WIDTH * SCREEN_HEIGHT];
    struct status_state state;
};

int zmk_widget_screen_init(struct zmk_widget_screen *widget, lv_obj_t *parent);
lv_obj_t *zmk_widget_screen_obj(struct zmk_widget_screen *widget);
