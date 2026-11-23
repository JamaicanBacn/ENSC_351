#ifndef CONTROLS_H
#define CONTROLS_H

#include "rotaryEncoder.h"
#include "joystick.h"
#include "accellerometer.h"

typedef struct {
    Rotary_encoder rotary;
    JoyStick joystick;
    Accel_struct accel;
} Controls;

void *controls_run(void *arg);
void controls_stop(void);

#endif
