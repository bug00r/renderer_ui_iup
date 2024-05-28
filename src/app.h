#ifndef APP_H
#define APP_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct {
	char *title;
	int width, height;
	bool fullscreen;
	void * app_data;
} AppParam;

typedef struct _App_{
	int argc; 
	char** argv;
	AppParam * param;
	AppParam * (* create_app_param)(void);
	void (* init_app)(struct _App_ * app, AppParam * param);
	void (* init_app_param)(AppParam * param);
	bool (* run_app)(struct _App_ * app);
	void (* free_app)(struct _App_ * app);
} App;

AppParam * new_app_param();
void free_app_param(AppParam * param);
App * new_app(int argc, char* argv[]);
void run_app(App * app);
void free_app(App * app);

#endif