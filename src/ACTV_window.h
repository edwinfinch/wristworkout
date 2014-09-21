#pragma once
void tick_handler(struct tm *t, TimeUnits units);
void ACTV_window_init();
void ACTV_window_deinit();
void launch_activity(int activity);
void end_all_threads();