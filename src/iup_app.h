#ifndef IUP_APP_H
#define IUP_APP_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <iup.h>
#include <cd.h>
#include <cdiup.h>
#include <wd.h>

#include "scene_builder.h"
#include "renderer.h"
#include "texture.h"

#include "app.h"
#include "plugin.h"
#include "plugin_ui_main.h"

#if 0
	/**
	* This Section includes iup app preparation. 
	* 
	* iup_init_app		=> prepare vom app initialisation (runs before iup main loop)
	*			=> GUI-Creation
				=> Plugin-Structure creation
					=> ui .....
	* iup_init_param 	=> param preparation if needed (runs before iup main loop)
	* iup_run_app 		=> start iup application (starts the iup main loop)
	*/
#endif

void iup_init_app(app_t * app, app_param_t * param);
void iup_init_app_param(app_param_t * param);
bool iup_run_app(app_t * app);
void iup_free_app(app_t * app);

#endif