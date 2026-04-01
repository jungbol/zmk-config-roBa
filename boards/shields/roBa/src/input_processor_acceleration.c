#include <zephyr/device.h>
#include <zephyr/init.h>
#include <zephyr/sys/util.h>

#include <zmk/input/input_processor.h>
#include <zmk/events/input_event.h>

#include "input_processor_acceleration.h"

#define DT_DRV_COMPAT zmk_input_processor_acceleration

static int input_processor_acceleration_handle_event(
    struct zmk_input_event *event,
    const struct device *dev
) {
    const struct input_processor_acceleration_config *config = dev->config;

    if (event->type != INPUT_EV_REL) {
        return 0;
    }

    int32_t value = event->value;
    int32_t abs_value = (value < 0) ? -value : value;

    // 간단한 가속 로직
    if (config->acceleration_exponent > 10) {
        for (int i = 10; i < config->acceleration_exponent; i += 5) {
            value = (value * abs_value) / 10;
        }
    }

    value = (value * config->acceleration_coefficient) / 10;

    event->value = value;

    return 0;
}

static const struct input_processor_driver_api api = {
    .handle_event = input_processor_acceleration_handle_event,
};

#define INPUT_PROCESSOR_ACCELERATION_INIT(inst)                                \
    static const struct input_processor_acceleration_config                    \
        config_##inst = {                                                      \
            .acceleration_exponent = DT_INST_PROP(inst, acceleration_exponent),\
            .acceleration_coefficient = DT_INST_PROP(inst, acceleration_coefficient),\
    };                                                                         \
                                                                               \
    DEVICE_DT_INST_DEFINE(inst,                                                \
                          NULL,                                                \
                          NULL,                                                \
                          NULL,                                                \
                          &config_##inst,                                      \
                          APPLICATION,                                         \
                          CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,                 \
                          &api);

DT_INST_FOREACH_STATUS_OKAY(INPUT_PROCESSOR_ACCELERATION_INIT)
