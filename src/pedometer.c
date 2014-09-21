#include <pebble.h>
#include <math.h>
#include "pedometer.h"

AppTimer *timer;
const int ACCEL_STEP_MS = 475;
// value to auto adjust step acceptance 
const int PED_ADJUST = 2;
// steps required per calorie
const int STEPS_PER_CALORIE = 22;
// value by which step goal is incremented
const int STEP_INCREMENT = 50;
// values for max/min number of calibration options 
const int MAX_CALIBRATION_SETTINGS = 3;
const int MIN_CALIBRATION_SETTINGS = 1;

int X_DELTA = 35;
int Y_DELTA, Z_DELTA = 185;
int YZ_DELTA_MIN = 175;
int YZ_DELTA_MAX = 195; 
int X_DELTA_TEMP, Y_DELTA_TEMP, Z_DELTA_TEMP = 0;
int lastX, lastY, lastZ, currX, currY, currZ = 0;
int sensitivity = 1;

long stepGoal = 0;
uint8_t pedometerCount = 0;
long caloriesBurned = 0;
long tempTotal = 0;

bool did_pebble_vibrate = false;
bool validX, validY, validZ = false;
bool SID;
bool isDark;
bool startedSession = false;

void resetUpdate() {
	lastX = currX;
	lastY = currY;
	lastZ = currZ;
	validX = false;
	validY = false;
	validZ = false;
}

void ui_update(){
	if ((validX && validY) || (validX && validZ)) {
		pedometerCount++;
	}
	resetUpdate();
}

void pedometer_override(bool enabled){
	startedSession = enabled;
}

void register_pedometer(){
	accel_data_service_subscribe(0, NULL);
	timer = app_timer_register(475, timer_callback, NULL);
}

void deregister_pedometer(){
	accel_data_service_unsubscribe();
	app_timer_cancel(timer);
}

void reset_steps(){
	pedometerCount = 0;
}

uint8_t get_steps(){
	return pedometerCount;
}

void autoCorrectZ(){
	if (Z_DELTA > YZ_DELTA_MAX){
		Z_DELTA = YZ_DELTA_MAX; 
	} else if (Z_DELTA < YZ_DELTA_MIN){
		Z_DELTA = YZ_DELTA_MIN;
	}
}

void autoCorrectY(){
	if (Y_DELTA > YZ_DELTA_MAX){
		Y_DELTA = YZ_DELTA_MAX; 
	} else if (Y_DELTA < YZ_DELTA_MIN){
		Y_DELTA = YZ_DELTA_MIN;
	}
}

void pedometer_update() {
	if (startedSession) {
		//APP_LOG(APP_LOG_LEVEL_DEBUG, "yes");
		X_DELTA_TEMP = abs(abs(currX) - abs(lastX));
		if (X_DELTA_TEMP >= X_DELTA) {
			validX = true;
		}
		Y_DELTA_TEMP = abs(abs(currY) - abs(lastY));
		if (Y_DELTA_TEMP >= Y_DELTA) {
			validY = true;
			if (Y_DELTA_TEMP - Y_DELTA > 200){
				autoCorrectY();
				Y_DELTA = (Y_DELTA < YZ_DELTA_MAX) ? Y_DELTA + PED_ADJUST : Y_DELTA;
			} else if (Y_DELTA - Y_DELTA_TEMP > 175){
				autoCorrectY();
				Y_DELTA = (Y_DELTA > YZ_DELTA_MIN) ? Y_DELTA - PED_ADJUST : Y_DELTA;
			}
		}
		Z_DELTA_TEMP = abs(abs(currZ) - abs(lastZ));
		if (abs(abs(currZ) - abs(lastZ)) >= Z_DELTA) {
			validZ = true;
			if (Z_DELTA_TEMP - Z_DELTA > 200){
				autoCorrectZ();
				Z_DELTA = (Z_DELTA < YZ_DELTA_MAX) ? Z_DELTA + PED_ADJUST : Z_DELTA;
			} else if (Z_DELTA - Z_DELTA_TEMP > 175){
				autoCorrectZ();
				Z_DELTA = (Z_DELTA < YZ_DELTA_MAX) ? Z_DELTA + PED_ADJUST : Z_DELTA;
			}
		}
	}
}

void timer_callback(void *data) {
	AccelData accel = (AccelData) { .x = 0, .y = 0, .z = 0 };
	accel_service_peek(&accel);

	if (!startedSession) {
		lastX = accel.x;
		lastY = accel.y;
		lastZ = accel.z;
	} else {
		currX = accel.x;
		currY = accel.y;
		currZ = accel.z;
	}
	pedometer_update();
	ui_update();
	timer = app_timer_register(ACCEL_STEP_MS, timer_callback, NULL);
}