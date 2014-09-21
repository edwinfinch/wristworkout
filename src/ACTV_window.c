#include <pebble.h>
#include "data_framework.h"
#include "animations.h"
#include "pedometer.h"
#include "stop_window.h"
#include "motions_pedo.h"
	
typedef struct TimeData {
	int min;
	int sec;
	int hour;
}TimeData;

TimeData Timedata;
	
Window *ACTV_window;
Activity temp_ACTV;
ActionBarLayer *ACTV_ab;
Layer *ACTV_bgelmnts;
TextLayer *ACTV_title, *ACTV_data_t_1, *ACTV_data_t_2, *ACTV_data_1, *ACTV_data_2;
TextLayer *ACTV_other_t, *ACTV_other, *ACTV_time;
GBitmap *previous_i, *next_i, *pause_i, *play_i, *stop_i, *restart_i;
InverterLayer *ACTV_selector;
bool isactive = false;

char *data_types[3] = {
	"Steps", "Weight", "Time"
};
char *weight_data_units[2] = {
	"kg", "lbs",
};

void reset_timedata(){
	Timedata.sec = 0;
	Timedata.min = 0;
	Timedata.hour = 0;
}

void tick_handler(struct tm *t, TimeUnits units){
	if(isactive){
		Timedata.sec++;
		if(Timedata.sec > 59){
			Timedata.sec = 0;
			Timedata.min++;
		}
		if(Timedata.min > 59){
			Timedata.sec = 0;
			Timedata.min = 0;
			Timedata.hour++;
		}
		static char buffer[] = "00:00";
		if(Timedata.hour < 1){
			if(Timedata.sec < 10){
				snprintf(buffer, sizeof(buffer), "%d:0%d", Timedata.min, Timedata.sec);
			}
			else{				
				snprintf(buffer, sizeof(buffer), "%d:%d", Timedata.min, Timedata.sec);
			}
		}
		else{
			if(Timedata.min < 10){
				snprintf(buffer, sizeof(buffer), "%d:0%d", Timedata.hour, Timedata.min);
			}
			else{
				snprintf(buffer, sizeof(buffer), "%d:%d", Timedata.hour, Timedata.min);
			}
		}
		text_layer_set_text(ACTV_time, buffer);
	}
	/*
	if(temp_ACTV.type == 0){
		static char steps_buffer[] = "1337...";
		get_steps();
	}*/
	if(isactive){
		static char steps_buffer[] = "1337...";
		snprintf(steps_buffer, sizeof(steps_buffer), "%d", get_motions());
		text_layer_set_text(ACTV_data_1, steps_buffer);
		
		static char goal_buffer[] = "1337...";
		snprintf(goal_buffer, sizeof(goal_buffer), "%d", temp_ACTV.goal);
		text_layer_set_text(ACTV_data_2, goal_buffer);
		
		layer_mark_dirty(ACTV_bgelmnts);
	}
}

TextLayer* text_layer_init(GRect location, GTextAlignment alignment, int font)
{
	TextLayer *layer = text_layer_create(location);
	text_layer_set_text_color(layer, GColorBlack);
	text_layer_set_background_color(layer, GColorClear);
	text_layer_set_text_alignment(layer, alignment);
	switch(font){
		case 0:
			text_layer_set_font(layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
			break;
		case 1:
			text_layer_set_font(layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
			break;
		case 2:
			text_layer_set_font(layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
			break;
		case 3:
			text_layer_set_font(layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
			break;
		case 4:
			text_layer_set_font(layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
			break;
	}
	return layer;
}

void launch_activity(int activity){
	temp_ACTV = get_activity(activity);
	window_stack_push(ACTV_window, true);
}

void ACTV_pause(ClickRecognizerRef recognizer, void *context){
	isactive = !isactive;
	if(!isactive){
		action_bar_layer_set_icon(ACTV_ab, BUTTON_ID_SELECT, play_i);
	}
	else{
		action_bar_layer_set_icon(ACTV_ab, BUTTON_ID_SELECT, pause_i);
	}
	pedometer_override(isactive);
}

void ACTV_stop(ClickRecognizerRef recognizer, void *context){
	stop_window_launch();
}

void ACTV_restart(ClickRecognizerRef recognizer, void *context){
	reset_steps();
	reset_timedata();
	reset_motions();
	layer_mark_dirty(ACTV_bgelmnts);
}

void click_config_ACTV(void *contx){
	window_single_click_subscribe(BUTTON_ID_SELECT, ACTV_pause);
	window_single_click_subscribe(BUTTON_ID_UP, ACTV_stop);
	window_single_click_subscribe(BUTTON_ID_DOWN, ACTV_restart);
	window_single_click_subscribe(BUTTON_ID_BACK, stop_window_launch);
}

void bgelmnts_proc(Layer *l, GContext *ctx){
	graphics_fill_rect(ctx, GRect(7, 120, 110, 42), 4, GCornersAll);
	float steps = (float)get_motions();
	float fix = ((steps/temp_ACTV.goal)*144);
	//APP_LOG(APP_LOG_LEVEL_DEBUG, "fix: %d", fix);
	graphics_fill_rect(ctx, GRect(0, 0, fix, 3), 0, GCornerNone);
}

void window_load_ACTV(Window *w){
	Layer *window_layer = window_get_root_layer(w);
	
	GRect t_rect = GRect(0, -150, 144, 168);
	
	temp_ACTV.goal = 350;
	
	//register_pedometer();
	start_motions_pedometer_service();
	tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
	
	next_i = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NEXT_ICON);
	pause_i = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PAUSE_ICON);
	play_i = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PLAY_ICON);
	previous_i = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PREVIOUS_ICON);
	stop_i = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_STOP);
	restart_i = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_RESTART);
	
	ACTV_ab = action_bar_layer_create();
	action_bar_layer_set_click_config_provider(ACTV_ab, click_config_ACTV);
	action_bar_layer_set_icon(ACTV_ab, BUTTON_ID_SELECT, pause_i);
	action_bar_layer_set_icon(ACTV_ab, BUTTON_ID_DOWN, restart_i);
	action_bar_layer_set_icon(ACTV_ab, BUTTON_ID_UP, stop_i);
	action_bar_layer_add_to_window(ACTV_ab, ACTV_window);
	
	ACTV_bgelmnts = layer_create(GRect(0, 0, 144, 168));
	layer_set_update_proc(ACTV_bgelmnts, bgelmnts_proc);
	layer_add_child(window_layer, ACTV_bgelmnts);
	
	ACTV_other_t = text_layer_init(t_rect, GTextAlignmentLeft, 0);
	text_layer_set_text(ACTV_other_t, "Other");
	layer_add_child(window_layer, text_layer_get_layer(ACTV_other_t));
	
	ACTV_data_t_1 = text_layer_init(t_rect, GTextAlignmentLeft, 0);
	text_layer_set_text(ACTV_data_t_1, "Motions");
	layer_add_child(window_layer, text_layer_get_layer(ACTV_data_t_1));
	
	ACTV_data_1 = text_layer_init(t_rect, GTextAlignmentLeft, 1);
	text_layer_set_text(ACTV_data_1, "133709");
	layer_add_child(window_layer, text_layer_get_layer(ACTV_data_1));
	
	ACTV_data_t_2 = text_layer_init(t_rect, GTextAlignmentLeft, 0);
	text_layer_set_text(ACTV_data_t_2, "Goal");
	layer_add_child(window_layer, text_layer_get_layer(ACTV_data_t_2));
	
	ACTV_data_2 = text_layer_init(t_rect, GTextAlignmentLeft, 1);
	text_layer_set_text(ACTV_data_2, "900100");
	layer_add_child(window_layer, text_layer_get_layer(ACTV_data_2));
	
	ACTV_title = text_layer_init(t_rect, GTextAlignmentLeft, 2);
	text_layer_set_text(ACTV_title, "General");
	layer_add_child(window_layer, text_layer_get_layer(ACTV_title));
	
	ACTV_other = text_layer_init(t_rect, GTextAlignmentLeft, 3);
	text_layer_set_text(ACTV_other, "Keep moving!"); //17 char limit
	layer_add_child(window_layer, text_layer_get_layer(ACTV_other));
	
	ACTV_time = text_layer_init(t_rect, GTextAlignmentCenter, 4);
	text_layer_set_text(ACTV_time, "01:05");
	text_layer_set_text_color(ACTV_time, GColorWhite);
	layer_add_child(window_layer, text_layer_get_layer(ACTV_time));
	
	isactive = true;
	pedometer_override(true);
	animate_layer(text_layer_get_layer(ACTV_title), &t_rect, &GRect(7, 0, 144, 36), 600, 50);
	animate_layer(text_layer_get_layer(ACTV_data_t_1), &t_rect, &GRect(7, 35, 144, 32), 600, 100);
	animate_layer(text_layer_get_layer(ACTV_data_t_2), &t_rect, &GRect(70, 35, 144, 32), 600, 150);
	animate_layer(text_layer_get_layer(ACTV_data_1), &t_rect, &GRect(7, 50, 144, 32), 600, 200);
	animate_layer(text_layer_get_layer(ACTV_data_2), &t_rect, &GRect(70, 50, 144, 32), 600, 250);
	animate_layer(text_layer_get_layer(ACTV_other_t), &t_rect, &GRect(7, 85, 144, 32), 600, 300);
	animate_layer(text_layer_get_layer(ACTV_other), &t_rect, &GRect(7, 97, 144, 36), 600, 350);
	animate_layer(text_layer_get_layer(ACTV_time), &GRect(12, 400, 100, 36), &GRect(-10, 122, 140, 36), 600, 400);
	
	/*
	TextLayer *ACTV_title, *ACTV_data_t_1, *ACTV_data_t_2, *ACTV_data_1, *ACTV_data_2;
TextLayer *ACTV_other_t, *ACTV_other, *ACTV_time;
	*/
	struct tm *t;
  	time_t temp;        
  	temp = time(NULL);        
  	t = localtime(&temp);
	tick_handler(t, SECOND_UNIT);
}

void window_unload_ACTV(Window *w){
	gbitmap_destroy(previous_i);
	gbitmap_destroy(next_i);
	gbitmap_destroy(play_i);
	gbitmap_destroy(pause_i);
	layer_destroy(ACTV_bgelmnts);
	text_layer_destroy(ACTV_title);
	text_layer_destroy(ACTV_data_t_1);
	text_layer_destroy(ACTV_data_t_2);
	text_layer_destroy(ACTV_data_1);
	text_layer_destroy(ACTV_data_2);
	text_layer_destroy(ACTV_other_t);
	text_layer_destroy(ACTV_other);
	text_layer_destroy(ACTV_time);
	//text_layer_destroy(ACTV_subtitle);
	action_bar_layer_destroy(ACTV_ab);
	isactive = false;
	pedometer_override(false);
	deregister_pedometer();
	tick_timer_service_unsubscribe();
	accel_data_service_unsubscribe();
	reset_timedata();
	reset_motions();
	
	/*
	TextLayer *ACTV_title, *ACTV_data_t_1, *ACTV_data_t_2, *ACTV_data_1, *ACTV_data_2;
	TextLayer *ACTV_other_t, *ACTV_other, *ACTV_time, *ACTV_subtitle;
	*/
}

void ACTV_window_init(){
	ACTV_window = window_create();
	window_set_fullscreen(ACTV_window, true);
	window_set_window_handlers(ACTV_window, (WindowHandlers){
		.load = window_load_ACTV,
		.unload = window_unload_ACTV,
	});
}

void ACTV_window_deinit(){
	window_destroy(ACTV_window);
}