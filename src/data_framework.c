#include <pebble.h>
#include "data_framework.h"
	
ActivityData main_ACTV;
ActivityDataPersist ps_one, ps_two, ps_three;

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

void activity_copy(int number, Activity ACTV){
	main_ACTV.activities[number] = ACTV;
	APP_LOG(APP_LOG_LEVEL_DEBUG, "ACTV num: %d has been copied.", main_ACTV.activities[number].number);
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