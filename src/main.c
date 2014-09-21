#include <pebble.h>
#include "data_framework.h"
#include "javascript_handler.h"
#include "ACTV_window.h"
#include "animations.h"
#include "pedometer.h"
#include "stop_window.h"
#include "main.h"
	
#define SECTION_NUM 1
#define ITEMS_NUM 9
	
Window *activity_window;
SimpleMenuSection ACTV_section[SECTION_NUM];
SimpleMenuItem ACTV_menu_items[ITEMS_NUM];
SimpleMenuLayer *activity_menu;

void refresh_menu(){
	for(int i = 0; i < 9; i++){
		Activity temp_r = get_activity(i);
		if(temp_r.enabled){
			static char t_buffer[11];
			snprintf(t_buffer, sizeof(t_buffer), "%s", temp_r.name);
			ACTV_menu_items[i].title = t_buffer;
			static char buffer[] = "Goal of 9000001";
			snprintf(buffer, sizeof(buffer), "Goal of %d", temp_r.goal);
			ACTV_menu_items[i].subtitle = buffer;
		}
	}
}

void activity_callback(int index, void *ctx){
	launch_activity(index);
}

void window_load_activity(Window *w){
	Layer *window_layer = window_get_root_layer(w);
	int i = 0;
	
	/*
	Activity temp_y;
	temp_y.enabled = true;
	strncpy(temp_y.name, "Your mom", 10);
	temp_y.goal = 666;
	activity_copy(2, temp_y);
	*/
	
	while(i < ITEMS_NUM){
		ACTV_menu_items[i] = (SimpleMenuItem){
			.title = "Activity",
			.subtitle = "No data.",
			.callback = activity_callback,
		};
		i++;
	}
    ACTV_section[0] = (SimpleMenuSection){
		.title = "Activities",
		.num_items = ITEMS_NUM,
		.items = ACTV_menu_items,
    };
	activity_menu = simple_menu_layer_create(GRect(0, 0, 144, 0), activity_window, ACTV_section, SECTION_NUM, NULL);
	layer_add_child(window_layer, simple_menu_layer_get_layer(activity_menu));
	
	refresh_menu();
	
	animate_layer(simple_menu_layer_get_layer(activity_menu), &GRect(0, 400, 144, 154), &GRect(0, 0, 144, 154), 700, 0);
}

void window_unload_activity(Window *w){
	simple_menu_layer_destroy(activity_menu);
}

void init(){
	activity_window = window_create();
	window_set_window_handlers(activity_window, (WindowHandlers){
		.load = window_load_activity,
		.unload = window_unload_activity,
	});
	persist_load();	
	stop_window_init();
	ACTV_window_init();
	app_message_register_inbox_received(rec_handler_jsfw);
	tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
	app_message_open(512, 512);
	window_stack_push(activity_window, true);
}

void deinit(){
	stop_window_deinit();
	ACTV_window_deinit();
	persist_save();
	window_destroy(activity_window);
}

int main(){
	init();
	app_event_loop();
	deinit();
	return 0;
}