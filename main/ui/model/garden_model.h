#ifndef GARDEN_MODEL_H
#define GARDEN_MODEL_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    bool corridor_light;
    bool strip_light;
    bool veg_light;
    uint8_t veg_pwm_duty;
    bool fan;
    bool servo_hang;
    bool auto_mode;
    uint8_t soil;
    uint8_t light;
    bool rain_wet;
    uint8_t temperature;
    uint8_t humidity;
} garden_model_t;

void garden_model_init(void);
void garden_model_start(void);
void garden_model_stop(void);
const garden_model_t *garden_model_get(void);

void garden_model_set_light(const char *device, bool on);
void garden_model_set_fan(bool on);
void garden_model_set_servo(bool hang);
void garden_model_set_auto(bool on);

#endif
