#include <pebble.h>
#include "menu.h"
#include "splash_screen.h"

#define KEY_NUM 0
#define KEY_URL 1
#define KEY_TYPE 2
#define KEY_CONTENTS 3

static Window *window;
static MenuLayer *menuL;
char **titles;
char **urls;
int rows;

void menu_show(bool animated){
	window_stack_push(window, animated);
}

uint16_t get_num_rows(MenuLayer *menu_layer, uint16_t section_index, void *data){
	return rows;
}

void draw_rows(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data){
	#ifndef PBL_ROUND
		menu_cell_title_draw(ctx, cell_layer, titles[cell_index->row]);
	#else
		menu_cell_basic_draw(ctx, cell_layer, titles[cell_index->row], NULL, NULL);
	#endif
}

void selected(MenuLayer *menu_layer, MenuIndex *cell_index, void *data){
	splash_init("Downloading");
	splash_show(true);
	
	DictionaryIterator *iter;
	app_message_outbox_begin(&iter);
	
	dict_write_cstring(iter, KEY_URL, urls[cell_index->row]);
	dict_write_int8(iter, KEY_TYPE, 0);
	
	dict_write_end(iter);
	app_message_outbox_send();
}

static int16_t cell_height(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
	#ifndef PBL_ROUND
		return 150;
	#else
		return 44*2;
	#endif
}


void menu_window_load(){
	Layer *mLayer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(mLayer);
	
	menuL = menu_layer_create(bounds);
	menu_layer_set_callbacks(menuL, NULL, (MenuLayerCallbacks){
		.get_num_rows = get_num_rows,
		.draw_row  = draw_rows,
		.select_click  = selected,
		.get_cell_height = cell_height
	});

	menu_layer_set_click_config_onto_window(menuL, window);
	
	layer_add_child(mLayer, menu_layer_get_layer(menuL));
}
void menu_window_unload(){
	menu_layer_destroy(menuL);
}

void menu_init(char **titles_, char **urls_, int rows_){
	titles = titles_;
	rows = rows_;
	urls = urls_;
	
	
	window = window_create();
	window_set_window_handlers(window, (WindowHandlers) {
    .load = menu_window_load,
    .unload = menu_window_unload
  });
}

void menu_destroy(){
	window_destroy(window);
}