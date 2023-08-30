#include <stdlib.h>
#include <stdint.h>

#include "light.h"

static uint8_t s_led_state = 0;

void Configure_light(void)
{
    gpio_reset_pin(BLINK_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
}

void Toggle_light(void)
{
    /* Set the GPIO level according to the state (LOW or HIGH)*/
        s_led_state = !s_led_state;

    gpio_set_level(BLINK_GPIO, s_led_state);
}
