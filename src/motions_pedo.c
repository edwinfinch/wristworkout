#include <pebble.h>
int motions_count = 0;

void reset_motions(){
	motions_count = 0;
}

int get_motions(){
	return motions_count;
}

bool isLow(int16_t i){
	if(i < 0){
		i = i*(-1);
	}
	if(i > 400 && i < 900){
		return true;
	}
	return false;
}

bool isHigh(int16_t i){
	if(i < 0){
		i = i*(-1);
	}
	if(i > 1000){
		return true;
	}
	return false;
}

bool find_high(int16_t data[20]){
	int16_t i;
	for(i = 0; i != 19; i++){
		if(isHigh(data[i])){
			return true;
		}
	}
	return false;
}

bool find_low(int16_t data[20]){
	int16_t i;
	for(i = 0; i != 19; i++){
		if(isLow(data[i])){
			return true;
		}
	}
	return false;
}

void accel_handler(AccelData *data, uint32_t samples){
	int16_t x_data[20];
	int i;
	for(i = 0; i < (int)samples; i++){
		x_data[i] = data[i].x;
		//APP_LOG(APP_LOG_LEVEL_DEBUG, "X: %d, i: %d", x_data[i], i);
	}
	if(find_high(x_data) && find_low(x_data)){
		motions_count++;
	}
}

void start_motions_pedometer_service() {
	accel_data_service_subscribe(20, accel_handler);
}

void end_motions_pedometer_service(){
	accel_data_service_unsubscribe();
}