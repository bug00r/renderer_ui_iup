#include "app.h"

AppParam * new_app_param() {
	AppParam * new_app_param = malloc(sizeof(AppParam));
	return new_app_param;
}

void free_app_param(AppParam * param) {
	free(param);
}

static void default_init_app(App * app, AppParam * param) {
	printf("You not implement init_app function!!\n");
	
}

static void default_init_app_param(AppParam * param) {
	printf("You not implement init_app_param function!!\n");
}

static AppParam * default_create_app_param() {
	return new_app_param();
}

static bool default_run_app(App * app) {
	printf("You not implement run_app function!!\n");
	return false;
}

void free_app(App * app) {
	if (app->param) 
		free_app_param(app->param);
	free(app);
}

App * new_app(int argc, char* argv[]) {
	App * new_app = malloc(sizeof(App));
	new_app->argc = argc;
	new_app->argv = argv;
	new_app->create_app_param = default_create_app_param;
	new_app->init_app = default_init_app;
	new_app->init_app_param = default_init_app_param;
	new_app->run_app = default_run_app;
	new_app->free_app = free_app;
	new_app->param = NULL;
	return new_app;
}

static bool check_app_if_valid(const App * app) {
	return app->create_app_param && 
		   app->init_app_param   &&
		   (app->init_app_param != default_init_app_param) &&
		   app->init_app   &&
		   (app->init_app != default_init_app);
}

void run_app(App * app) {
	if(check_app_if_valid(app)) {
		AppParam * params = app->create_app_param();
		app->init_app_param(params);
		app->init_app(app, params);
		app->param = params;	
		bool runsuccess = app->run_app(app);
		if (!runsuccess) {
			printf("runnig app failed => exit()\n");
			return;
		}
		app->free_app(app);
	} else {
		printf("invalid app!!\n");
		return;
	}
}

