#include <pebble.h>
#include "data_framework.h"
#include "javascript_handler.h"
#include "ACTV_window.h"
#include "main.h"
	
#define SEND_DATA_KEY 100
#define MESSAGE_TYPE 99
#define ACTIVITY_1_N 10
#define ACTIVITY_1_C 11
#define ACTIVITY_1_G 12
	
#define ACTIVITY_2_N 20
#define ACTIVITY_2_C 21
#define ACTIVITY_2_G 22
	
#define ACTIVITY_3_N 30
#define ACTIVITY_3_C 31
#define ACTIVITY_3_G 32
	
Activity *temp;
int number_to_cpy = 0;
bool malloced = false;

void send_message_to_js(){
	APP_LOG(APP_LOG_LEVEL_INFO, "called!");
	DictionaryIterator *iter;
	AppMessageResult result = app_message_outbox_begin(&iter);
	
	dict_write_uint8(iter, MESSAGE_TYPE, SEND_DATA_KEY);
	Activity temp = get_activity(last_used_ACTVs(1));
	dict_write_cstring(iter, ACTIVITY_1_N, temp.name);
	dict_write_uint32(iter, ACTIVITY_1_C, temp.used);
	dict_write_uint32(iter, ACTIVITY_1_G, temp.goal);
	
	//APP_LOG(APP_LOG_LEVEL_INFO, "%s", temp.name);
	
	Activity temp_1 = get_activity(last_used_ACTVs(2));
	dict_write_cstring(iter, ACTIVITY_2_N, temp_1.name);
	dict_write_uint32(iter, ACTIVITY_2_C, temp_1.used);
	dict_write_uint32(iter, ACTIVITY_2_G, temp_1.goal);
	
	Activity temp_2 = get_activity(last_used_ACTVs(3));
	dict_write_cstring(iter, ACTIVITY_3_N, temp_2.name);
	dict_write_uint32(iter, ACTIVITY_3_C, temp_2.used);
	dict_write_uint32(iter, ACTIVITY_3_G, temp_2.goal);
	app_message_outbox_send();
	APP_LOG(APP_LOG_LEVEL_INFO, "sent!");
}

void copy_ACTV_data(){
	activity_copy(number_to_cpy, temp);
	refresh_menu();
	malloced = false;
}

void process_tuple(Tuple *t)
{
	int key = t->key;
	int value = t->value->int32;
	static char string_value[32];
	switch(key){
		case 0:
			number_to_cpy = (uint8_t)value-1;
			break;
		case 1:
		APP_LOG(APP_LOG_LEVEL_INFO, "%d vl", value);
			temp->data_type = value;
			break;
		case 2:
			strcpy(string_value, t->value->cstring);
			strcpy(temp->name, string_value);
			break;
		case 3:
			strcpy(string_value, t->value->cstring);
			strcpy(temp->other, string_value);
			break;
		case 4:
			temp->goal = value;
			break;
		case 6:
			temp->enabled = (bool)value;
			break;
	}
}

void rec_handler_jsfw(DictionaryIterator *iter, void *context){
	if(malloced){
		return;
	}
	temp = malloc(sizeof(Activity));
	malloced = true;
	Tuple *t = dict_read_first(iter);
	if(t)
	{
		process_tuple(t);
	}
	while(t != NULL)
	{
		t = dict_read_next(iter);
		if(t)
		{
			process_tuple(t);
		}
	}
	copy_ACTV_data();
	free(temp);
}