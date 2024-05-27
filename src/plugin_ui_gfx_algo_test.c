#include "plugin_ui_gfx_algo_test.h"

EXTERN_BLOB(zip_resource, 7z);

static void _gfx_algo_test_init_(void * data) {
	printf("main init\n");
	#if 0
		/** init context here
			All needed things:
		  */
	#endif
	GfxAlgoTestCtx * mctx = (GfxAlgoTestCtx *)data;
	
	mctx->frame=NULL;
}

static void _gfx_algo_test_free_(void * data) {
	#if 0
		/** remove all special allocated things from init method..
		  */
	#endif
	GfxAlgoTestCtx * mctx = (GfxAlgoTestCtx *)data;
	free(mctx);
}

static const char * _gfx_algo_test_name_(void * data) {
	return "GFX Algo Tests 0.1";
}


int on_gfx_algo_changed(Ihandle *ih, char *text, int item, int state) {
	
	/* item selected with 1, 0 means deselected */
	if ( state == 1 ) {
		printf("%s\n", text);
		Ihandle *algoparam = (Ihandle *)IupGetAttribute(ih, "algoparameter");
		GfxAlgoTestCtx *ctx = (GfxAlgoTestCtx *)IupGetAttribute(ih, "GFXCTX");
		
		if ( algoparam != NULL && ctx != NULL ) {
			printf("set cur algo\n");
			IupSetInt(algoparam, "VALUEPOS", item - 1);
			ctx->cur_algo = IupGetAttributeHandle(algoparam, "VALUE");
		}
	}
	
	return IUP_DEFAULT;
}

static void gfx_draw_on_canvas(int32_t const * const x, int32_t const * const y, void *data) {
	cdCanvasPixel((cdCanvas *)data, *x, cdCanvasInvertYAxis((cdCanvas *)data, *y), 0);
}

static void gfx_draw_bezier_on_canvas(Vec2 const * const p1, Vec2 const * const p2, void *data) {
	//cdCanvasPixel((cdCanvas *)data, *x, cdCanvasInvertYAxis((cdCanvas *)data, *y), 0);
	cdCanvasLine((cdCanvas *)data, (int)p1->x, (int)p1->y, (int)p2->x, (int)p2->y);
}

static void _gfx_algo_test_draw_line_trigger(Ihandle *_ih) {
	Ihandle *ih = _ih;

	Vec2 start = { IupGetInt((Ihandle *)IupGetAttribute(ih, "lx0"), "SPINVALUE"), IupGetInt((Ihandle *)IupGetAttribute(ih, "ly0"), "SPINVALUE") };
	Vec2 end = { IupGetInt((Ihandle *)IupGetAttribute(ih, "lx1"), "SPINVALUE"), IupGetInt((Ihandle *)IupGetAttribute(ih, "ly1"), "SPINVALUE") };

	void * data = IupGetAttribute((Ihandle*)IupGetAttribute(ih, "gfx_canvas"), "GFX_TEST_CD_CANVAS_DBUFFER");
	geometry_line(&start, &end, gfx_draw_on_canvas, data);
}

static void _gfx_algo_test_draw_circle_trigger(Ihandle *_ih) {
	Ihandle *ih = _ih;

	Vec2 center = { IupGetInt((Ihandle *)IupGetAttribute(ih, "cx"), "SPINVALUE"), IupGetInt((Ihandle *)IupGetAttribute(ih, "cy"), "SPINVALUE") };
	int32_t radius = IupGetInt((Ihandle *)IupGetAttribute(ih, "cr"), "SPINVALUE");
 
	void * data = IupGetAttribute((Ihandle*)IupGetAttribute(ih, "gfx_canvas"), "GFX_TEST_CD_CANVAS_DBUFFER");

	geometry_circle(&center, &radius, gfx_draw_on_canvas, data);

}

static void _gfx_algo_test_draw_ellipse_trigger(Ihandle *_ih) {
	
	Ihandle *ih = _ih;

	Vec2 center = { IupGetInt((Ihandle *)IupGetAttribute(ih, "ex"), "SPINVALUE"), IupGetInt((Ihandle *)IupGetAttribute(ih, "ey"), "SPINVALUE") };
	int32_t a = IupGetInt((Ihandle *)IupGetAttribute(ih, "ea"), "SPINVALUE");
	int32_t b = IupGetInt((Ihandle *)IupGetAttribute(ih, "eb"), "SPINVALUE");
 
	void * data = IupGetAttribute((Ihandle*)IupGetAttribute(ih, "gfx_canvas"), "GFX_TEST_CD_CANVAS_DBUFFER");

	geometry_ellipse(&center, &a, &b, gfx_draw_on_canvas, data);

}

static void _gfx_algo_test_draw_bezier1_trigger(Ihandle *_ih) {

	Ihandle *ih = _ih;

	Vec2 start = {IupGetInt((Ihandle *)IupGetAttribute(ih, "bsx"), "SPINVALUE"), IupGetInt((Ihandle *)IupGetAttribute(ih, "bsy"), "SPINVALUE")};
	Vec2 cp = {IupGetInt((Ihandle *)IupGetAttribute(ih, "bcx"), "SPINVALUE"), IupGetInt((Ihandle *)IupGetAttribute(ih, "bcy"), "SPINVALUE")};
	Vec2 end = {IupGetInt((Ihandle *)IupGetAttribute(ih, "bex"), "SPINVALUE"), IupGetInt((Ihandle *)IupGetAttribute(ih, "bey"), "SPINVALUE")};

	vec2_print(&start);
	vec2_print(&cp);
	vec2_print(&end);

	void * data = IupGetAttribute((Ihandle*)IupGetAttribute(ih, "gfx_canvas"), "GFX_TEST_CD_CANVAS_DBUFFER");

	uint32_t steps = 10;
	
	geometry_bezier1(&start, &cp, &end, &steps, gfx_draw_bezier_on_canvas, data);
}

static void _gfx_algo_test_draw_bezier2_trigger(Ihandle *_ih) {
	Ihandle *ih = _ih;

	Vec2 start = {IupGetInt((Ihandle *)IupGetAttribute(ih, "b2sx"), "SPINVALUE"), IupGetInt((Ihandle *)IupGetAttribute(ih, "b2sy"), "SPINVALUE")};
	Vec2 cp = {IupGetInt((Ihandle *)IupGetAttribute(ih, "b2cx"), "SPINVALUE"), IupGetInt((Ihandle *)IupGetAttribute(ih, "b2cy"), "SPINVALUE")};
	Vec2 cp2 = {IupGetInt((Ihandle *)IupGetAttribute(ih, "b2cx2"), "SPINVALUE"), IupGetInt((Ihandle *)IupGetAttribute(ih, "b2cy2"), "SPINVALUE")};
	Vec2 end = {IupGetInt((Ihandle *)IupGetAttribute(ih, "b2ex"), "SPINVALUE"), IupGetInt((Ihandle *)IupGetAttribute(ih, "b2ey"), "SPINVALUE")};

	void * data = IupGetAttribute((Ihandle*)IupGetAttribute(ih, "gfx_canvas"), "GFX_TEST_CD_CANVAS_DBUFFER");

	uint32_t steps = 10;
	
	geometry_bezier2(&start, &cp, &cp2, &end, &steps, gfx_draw_bezier_on_canvas, data);
}

typedef struct {
    cdCanvas *canvas;
    Vec2 charPos;
} __gfx_algo_render_txt_ctx_t;

static void __rf_text_render_func(float const * const x, float const * const y, void *data)
{
    __gfx_algo_render_txt_ctx_t *ctx = data;
    Vec2 *charPos = &ctx->charPos;

    long used_x = charPos->x + *x;
    long used_y = charPos->y + *y;

	cdCanvasPixel(ctx->canvas, used_x, used_y, 0);
}

static void _gfx_algo_test_render_text_trigger(Ihandle *_ih) {
	Ihandle *ih = _ih;

	float glyphSize = IupGetFloat((Ihandle *)IupGetAttribute(ih, "rfsize"), "SPINVALUE");
	char* text = IupGetAttribute((Ihandle *)IupGetAttribute(ih, "rftxt"), "VALUE");
	cdCanvas *canvas = (cdCanvas*)IupGetAttribute((Ihandle*)IupGetAttribute(ih, "gfx_canvas"), "GFX_TEST_CD_CANVAS_DBUFFER");

	RFProvider* provider = get_default_provider();

    RFCtx rf_ctx;
    rfont_init(&rf_ctx, provider);

	Vec2 charPos = { IupGetFloat((Ihandle *)IupGetAttribute(ih, "rfposx"), "SPINVALUE") , 
					   IupGetFloat((Ihandle *)IupGetAttribute(ih, "rfposy"), "SPINVALUE")};
	__gfx_algo_render_txt_ctx_t renderCtx = {canvas, charPos}; 

	rfont_raster_text(&rf_ctx, (unsigned char const * const)text, glyphSize, __rf_text_render_func, &renderCtx);
}

static int on_gfx_trigger_drawing(Ihandle* ih) {

	GfxAlgoTestCtx *ctx = (GfxAlgoTestCtx *)IupGetAttribute(ih, "GFXCTX");

	if ( ctx != NULL && ctx->cur_algo != NULL ) {
		DRAW_TRIGGER trigger = (DRAW_TRIGGER)IupGetAttribute(ctx->cur_algo, "drawtrigger");
		if ( trigger != NULL ) {
			cdCanvas *canvas = (cdCanvas*)IupGetAttribute((Ihandle*)IupGetAttribute(ih, "gfx_canvas"), "GFX_TEST_CD_CANVAS_DBUFFER");
			cdCanvasActivate(canvas);
			trigger(ctx->cur_algo);
			cdCanvasDeactivate(canvas);
			cdCanvasFlush(canvas);
			
		}
	}

	return IUP_DEFAULT;
}

static int on_gfx_trigger_clear_canvas(Ihandle* ih) {
	cdCanvas *canvas = (cdCanvas*)IupGetAttribute((Ihandle*)IupGetAttribute(ih, "gfx_canvas"), "GFX_TEST_CD_CANVAS_DBUFFER");

	cdCanvasActivate(canvas);
	cdCanvasClear(canvas);
	cdCanvasDeactivate(canvas);
	cdCanvasFlush(canvas);
	

	return IUP_DEFAULT;
}

static void _gfx_algo_test_map_canvas(Ihandle * ih)
{
	cdCanvas * cd_canvas = cdCreateCanvas(CD_IUPDBUFFERRGB, ih);
	printf("INIT CANVAS %p\n", cd_canvas);
	IupSetAttribute(ih, "GFX_TEST_CD_CANVAS_DBUFFER", (void*)cd_canvas);

}

static int _gfx_algo_test_wheel_cb_canvas(Ihandle * ih, float delta, int x, int y, char *status)
{
	return IUP_IGNORE;
}

void * _gfx_algo_test_frame_(void * data) {

	#if debug > 0
		printf("main frame\n");
	#endif
	
	GfxAlgoTestCtx * mctx = (GfxAlgoTestCtx *)data;
	Ihandle * frame = mctx->frame;
	if ( mctx->frame == NULL ) {
		archive_resource_t* ar = archive_resource_memory(&_binary_zip_resource_7z_start, (size_t)&_binary_zip_resource_7z_size);
		
		xml_source_t* xml_src = xml_source_from_resname(ar, "gfx_algo_test_ui");

		iup_xml_builder_t *builder = iup_xml_builder_new();

		iup_xml_builder_add_bytes(builder, "gfx_algo_test_ui",  (const char *)xml_src->src_data, *xml_src->src_size);

		iup_xml_builder_add_callback(builder, "on_gfx_algo_changed", (Icallback)on_gfx_algo_changed);
		iup_xml_builder_add_callback(builder, "on_gfx_trigger_drawing", (Icallback)on_gfx_trigger_drawing);
		iup_xml_builder_add_callback(builder, "on_gfx_trigger_clear_canvas", (Icallback)on_gfx_trigger_clear_canvas);
		iup_xml_builder_add_callback(builder, "gfx_algo_test_wheel_cb_canvas", (Icallback)_gfx_algo_test_wheel_cb_canvas);
		iup_xml_builder_add_callback(builder, "gfx_algo_test_map_canvas", (Icallback)_gfx_algo_test_map_canvas);

		iup_xml_builder_add_user_data(builder, "GFXCTX", (void*)mctx);
		iup_xml_builder_add_user_data(builder, "drawlinetrigger", (void*)_gfx_algo_test_draw_line_trigger);
		iup_xml_builder_add_user_data(builder, "drawcircletrigger", (void*)_gfx_algo_test_draw_circle_trigger);
		iup_xml_builder_add_user_data(builder, "drawellipsetrigger", (void*)_gfx_algo_test_draw_ellipse_trigger);
		iup_xml_builder_add_user_data(builder, "drawbezier1trigger", (void*)_gfx_algo_test_draw_bezier1_trigger);
		iup_xml_builder_add_user_data(builder, "drawbezier2trigger", (void*)_gfx_algo_test_draw_bezier2_trigger);
		iup_xml_builder_add_user_data(builder, "rendertexttrigger", (void*)_gfx_algo_test_render_text_trigger);
		
		iup_xml_builder_parse(builder);

		Ihandle *mres = iup_xml_builder_get_result(builder, "gfx_algo_test_ui");

		mctx->frame = iup_xml_builder_get_main(mres);
		mctx->cur_algo = iup_xml_builder_get_name(mres, "gfx_algo_1");
		if ( mctx->cur_algo == NULL ) printf("MISSING INIT ALGO!!!"); 
	}
	
	return mctx->frame;
}

void _gfx_algo_test_prepare_(void * data) {
	//init plugins here
	GfxAlgoTestCtx * mctx = (GfxAlgoTestCtx *)data;
}

void _gfx_algo_test_cleanup_(void * data) {
	GfxAlgoTestCtx * mctx = (GfxAlgoTestCtx *)data;	
}

Plugin * gfx_algo_test_plugin(Plugin * plugin) {
	plugin->name	= _gfx_algo_test_name_;
	plugin->frame	= _gfx_algo_test_frame_;
	plugin->init	= _gfx_algo_test_init_;
	plugin->free 	= _gfx_algo_test_free_;
	plugin->prepare = _gfx_algo_test_prepare_;
	plugin->cleanup = _gfx_algo_test_cleanup_;
	plugin->data 	= malloc(sizeof(GfxAlgoTestCtx)); //here malloc
	return plugin;
}
