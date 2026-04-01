static const struct input_processor_driver_api api = {
    .handle_event = input_processor_code_acceleration_handle_event,
};

DEVICE_DT_DEFINE(...);
