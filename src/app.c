#include "app.h"

app_param_t * new_app_param() {
	app_param_t * new_app_param = malloc(sizeof(app_param_t));
	return new_app_param;
}

void free_app_param(app_param_t * param) {
	free(param);
}

static void default_init_app(app_t * app, app_param_t * param) {
	printf("You not implement init_app function!!\n");
	
}

static void default_init_app_param(app_param_t * param) {
	printf("You not implement init_app_param function!!\n");
}

static app_param_t * default_create_app_param() {
	return new_app_param();
}

static bool default_run_app(app_t * app) {
	printf("You not implement run_app function!!\n");
	return false;
}

void free_app(app_t * app) {
	if (app->param) 
		free_app_param(app->param);
	free(app);
}

app_t * new_app(int argc, char* argv[]) {
	app_t * new_app = malloc(sizeof(app_t));
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

static bool check_app_if_valid(const app_t * app) {
	return app->create_app_param && 
		   app->init_app_param   &&
		   (app->init_app_param != default_init_app_param) &&
		   app->init_app   &&
		   (app->init_app != default_init_app);
}

void run_app(app_t * app) {
	if(check_app_if_valid(app)) {
		app_param_t * params = app->create_app_param();
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

