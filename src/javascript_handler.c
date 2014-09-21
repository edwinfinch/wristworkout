#include <pebble.h>
#include "data_framework.h"
#include "javascript_handler.h"
#include "main.h"
	
Activity temp;
AppTimer *copy_timer;

void copy_ACTV_data(){
	activity_copy(temp.number, temp);
	refresh_menu();
}

void process_tuple(Tuple *t)
{
	int key = t->key;
	int value = t->value->int32;
	char string_value[32];
	strcpy(string_value, t->value->cstring);
	switch(key){
		case 0:
			temp.number = (uint8_t)value-1;
			break;
		case 1:
			temp.data_type = (uint8_t)value;
			break;
		case 2:
			strcpy(temp.name, string_value);
			break;
		case 3:
			strcpy(temp.other, string_value);
			break;
		case 4:
			temp.goal = value;
			break;
		case 5:
			temp.length = value;
			break;
		case 6:
			temp.enabled = (bool)value;
			break;
		case 7:
			temp.accel_service = value;
			break;
	}
}

void rec_handler_jsfw(DictionaryIterator *iter, void *context){
	copy_timer = app_timer_register(2000, copy_ACTV_data, NULL);
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
}