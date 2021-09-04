#include "plugin_ui_gfx_algo_test.h"

EXTERN_BLOB(zip_resource, 7z);

static void _gfx_algo_test_init_(void * data) {
	printf("main init\n");
	#if 0
		/** init context here
			All needed things:
		  */
	#endif
	gfx_algo_test_ctx_t * mctx = (gfx_algo_test_ctx_t *)data;
	
	mctx->frame=NULL;
}

static void _gfx_algo_test_free_(void * data) {
	#if 0
		/** remove all special allocated things from init method..
		  */
	#endif
	gfx_algo_test_ctx_t * mctx = (gfx_algo_test_ctx_t *)data;
	free(mctx);
}

static const char * _gfx_algo_test_name_(void * data) {
	return "GFX Algo Tests 0.1";
}

void * _gfx_algo_test_frame_(void * data) {

	#if debug > 0
		printf("main frame\n");
	#endif
	
	gfx_algo_test_ctx_t * mctx = (gfx_algo_test_ctx_t *)data;
	Ihandle * frame = mctx->frame;
	if ( mctx->frame == NULL ) {
		archive_resource_t* ar = archive_resource_memory(&_binary_zip_resource_7z_start, (size_t)&_binary_zip_resource_7z_size);
		
		xml_source_t* xml_src = xml_source_from_resname(ar, "gfx_algo_test_ui");

		iup_xml_builder_t *builder = iup_xml_builder_new();

		iup_xml_builder_add_bytes(builder, "gfx_algo_test_ui",  (const char *)xml_src->src_data, *xml_src->src_size);

		//iup_xml_builder_add_callback(builder, "previewclb", (Icallback)_iup_xb_prev_show_preview);
		//iup_xml_builder_add_callback(builder, "caretclb", (Icallback)on_xml_src_caret_changed);

		iup_xml_builder_parse(builder);

		Ihandle *mres = iup_xml_builder_get_result(builder, "gfx_algo_test_ui");

		mctx->frame = iup_xml_builder_get_main(mres);
	}
	
	return mctx->frame;
}

void _gfx_algo_test_prepare_(void * data) {
	//init plugins here
	gfx_algo_test_ctx_t * mctx = (gfx_algo_test_ctx_t *)data;
}

void _gfx_algo_test_cleanup_(void * data) {
	gfx_algo_test_ctx_t * mctx = (gfx_algo_test_ctx_t *)data;	
}

plugin_t * gfx_algo_test_plugin(plugin_t * plugin) {
	plugin->name	= _gfx_algo_test_name_;
	plugin->frame	= _gfx_algo_test_frame_;
	plugin->init	= _gfx_algo_test_init_;
	plugin->free 	= _gfx_algo_test_free_;
	plugin->prepare = _gfx_algo_test_prepare_;
	plugin->cleanup = _gfx_algo_test_cleanup_;
	plugin->data 	= malloc(sizeof(gfx_algo_test_ctx_t)); //here malloc
	return plugin;
}
