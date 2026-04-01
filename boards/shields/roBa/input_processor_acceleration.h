/*
 * Copyright (c) 2024 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <zephyr/kernel.h>
#include <zmk/events/input_event.h>

struct input_processor_code_acceleration_config {
    uint16_t acceleration_exponent;
    uint16_t acceleration_coefficient;
};

static inline int
input_processor_code_acceleration_handle_event(struct zmk_input_event *event,
                                              const struct input_processor_code_acceleration_config *config) {
    if (event->type != INPUT_EV_REL) {
        return 0;
    }

    int32_t value = event->value;
    int32_t abs_value = (value < 0) ? -value : value;

    // Apply acceleration: value = sign(value) * (abs(value) ^ (exponent / 10)) * (coefficient / 10)
    // For simplicity in C, we use a basic power function or linear scaling if exponent is 10.
    
    if (config->acceleration_exponent != 10) {
        // This is a simplified version for MCU environments
        for (int i = 10; i < config->acceleration_exponent; i += 10) {
            value = (value * abs_value) / 10;
        }
    }

    event->value = (value * config->acceleration_coefficient) / 10;

    return 0;
}
