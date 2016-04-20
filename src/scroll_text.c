#include <pebble.h>
#include "scroll_text.h"
#include "splash_screen.h"

#define paddingX 4
#define paddingY 4
#define maxH 2000


Window *window;
ScrollLayer *scrollL;
TextLayer *textL;
TextLayer *titleL;
TextLayer *authorL;
char *text;
char *title;
char *author;

void scroll_text_reload(){
	window_stack_pop(false);
	scroll_text_init(text, title, author);
	scroll_text_show(false);
}


void scroll_text_show(bool animate){
	window_stack_push(window, animate);
}

void scroll_text_init(char* text_, char* title_, char* author_){
	title = title_;
	author = author_;
	
	window = window_create();
	Layer *rootLayer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(rootLayer);
	
	
	scrollL = scroll_layer_create(bounds);
	
	GTextAttributes *s_attributes = graphics_text_attributes_create();
  graphics_text_attributes_enable_screen_text_flow(s_attributes, PBL_IF_ROUND_ELSE(7,0));
	
	titleL = text_layer_create(GRect(paddingX, paddingY, bounds.size.w - (paddingX*2), maxH));
	text_layer_set_text(titleL, title);
	text_layer_set_font(titleL, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
	text_layer_set_background_color(titleL, GColorClear);
	text_layer_set_text_alignment(titleL, GTextAlignmentCenter);
	scroll_layer_add_child(scrollL, text_layer_get_layer(titleL));

	
	GSize Tsize = graphics_text_layout_get_content_size_with_attributes(title, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), GRect(paddingX, paddingY, bounds.size.w - (paddingX*2), maxH), GTextOverflowModeWordWrap, GTextAlignmentCenter, s_attributes);
	
	authorL = text_layer_create(GRect(paddingX, paddingY + Tsize.h , bounds.size.w - (paddingX*2), maxH));
	text_layer_set_text(authorL, author);
	text_layer_set_font(authorL, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
	text_layer_set_background_color(authorL, GColorClear);
	text_layer_set_text_alignment(authorL, GTextAlignmentRight);
	scroll_layer_add_child(scrollL, text_layer_get_layer(authorL));
	
	GSize Asize = graphics_text_layout_get_content_size_with_attributes(author, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD), GRect(paddingX, paddingY, bounds.size.w - (paddingX*2), maxH), GTextOverflowModeWordWrap, GTextAlignmentRight, s_attributes);
	
	textL = text_layer_create(GRect(paddingX, paddingY + Tsize.h + Asize.h, bounds.size.w - (paddingX*2), maxH));
	text_layer_set_text(textL, text_);
	text_layer_set_font(textL, fonts_get_system_font(FONT_KEY_GOTHIC_18));
	text_layer_set_background_color(textL, GColorClear);
	scroll_layer_add_child(scrollL, text_layer_get_layer(textL));
	
	GSize max_size = text_layer_get_content_size(textL);
	
	scroll_layer_set_content_size(scrollL, GSize(bounds.size.w, max_size.h + (paddingY * 3) + Tsize.h + Asize.h));
	//scroll_layer_set_shadow_hidden(scrollL, false);
	scroll_layer_set_click_config_onto_window(scrollL, window);
	
	layer_add_child(rootLayer, scroll_layer_get_layer(scrollL));
		
	#if defined(PBL_ROUND)
		text_layer_enable_screen_text_flow_and_paging(titleL, 2);
		text_layer_enable_screen_text_flow_and_paging(authorL, 2);
		text_layer_enable_screen_text_flow_and_paging(textL, 2);
		scroll_layer_set_paging(scrollL, true);
	#endif
}

void scroll_text_destroy(){
	window_destroy(window);
	scroll_layer_destroy(scrollL);
	text_layer_destroy(textL);
}
