#pragma once
void pedometer_override(bool enabled);
void register_pedometer();
void deregister_pedometer();
void reset_steps();
uint8_t get_steps();
void timer_callback(void *data);