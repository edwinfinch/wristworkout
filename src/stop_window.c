#include <pebble.h>
#include "stop_window.h"
#include "ACTV_window.h"
#include "pedometer.h"
	
Window *stop_window;
ActionBarLayer *stop_ab;
TextLayer *stop_warning_layer;
GBitmap *checkmark;
	
void stop_program(void *ctx, ClickRecognizerRef poop){
	reset_steps();
	window_stack_remove(stop_window, true);
	window_stack_pop(true);
}

void stop_config(void *cont){
	window_single_click_subscribe(BUTTON_ID_SELECT, stop_program);
}

void wl_stop(Window *w){
	checkmark = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CHECKMARK);
	
	stop_warning_layer = text_layer_create(GRect(6, 0, 120, 168));
	text_layer_set_font(stop_warning_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
	text_layer_set_text(stop_warning_layer, "Closing the window will stop anything running.");
	layer_add_child(window_get_root_layer(stop_window), text_layer_get_layer(stop_warning_layer));
	
	stop_ab = action_bar_layer_create();
	action_bar_layer_set_click_config_provider(stop_ab, stop_config);
	action_bar_layer_set_icon(stop_ab, BUTTON_ID_SELECT, checkmark);
	action_bar_layer_add_to_window(stop_ab, stop_window);
}

void wul_stop(Window *w){
	gbitmap_destroy(checkmark);
	text_layer_destroy(stop_warning_layer);
	action_bar_layer_destroy(stop_ab);
}

void stop_window_launch(){
	window_stack_push(stop_window, true);
}

void stop_window_init(){
	stop_window = window_create();
	window_set_window_handlers(stop_window, (WindowHandlers){
		.load = wl_stop,
		.unload = wul_stop,
	});
}

void stop_window_deinit(){
	window_destroy(stop_window);
}