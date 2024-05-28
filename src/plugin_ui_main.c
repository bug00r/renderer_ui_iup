#include "plugin_ui_main.h"

static void _main_init_(void * data) {
	printf("main init\n");
	#if 0
		/** init context here
			All needed things:
		  */
	#endif
	MainCtx * mctx = (MainCtx *)data;
	mctx->cntplugins = 3;
	mctx->plugins = malloc( mctx->cntplugins * sizeof(Plugin));
	
	Plugin *plugin = &mctx->plugins[0];
	render_plugin(plugin);
	plugin->init(plugin->data);
	
	plugin = &mctx->plugins[1];
	texture_plugin(plugin);
	plugin->init(plugin->data);
	
	plugin = &mctx->plugins[2];
	gfx_algo_test_plugin(plugin);
	plugin->init(plugin->data);

	mctx->frame=NULL;
}

static void _main_free_(void * data) {
	printf("main free\n");
	#if 0
		/** remove all special allocated things from init method..
		  */
	#endif
	MainCtx * mctx = (MainCtx *)data;
	for ( unsigned int i = mctx->cntplugins; i--;) {
		void * plugin_data = mctx->plugins[i].data;
		mctx->plugins[i].free(plugin_data);
	}
	free(mctx->plugins);
	free(mctx);
}

static const char * _main_name_(void * data) {
	printf("main name\n");
	return "Render Bug 0.1";
}
//default exit callback
int exit_cb(void) {
	
	return IUP_CLOSE;
}

static Ihandle *create_file_menu() {
	Ihandle * item_exit = IupItem("Exit", NULL);
	IupSetCallback(item_exit, "ACTION", (Icallback)exit_cb);
	Ihandle * fmenu = IupMenu(item_exit, NULL); 
	Ihandle * submenu = IupSubmenu("File", fmenu);
	return submenu;
}

static void create_andor_open_plugin(Plugin * plugin) {
	const char * pName = plugin->name(NULL);
	printf("create_andor_open_plugin %s\n", pName);
	Ihandle *tabs = IupGetHandle("tabs");
	bool exist = false;
	Ihandle * child = NULL;
	int i = IupGetChildCount(tabs);
	printf("child count %i\n", i);
	for (;i--;) {
		child = IupGetChild(tabs, i);
		if ( child ) {
			char * tabname = IupGetAttribute(child,"TABTITLE");
			printf("found child tab: %s\n",tabname);
			exist = imStrEqual(tabname, pName);
			if ( exist )
				break;
		}
	}
	if ( !exist ) {
		Ihandle * plugin_frame = plugin->frame(plugin->data);
		if(plugin_frame) {
			int tabpos = IupGetChildCount(tabs);
			IupSetAttribute(plugin_frame, "EXPANDCHILDREN", "YES");
			IupSetAttribute(plugin_frame, "TABTITLE", pName);
			IupSetInt(plugin_frame, "TABPOS", tabpos);
			IupAppend(tabs, plugin_frame);
			IupMap(plugin_frame); //important: IupCreate  -> IupAppend -> IupMap
			IupSetInt(tabs, "VALUEPOS", tabpos);
			plugin->prepare(plugin->data);
		}
	} else {
		int tabpos = IupGetInt(child, "TABPOS");
		printf("set current tab %i\n", tabpos);
		char attr[13];
		sprintf(attr, "TABVISIBLE%d", tabpos);
		IupSetAttribute(tabs, attr, "YES");	
		IupSetInt(tabs, "VALUEPOS", tabpos);
	}
	IupRefresh(tabs);
}

static int plugin_cb(Ihandle * handle) {
	Plugin * plugin = (Plugin *)IupGetAttribute(handle, "PLUGIN_OBJ");
	if ( plugin ) {
		create_andor_open_plugin(plugin);
	}
	return IUP_DEFAULT;
}

static Ihandle * create_plugin_menu(MainCtx * mctx) {
	Ihandle * pmenu = IupMenu(NULL);
	for ( unsigned int i = 0; i < mctx->cntplugins ; ++i ) {
		Ihandle *mitem = IupItem(mctx->plugins[i].name(NULL), NULL);
		IupSetCallback(mitem, "ACTION", (Icallback)plugin_cb);
		IupSetAttribute(mitem, "PLUGIN_OBJ", (void *)&mctx->plugins[i]);
		IupAppend( pmenu, mitem);
	}
	Ihandle * submenu = IupSubmenu("Plugins", pmenu);
	return submenu;
}

void * _main_frame_(void * data) {

	#if debug > 0
		printf("main frame\n");
	#endif
	
	MainCtx * mctx = (MainCtx *)data;
	Ihandle * frame = mctx->frame;
	if ( mctx->frame == NULL ) {
		
		#if debug > 0
			printf("create new\n");
		#endif
		
		Ihandle * home = IupScrollBox(IupVbox(IupLabel("Home Tab"), IupFill(),NULL));
		IupSetAttributes(home, "EXPANDCHILDREN=yes, TABTITLE=Home");
		
		Ihandle * tabs = IupTabs(home, NULL);
		IupSetAttribute(tabs, "SHOWCLOSE", "yes");
		
		Ihandle * navtree = IupTree();
		IupSetAttributes(navtree, "EXPAND=VERTICAL, SIZE=100x");
		
		Ihandle * expand_tree = IupExpander(navtree);
		IupSetAttributes(expand_tree, "STATE=CLOSE, BARPOSITION=LEFT, EXPAND=VERTICAL");
		
		Ihandle * main_box = IupHbox(expand_tree, tabs, NULL);
		frame = IupDialog(main_box);
		IupSetAttribute(frame, "TITLE", _main_name_(data));
		IupSetAttribute(frame, "SIZE", "900xFULL");
		
		Ihandle * menu = IupMenu(create_file_menu(), create_plugin_menu(mctx), NULL);
		IupSetAttributeHandle(frame, "MENU", menu);
		
		mctx->frame = frame;
		
		IupSetHandle("tree",navtree);
		IupSetHandle("tabs",tabs);
		IupSetHandle("main_frame",mctx->frame);
	
	}
	
	return frame;
}

void _main_prepare_(void * data) {
	printf("handle main prepare\n");
	Ihandle *navtree = IupGetHandle("tree");
	IupSetAttribute(navtree, "TITLE","System");
	//init plugins here
	MainCtx * mctx = (MainCtx *)data;
	for ( unsigned int i = mctx->cntplugins; i--;) {
		IupSetAttribute(navtree, "ADDLEAF", mctx->plugins[i].name(NULL));
	}	
	IupSetAttribute(navtree, "ADDLEAF","Home");
}

void _main_cleanup_(void * data) {
	printf("main plugin cleanup\n");
	MainCtx * mctx = (MainCtx *)data;
	for ( unsigned int i = mctx->cntplugins; i--;) {
		void * plugin_data = mctx->plugins[i].data;
		mctx->plugins[i].cleanup(plugin_data);
	}	
}

Plugin * main_plugin(Plugin * plugin) {
	plugin->name = _main_name_;
	plugin->frame = _main_frame_;
	plugin->init = _main_init_;
	plugin->free = _main_free_;
	plugin->prepare = _main_prepare_;
	plugin->cleanup = _main_cleanup_;
	plugin->data = malloc(sizeof(MainCtx)); //here malloc
	return plugin;
}
