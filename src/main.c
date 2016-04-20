#include <pebble.h>
#include "splash_screen.h"
#include "menu.h"
#include "scroll_text.h"

#define KEY_NUM 0
#define KEY_URL 1
#define KEY_TYPE 2
#define KEY_CONTENTS 3
#define KEY_AUTHOR 4
#define KEY_TITLE 5

char **titles;
char **urls;

static void inbox_received_callback(DictionaryIterator *iterator, void *context){
	
	//APP_LOG(APP_LOG_LEVEL_DEBUG, "%i", dict_find(iterator, KEY_TYPE)->value->int8);
	
	if(dict_find(iterator, KEY_TYPE)->value->int8 == 0){
		int num = dict_find(iterator, KEY_NUM)->value->int8;
		
		for(int i = 0; i < num; i++){
			strcpy(titles[i], dict_find(iterator, 100+i)->value->cstring);
		}
		
		for(int i = 0; i < num; i++){
			strcpy(urls[i], dict_find(iterator, 1000+i)->value->cstring);
		}
		
		splash_remove(false);
		menu_init(titles, urls, num);
		menu_show(true);
	}else if(dict_find(iterator, KEY_TYPE)->value->int8 == 1){
		splash_remove(false);
		scroll_text_init(dict_find(iterator, KEY_CONTENTS)->value->cstring, 
										 dict_find(iterator, KEY_TITLE)->value->cstring, 
										 dict_find(iterator, KEY_AUTHOR)->value->cstring);
		scroll_text_show(true);
	}else if (dict_find(iterator, KEY_TYPE)->value->int8 == 2){
		splash_remove(false);
		splash_destroy();
		splash_init("Set Up On Phone");
		splash_show(true);
	}
}

static void inbox_dropped_callback(AppMessageResult reason, void *context){
	APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context){
	APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context){
	APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send success!!");
}

void init(){
	titles = (char**) malloc(sizeof(char*)*10);
	for(int i = 0; i<10; i++){
		titles[i] = (char*) malloc(sizeof(char) * 255);
	}
	
	urls = (char**) malloc(sizeof(char*)*10);
	for(int i = 0; i<10; i++){
		urls[i] = (char*) malloc(sizeof(char) * 255);
	}
	
	app_message_register_inbox_received(inbox_received_callback);
	app_message_register_inbox_dropped(inbox_dropped_callback);
	app_message_register_outbox_failed(outbox_failed_callback);
	app_message_register_outbox_sent(outbox_sent_callback);
	
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
	
	splash_init("Downloading...");
	splash_show(true);
	
}

void deinit(){
	for(int i = 0; i<10; i++){
		free(titles[i]);
	}
	free(titles);
	
	for(int i = 0; i<10; i++){
		free(urls[i]);
	}
	free(urls);

	splash_destroy();
	menu_destroy();
}

int main(){
	init();
	app_event_loop();
	deinit();
}