#include "plugin_ui_proto.h"

static void _proto_init_(void * data) {
	printf("main init\n");
	#if 0
		/** init context here
			All needed things:
		  */
	#endif
	ProtoCtx * mctx = (ProtoCtx *)data;
	
	mctx->frame=NULL;
}

static void _proto_free_(void * data) {
	#if 0
		/** remove all special allocated things from init method..
		  */
	#endif
	ProtoCtx * mctx = (ProtoCtx *)data;
	free(mctx);
}

static const char * _proto_name_(void * data) {
	return "Proto 0.1";
}

void * _proto_frame_(void * data) {

	#if debug > 0
		printf("main frame\n");
	#endif
	
	ProtoCtx * mctx = (ProtoCtx *)data;
	Ihandle * frame = mctx->frame;
	if ( mctx->frame == NULL ) {
	
	}
	
	return frame;
}

void _proto_prepare_(void * data) {
	//init plugins here
	ProtoCtx * mctx = (ProtoCtx *)data;
}

void _proto_cleanup_(void * data) {
	ProtoCtx * mctx = (ProtoCtx *)data;	
}

Plugin * proto_plugin(Plugin * plugin) {
	plugin->name	= _proto_name_;
	plugin->frame	= _proto_frame_;
	plugin->init	= _proto_init_;
	plugin->free 	= _proto_free_;
	plugin->prepare = _proto_prepare_;
	plugin->cleanup = _proto_cleanup_;
	plugin->data 	= malloc(sizeof(ProtoCtx)); //here malloc
	return plugin;
}
