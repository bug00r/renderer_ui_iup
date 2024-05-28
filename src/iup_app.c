#include "iup_app.h"

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

void iup_init_app(App * app, AppParam * param) {
	printf("calling iup init app\n");
	#if 0
		/** 
			Iup should opened here to setup iup app environment.
				- open Iup Environment
				- Plugin preparation callbacks for lazy ui elements
					- plugin configuration should implement like app => every plugin got func
						example:  renderer_plugin(...) return inf renderer plugin prototype
								  texturing_plugin(...) return inf texturing plugin prototype
				- Main GUI Initialisation
		 */
	#endif
	IupOpen(&app->argc, &app->argv);
	
	IupSetGlobal("UTF8MODE", "YES");

	Plugin * plugin = new_plugin(); 
	main_plugin(plugin);
	plugin->init(plugin->data);
	MainCtx * mainctx = (MainCtx *)plugin->data;
	printf("load main ctx with: %i\n", mainctx->frame);
	Ihandle * maindlg = plugin->frame(plugin->data);
	IupShowXY(maindlg, IUP_CENTER, IUP_CENTER);
	plugin->prepare(plugin->data);
	
	param->app_data = (void*)plugin;
}

void iup_init_app_param(AppParam * param) {
	printf("calling iup init param\n");
}

bool iup_run_app(App * app)
{ 
  printf("call main loop\n");
  IupMainLoop(); 

  return true;
}

void iup_free_app(App * app) {
	printf("iup app free\n");
	AppParam * param = app->param;
	Plugin *plugin = (Plugin *)param->app_data;
	plugin->cleanup(plugin->data);
	plugin->free(plugin->data);
	IupClose();
}

