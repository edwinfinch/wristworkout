#include <pebble.h>
#include "data_framework.h"
	
ActivityData main_ACTV;
ActivityDataPersist ps_one, ps_two, ps_three;

int used_stack[3] = {
	0, 1, 2
};

void used_stack_push(int activity){
	used_stack[2] = used_stack[1];
	used_stack[1] = used_stack[0];
	used_stack[0] = activity;
}

int last_used_ACTVs(int stack_num){
	return used_stack[stack_num-1];
}

void persist_load(){
	int value = persist_read_data(0, &ps_one, sizeof(ps_one));
	int value1 = persist_read_data(1, &ps_two, sizeof(ps_two));
	int value2 = persist_read_data(2, &ps_three, sizeof(ps_three));
	
	//yes this is inefficient shut up
	main_ACTV.activities[0] = ps_one.activities[0];
	main_ACTV.activities[1] = ps_one.activities[1];
	main_ACTV.activities[2] = ps_one.activities[2];
	main_ACTV.activities[3] = ps_two.activities[0];
	main_ACTV.activities[4] = ps_two.activities[1];
	main_ACTV.activities[5] = ps_two.activities[2];
	main_ACTV.activities[6] = ps_three.activities[0];
	main_ACTV.activities[7] = ps_three.activities[1];
	main_ACTV.activities[8] = ps_three.activities[2];
	
	int valuet = value+value1+value2;
	
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Read %d bytes", valuet);
	
	//APP_LOG(APP_LOG_LEVEL_INFO, "0: %d, 1: %d, 2: %d", main_ACTV.activities[0], main_ACTV.activities[], main_ACTV.activities[0])
}

void persist_save(){
	//don't say it
	//(for loop)
	ps_one.activities[0] = main_ACTV.activities[0];
	ps_one.activities[1] = main_ACTV.activities[1];
	ps_one.activities[2] = main_ACTV.activities[2];
	ps_two.activities[0] = main_ACTV.activities[3];
	ps_two.activities[1] = main_ACTV.activities[4];
	ps_two.activities[2] = main_ACTV.activities[5];
	ps_three.activities[0] = main_ACTV.activities[6];
	ps_three.activities[1] = main_ACTV.activities[7];
	ps_three.activities[2] = main_ACTV.activities[8];
	
	int value = persist_write_data(0, &ps_one, sizeof(ps_one));
	int value1 = persist_write_data(1, &ps_two, sizeof(ps_two));
	int value2 = persist_write_data(2, &ps_three, sizeof(ps_three));
	
	int valuet = value+value1+value2;
	
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Wrote %d bytes", valuet);
}

void activity_copy_alt(int number, Activity ACTV){
	main_ACTV.activities[number] = ACTV;
}

void activity_copy(int number, Activity *ACTV){
	strncpy(main_ACTV.activities[number].name, ACTV->name, 10);
	strncpy(main_ACTV.activities[number].other, ACTV->other, 20);
	main_ACTV.activities[number].enabled = ACTV->enabled;
	main_ACTV.activities[number].data_type = ACTV->data_type;
	main_ACTV.activities[number].goal = ACTV->goal;
	APP_LOG(APP_LOG_LEVEL_INFO, "type: %d", main_ACTV.activities[number].data_type);
}

void set_enabled(int activity, bool exists){
	main_ACTV.activities[activity].enabled = exists;
}

uint8_t get_amountof_activities(){
	uint8_t amount = 0;
	int i = 0;
	while(i < 8){
		if(main_ACTV.activities[i].enabled){
			amount++;
		}
		i++;
	}
	return amount;
}

Activity get_activity(int activity){
	return main_ACTV.activities[activity];
}