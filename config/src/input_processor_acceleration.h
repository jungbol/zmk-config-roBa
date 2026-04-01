#pragma once

#include <zephyr/kernel.h>
#include <zmk/events/input_event.h>

struct input_processor_acceleration_config {
    uint16_t acceleration_exponent;
    uint16_t acceleration_coefficient;
};

int input_processor_acceleration_handle_event(
    struct zmk_input_event *event,
    const struct input_processor_acceleration_config *config
);
