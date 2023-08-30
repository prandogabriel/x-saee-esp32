
#ifndef LIGHT_H_
#define LIGHT_H_

#include "driver/gpio.h"

#define BLINK_GPIO GPIO_NUM_2

// static uint8_t s_led_state = 0;
// 

void Configure_light(void);

void Toggle_light(void);

#endif /* ifndef LIGHT_H_ */
