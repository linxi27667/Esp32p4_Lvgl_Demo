#include "garden_model.h"
#include "../core/ui_events.h"
#include "lvgl.h"
#include <stdlib.h>
#include <string.h>

static garden_model_t s_model;
static lv_timer_t *s_timer;

static void sim_tick(lv_timer_t *timer)
{
    (void)timer;
    s_model.soil = 30 + (rand() % 20);
    s_model.light = 40 + (rand() % 30);
    s_model.temperature = 22 + (rand() % 8);
    s_model.humidity = 50 + (rand() % 20);

    if (s_model.auto_mode) {
        if (s_model.veg_light) {
            if (s_model.light < 20) s_model.veg_pwm_duty = 100;
            else if (s_model.light > 70) s_model.veg_pwm_duty = 10;
            else s_model.veg_pwm_duty = (uint8_t)(100 - s_model.light);
        }
        if (s_model.rain_wet && s_model.servo_hang) {
            s_model.servo_hang = false;
        }
    }

    ui_event_publish(UI_EVENT_MODEL_UPDATED);
}

void garden_model_init(void)
{
    memset(&s_model, 0, sizeof(s_model));
    s_model.auto_mode = true;
    s_model.servo_hang = true;
    s_model.soil = 45;
    s_model.light = 55;
    s_model.temperature = 25;
    s_model.humidity = 60;
}

void garden_model_start(void)
{
    if (!s_timer) {
        s_timer = lv_timer_create(sim_tick, 2000, NULL);
    }
}

void garden_model_stop(void)
{
    if (s_timer) {
        lv_timer_delete(s_timer);
        s_timer = NULL;
    }
}

const garden_model_t *garden_model_get(void)
{
    return &s_model;
}

void garden_model_set_light(const char *device, bool on)
{
    if (!device) return;
    if (strcmp(device, "corridor_light") == 0) s_model.corridor_light = on;
    else if (strcmp(device, "strip_light") == 0) s_model.strip_light = on;
    else if (strcmp(device, "veg_light") == 0) {
        s_model.veg_light = on;
        s_model.veg_pwm_duty = on ? 80 : 0;
    }
    ui_event_publish(UI_EVENT_MODEL_UPDATED);
}

void garden_model_set_fan(bool on)
{
    s_model.fan = on;
    ui_event_publish(UI_EVENT_MODEL_UPDATED);
}

void garden_model_set_servo(bool hang)
{
    s_model.servo_hang = hang;
    ui_event_publish(UI_EVENT_MODEL_UPDATED);
}

void garden_model_set_auto(bool on)
{
    s_model.auto_mode = on;
    ui_event_publish(UI_EVENT_MODEL_UPDATED);
}
