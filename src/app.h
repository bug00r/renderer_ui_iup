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
} app_param_t;

typedef struct _app_t_{
	int argc; 
	char** argv;
	app_param_t * param;
	app_param_t * (* create_app_param)(void);
	void (* init_app)(struct _app_t_ * app, app_param_t * param);
	void (* init_app_param)(app_param_t * param);
	bool (* run_app)(struct _app_t_ * app);
	void (* free_app)(struct _app_t_ * app);
} app_t;

app_param_t * new_app_param();
void free_app_param(app_param_t * param);
app_t * new_app(int argc, char* argv[]);
void run_app(app_t * app);
void free_app(app_t * app);

#endif