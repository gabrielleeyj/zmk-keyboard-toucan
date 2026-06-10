#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
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

// Local battery fuel gauge, used to read the actual cell voltage. Only the
// central's own battery is reachable this way; the peripheral only sends its
// state-of-charge over the split BLE link, so its voltage cannot be shown.
#if DT_HAS_CHOSEN(zmk_battery)
static const struct device *const battery = DEVICE_DT_GET(DT_CHOSEN(zmk_battery));
#else
static const struct device *const battery = NULL;
#endif

// Reads the cached voltage in millivolts. ZMK fetches the gauge periodically and
// redraws this widget on the resulting battery-state-changed event, so the cached
// value is fresh at draw time. Returns false if no reading is available.
static bool read_battery_mv(int32_t *millivolts) {
    if (battery == NULL || !device_is_ready(battery)) {
        return false;
    }

    struct sensor_value voltage;
    if (sensor_channel_get(battery, SENSOR_CHAN_GAUGE_VOLTAGE, &voltage) != 0) {
        return false;
    }

    *millivolts = voltage.val1 * 1000 + voltage.val2 / 1000;
    return true;
}

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

static void draw_detail(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_label_dsc_t label_dsc;
    init_label_dsc(&label_dsc, LVGL_FOREGROUND, &quinquefive_8, LV_TEXT_ALIGN_CENTER);

    char buf[12];
    snprintf(buf, sizeof(buf), "%d%%", state->battery);
    lv_canvas_draw_text(canvas, 8, 30, 60, &label_dsc, buf);

    int32_t millivolts;
    if (read_battery_mv(&millivolts)) {
        snprintf(buf, sizeof(buf), "%d.%02dV", millivolts / 1000, (millivolts % 1000) / 10);
        lv_canvas_draw_text(canvas, 8, 42, 60, &label_dsc, buf);
    }
}

void draw_battery_status(lv_obj_t *canvas, const struct status_state *state) {
    draw_level(canvas, state);
    draw_detail(canvas, state);
}
