#include "plugin_ui_renderer.h"

typedef struct {
	renderer_t *renderer;
	scene_t *scene;
	vec3_t from, to;
	float l,r,t,b,f,n;
	cRGB_t  bgcolor;
} render_context_t;



static void convertFramebuffer_iupCanvas(renderer_t * renderer, cdCanvas * canvas) 
{
	unsigned int bi=0, samplestart;
	int i, j, cj, imgW = renderer->imgWidth, 
				  imgH = renderer->imgHeight;
	const int us = renderer->used_samples, bw = renderer->bufWidth;
	cRGB_t fc;
	cRGB_t * fb = renderer->frameBuffer;
	cRGB_t * c = fb;

	for (j = imgH; j--; )
    {
	  bi = j * bw;
	  cj = ((imgH-1) - j);
	  for (i = imgW; i--; )
	  {
		fc.r = 0.f, fc.g = 0.f, fc.b = 0.f;
		samplestart = bi + (i*us);
		switch (us) {
			case 8:
				c = fb + samplestart + 7;
				fc.r += c->r; fc.g += c->g; fc.b += c->b;
			case 7:
				c = fb + samplestart + 6;
				fc.r += c->r; fc.g += c->g; fc.b += c->b;
			case 6:
				c = fb + samplestart + 5;
				fc.r += c->r; fc.g += c->g; fc.b += c->b;
			case 5:
				c = fb + samplestart + 4;
				fc.r += c->r; fc.g += c->g; fc.b += c->b;
			case 4:
				c = fb + samplestart + 3;
				fc.r += c->r; fc.g += c->g; fc.b += c->b;
			case 3:
			    c = fb + samplestart + 2;
				fc.r += c->r; fc.g += c->g; fc.b += c->b;
			case 2:
				c = fb + samplestart + 1;
				fc.r += c->r; fc.g += c->g; fc.b += c->b;
			case 1:
				c = fb + samplestart;
				fc.r += c->r; fc.g += c->g; fc.b += c->b;
				break;
		}
		//for (unsigned int sample = us; sample--;)
		//{
		//	c = fb + samplestart + sample;
		//	fc.r += c->r;
		//	fc.g += c->g;
		//	fc.b += c->b;
		//}
		
		cdCanvasPixel(canvas, i, cj, ((unsigned char)(fc.r * 255.f) << 16) |
									 ((unsigned char)(fc.g * 255.f) << 8) |
									 ((unsigned char)(fc.b * 255.f)));
	  }
    }
}

void render_scence_again()
{
	render_context_t * rctx = (render_context_t *)IupGetGlobal("RCTX");
	renderer_t * renderer = rctx->renderer;
	camera_t * curcam = &renderer->camera;
	scene_t * scene = rctx->scene;
	vec3_t * from = &rctx->from;
	vec3_t * to = &rctx->to;

	camera_lookAt(curcam, from, to);

	mat4_mul_dest(&curcam->transformation ,&curcam->view, &curcam->projection);
	renderer_clear_frame(renderer);
	render_scene(renderer, scene);	
}

static void render_canvas(cdCanvas * _canvas)
{
	cdCanvas *canvas = _canvas;
	render_context_t * rctx = (render_context_t *)IupGetGlobal("RCTX");
	renderer_t *renderer = rctx->renderer;
	
	if ( canvas != NULL && renderer != NULL)
	{
		convertFramebuffer_iupCanvas(renderer, canvas);
	} 
	else 
	{
		printf("something is missing: canvas: %i ,renderer: %i\n",
							canvas != NULL, renderer != NULL);
	}
}

static void __refresh_canvas() {
	cdCanvas *canvas = (cdCanvas*)IupGetAttribute(NULL, "RENDERER_CD_CANVAS_DBUFFER");
	cdCanvasActivate(canvas);
	render_canvas(canvas);
	cdCanvasDeactivate(canvas);
	cdCanvasFlush(canvas);
}


static void render_scene_again_and_refresh_canvas()
{
	render_scence_again();
	__refresh_canvas();
}


/*
	Here we need different kind of renderer:
		- Orthogonal
		- perspective
		- opengl
	
	this is for check all rendering options with z buffering inside. An easier way to find error.

*/

static void __color_scene(scene_t* scene) {

}

static render_context_t* create_test_renderer()
{
	printf("create_test_renderer\n");
	render_context_t * render_ctx = malloc(sizeof(render_context_t));
	render_ctx->bgcolor = (cRGB_t){0.0f, 0.0f, 0.0f};
	render_ctx->from = (vec3_t){0.f, 1.f, -1.5f };
	//render_ctx->from = (vec3_t){-1.f, 1.5f, -1.5f }; //perspective
	render_ctx->to = (vec3_t){0.f, 0.f, .0f};
	render_ctx->renderer = renderer_new(512, 512, &render_ctx->bgcolor, 1);
	render_ctx->renderer->projection = RP_PERSPECTIVE;
	float view = 2.f;
	render_ctx->l = -view;
	render_ctx->r = view;
	render_ctx->t = view;
	render_ctx->b = -view;
	render_ctx->f = 5.f;
	render_ctx->n = 0.01f;
	scene_t * scene;
	float waterfall_data[240] = { 
		//0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.010309f,0.041237f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.989691f,0.773196f,0.000000f,0.000000f,0.350515f,0.030928f,0.515464f,0.000000f,0.979381f,0.876289f,0.000000f,0.000000f,0.412371f,0.020619f,0.659794f,0.000000f,0.989583f,0.906250f,0.000000f,0.000000f,0.354167f,0.197917f,0.697917f,0.000000f,0.989691f,0.948454f,0.000000f,0.010309f,0.783505f,0.360825f,0.773196f,0.000000f,0.979381f,0.958763f,0.000000f,0.010309f,0.721649f,0.268041f,0.804124f,0.000000f,0.989691f,0.969072f,0.000000f,0.000000f,0.742268f,0.412371f,0.762887f,0.000000f,1.000000f,0.938144f,0.000000f,0.092784f,0.989691f,0.608247f,0.927835f,0.000000f,0.989362f,1.000000f,0.000000f,0.095745f,0.914894f,0.531915f,0.904255f,0.000000f,1.000000f,0.989796f,0.000000f,0.040816f,0.938776f,0.693878f,0.948980f,0.000000f,1.000000f,1.000000f,0.000000f,0.247423f,0.989691f,0.762887f,0.969072f,0.000000f,1.000000f,1.000000f,0.000000f,0.312500f,0.989583f,0.750000f,0.968750f,0.000000f,1.000000f,1.000000f,0.000000f,0.239583f,1.000000f,0.843750f,0.958333f,0.000000f,1.000000f,1.000000f,0.000000f,0.479167f,1.000000f,0.885417f,1.000000f,0.000000f,1.000000f,1.000000f,0.000000f,0.391753f,1.000000f,0.793814f,0.989691f,0.000000f,1.000000f,1.000000f,0.000000f,0.593750f,1.000000f,0.927083f,1.000000f,0.000000f,1.000000f,1.000000f,0.000000f,0.625000f,1.000000f,0.989583f,1.000000f,0.000000f,1.000000f,1.000000f,0.000000f,0.731959f,0.979381f,0.989691f,1.000000f,0.000000f,1.000000f,1.000000f,0.010417f,0.802083f,0.989583f,1.000000f,0.989583f,0.000000f,1.000000f,1.000000f,0.000000f,0.865979f,1.000000f,1.000000f,1.000000f,0.000000f,1.000000f,1.000000f,0.000000f,0.750000f,1.000000f,0.979167f,1.000000f,0.000000f,1.000000f,1.000000f,0.000000f,0.865979f,1.000000f,1.000000f,1.000000f
		//0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.927835f,0.742268f,0.000000f,0.000000f,0.237113f,0.000000f,0.525773f,0.000000f,0.876289f,0.804124f,0.000000f,0.000000f,0.432990f,0.000000f,0.536082f,0.000000f,0.927835f,0.886598f,0.000000f,0.000000f,0.247423f,0.092784f,0.556701f,0.000000f,0.969072f,0.896907f,0.000000f,0.010309f,0.659794f,0.226804f,0.680412f,0.000000f,0.979381f,0.958763f,0.000000f,0.020619f,0.783505f,0.144330f,0.670103f,0.000000f,0.989691f,0.917526f,0.000000f,0.000000f,0.618557f,0.134021f,0.711340f,0.000000f,1.000000f,0.979381f,0.000000f,0.030928f,0.896907f,0.711340f,0.927835f,0.000000f,0.989691f,0.989691f,0.000000f,0.030928f,0.907216f,0.494845f,0.865979f,0.000000f,0.969072f,0.979381f,0.000000f,0.030928f,0.824742f,0.536082f,0.824742f,0.000000f,0.989691f,0.989691f,0.000000f,0.134021f,0.948454f,0.752577f,0.979381f,0.000000f,1.000000f,0.989691f,0.000000f,0.195876f,0.948454f,0.670103f,1.000000f,0.000000f,1.000000f,0.989691f,0.000000f,0.144330f,0.989691f,0.804124f,0.979381f,0.000000f,1.000000f,1.000000f,0.000000f,0.309278f,1.000000f,0.865979f,1.000000f,0.000000f,1.000000f,1.000000f,0.000000f,0.422680f,1.000000f,0.711340f,1.000000f,0.000000f,1.000000f,1.000000f,0.000000f,0.247423f,0.979381f,0.752577f,0.969072f,0.000000f,1.000000f,1.000000f,0.000000f,0.412371f,1.000000f,0.783505f,1.000000f,0.000000f,1.000000f,1.000000f,0.010417f,0.479167f,1.000000f,0.927083f,1.000000f,0.000000f,0.989691f,1.000000f,0.010309f,0.567010f,0.979381f,0.989691f,1.000000f,0.000000f,1.000000f,1.000000f,0.000000f,0.546392f,1.000000f,1.000000f,1.000000f,0.000000f,1.000000f,1.000000f,0.000000f,0.541667f,1.000000f,0.989583f,1.000000f,0.000000f,1.000000f,1.000000f,0.000000f,0.474227f,0.979381f,0.958763f,1.000000f,0.000000f,1.000000f,1.000000f,0.000000f,0.541667f,1.000000f,0.989583f,1.000000f,0.000000f,1.000000f,1.000000f,0.000000f,0.505155f,0.989691f,1.000000f,1.000000f,0.000000f,1.000000f,1.000000f,0.010309f,0.463918f,1.000000f,1.000000f,1.000000f,0.000000f,1.000000f,1.000000f,0.000000f,0.577320f,1.000000f,1.000000f,1.000000f,0.000000f,1.000000f,1.000000f,0.010309f,0.546392f,1.000000f,0.969072f,1.000000f,0.000000f,1.000000f,1.000000f,0.000000f,0.639175f,1.000000f,0.989691f,1.000000f,0.000000f,1.000000f,1.000000f,0.041237f,0.742268f,1.000000f,0.979381f,1.000000f
		//0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.010309f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.865979f,0.536082f,0.000000f,0.000000f,0.030928f,0.020619f,0.298969f,0.000000f,0.948454f,0.628866f,0.000000f,0.000000f,0.082474f,0.000000f,0.381443f,0.000000f,0.690722f,0.701031f,0.000000f,0.000000f,0.113402f,0.010309f,0.278351f,0.000000f,1.000000f,0.783505f,0.000000f,0.000000f,0.216495f,0.082474f,0.432990f,0.000000f,0.948454f,0.659794f,0.000000f,0.000000f,0.298969f,0.092784f,0.422680f,0.000000f,1.000000f,0.814433f,0.000000f,0.000000f,0.319588f,0.144330f,0.505155f,0.000000f,0.989691f,0.917526f,0.000000f,0.000000f,0.711340f,0.443299f,0.670103f,0.000000f,0.989691f,0.896907f,0.000000f,0.000000f,0.773196f,0.288660f,0.670103f,0.000000f,1.000000f,0.927835f,0.000000f,0.000000f,0.814433f,0.422680f,0.680412f,0.000000f,1.000000f,0.958333f,0.000000f,0.010417f,0.916667f,0.697917f,0.927083f,0.000000f,1.000000f,0.989583f,0.000000f,0.010417f,0.833333f,0.687500f,0.895833f,0.000000f,1.000000f,0.989691f,0.000000f,0.020619f,0.948454f,0.556701f,0.948454f,0.000000f,1.000000f,0.989691f,0.000000f,0.061856f,0.958763f,0.742268f,0.927835f,0.000000f,1.000000f,1.000000f,0.000000f,0.041237f,0.958763f,0.804124f,0.969072f,0.000000f,1.000000f,1.000000f,0.000000f,0.031250f,0.937500f,0.666667f,0.968750f,0.000000f,1.000000f,0.989583f,0.000000f,0.177083f,0.989583f,0.833333f,0.937500f,0.000000f,1.000000f,0.989691f,0.000000f,0.247423f,0.979381f,0.824742f,1.000000f,0.000000f,1.000000f,0.989691f,0.000000f,0.247423f,0.824742f,0.896907f,1.000000f,0.000000f,1.000000f,1.000000f,0.000000f,0.395833f,1.000000f,0.885417f,1.000000f,0.000000f,1.000000f,1.000000f,0.000000f,0.416667f,0.937500f,0.937500f,1.000000f,0.000000f,1.000000f,1.000000f,0.000000f,0.474227f,0.989691f,0.896907f,0.989691f,0.000000f,1.000000f,0.989691f,0.000000f,0.587629f,1.000000f,0.989691f,1.000000f,0.000000f,1.000000f,0.989691f,0.000000f,0.597938f,1.000000f,1.000000f,1.000000f,0.000000f,1.000000f,1.000000f,0.000000f,0.484536f,0.969072f,1.000000f,1.000000f,0.000000f,1.000000f,0.989691f,0.000000f,0.463918f,0.989691f,0.989691f,1.000000f,0.000000f,1.000000f,1.000000f,0.000000f,0.484536f,0.979381f,0.927835f,0.989691f,0.000000f,1.000000f,1.000000f,0.000000f,0.536082f,0.969072f,0.979381f,1.000000f,0.000000f,1.000000f,1.000000f,0.000000f,0.546392f,1.000000f,0.876289f,1.000000f
		0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.468750f,0.218750f,0.000000f,0.000000f,0.020833f,0.000000f,0.114583f,0.000000f,0.447917f,0.187500f,0.000000f,0.000000f,0.000000f,0.000000f,0.135417f,0.000000f,0.422680f,0.206186f,0.000000f,0.000000f,0.010309f,0.000000f,0.247423f,0.000000f,0.597938f,0.288660f,0.000000f,0.000000f,0.020619f,0.000000f,0.226804f,0.000000f,0.587629f,0.360825f,0.000000f,0.000000f,0.061856f,0.000000f,0.216495f,0.000000f,0.670103f,0.391753f,0.000000f,0.000000f,0.072165f,0.000000f,0.309278f,0.000000f,0.855670f,0.546392f,0.000000f,0.000000f,0.113402f,0.020619f,0.360825f,0.000000f,0.948454f,0.680412f,0.000000f,0.000000f,0.185567f,0.010309f,0.567010f,0.000000f,0.927835f,0.597938f,0.000000f,0.000000f,0.309278f,0.072165f,0.453608f,0.000000f,0.927835f,0.835052f,0.000000f,0.000000f,0.463918f,0.072165f,0.639175f,0.000000f,0.989691f,0.886598f,0.000000f,0.000000f,0.288660f,0.185567f,0.618557f,0.000000f,0.979381f,0.855670f,0.000000f,0.000000f,0.412371f,0.113402f,0.804124f,0.000000f,1.000000f,0.907216f,0.000000f,0.000000f,0.659794f,0.247423f,0.907216f,0.000000f,1.000000f,0.907216f,0.000000f,0.000000f,0.587629f,0.185567f,0.886598f,0.000000f,1.000000f,0.885417f,0.000000f,0.010417f,0.625000f,0.270833f,0.895833f,0.000000f,1.000000f,0.979381f,0.000000f,0.072165f,0.711340f,0.257732f,0.876289f,0.000000f,1.000000f,0.948454f,0.000000f,0.072165f,0.556701f,0.278351f,0.855670f,0.000000f,1.000000f,0.948454f,0.000000f,0.041237f,0.927835f,0.298969f,0.958763f,0.000000f,1.000000f,1.000000f,0.000000f,0.288660f,0.948454f,0.432990f,0.979381f,0.000000f,1.000000f,0.958763f,0.000000f,0.309278f,0.886598f,0.371134f,0.989691f,0.000000f,0.989691f,0.989691f,0.000000f,0.175258f,0.855670f,0.505155f,0.969072f,0.000000f,1.000000f,1.000000f,0.000000f,0.350515f,0.876289f,0.505155f,0.979381f,0.000000f,1.000000f,0.989691f,0.000000f,0.412371f,0.876289f,0.670103f,1.000000f,0.000000f,1.000000f,0.969072f,0.000000f,0.412371f,0.855670f,0.752577f,1.000000f,0.000000f,1.000000f,0.989691f,0.000000f,0.360825f,0.886598f,0.773196f,1.000000f,0.000000f,1.000000f,1.000000f,0.000000f,0.298969f,0.938144f,0.711340f,0.979381f,0.000000f,1.000000f,1.000000f,0.000000f,0.432990f,0.938144f,0.835052f,0.969072f,0.000000f,1.000000f,1.000000f,0.000000f,0.288660f,0.824742f,0.804124f,1.000000f
	};

	scene = scene_create_waterfall_diagram(&waterfall_data[0], 30, 8);
	
	//scene = scene_create_triangle();
	//scene = scene_create_test_all();
	//scene = scene_create_test_cube();
	//scene = scene_create_tree();
	//scene = scene_create_test();
	render_ctx->scene = scene;
	IupSetGlobal("RCTX", (void*) render_ctx);
	config_camera_perspective(&render_ctx->renderer->camera, &render_ctx->from, &render_ctx->to, 
				render_ctx->l, render_ctx->r, render_ctx->t, render_ctx->b, render_ctx->n, render_ctx->f);
	render_scene(render_ctx->renderer, scene);
	return render_ctx;
}

/*static int resize_cb(Ihandle *ih, int canvas_w, int canvas_h)
{
	cdCanvas *canvas = (cdCanvas*)IupGetAttribute(ih, "_CD_CANVAS");
	//cdCanvasActivate(canvas);
	return IUP_DEFAULT;
}*/

static int action(Ihandle *ih, float fposx, float fposy)
{	
	if(IupGetInt(NULL, "RENDER_LIVE"))
	{
		__refresh_canvas();
	}

	return IUP_DEFAULT;
}

static void map_canvas(Ihandle * ih)
{
	cdCanvas * cd_canvas = cdCreateCanvas(CD_IUPDBUFFERRGB, ih);
	IupSetAttribute(NULL, "RENDERER_CD_CANVAS_DBUFFER", (void*)cd_canvas);

}

static int wheel_cb_canvas(Ihandle * ih, float delta, int x, int y, char *status)
{
	return IUP_IGNORE;
}

static Ihandle * create__render_canvas()
{
	Ihandle *canvas = IupCanvas(NULL);
	IupSetAttributes(canvas, "RASTERSIZE=512x512, EXPAND=NO, CANFOCUS=NO");
	
	//IupSetCallback(canvas, "RESIZE_CB",  (Icallback)resize_cb);
	IupSetCallback(canvas, "ACTION",  (Icallback)action);
	IupSetCallback(canvas, "MAP_CB",  (Icallback)map_canvas);
	IupSetCallback(canvas, "WHEEL_CB",  (Icallback)wheel_cb_canvas);
	
	return canvas;
}

static int toggle_live_view(Ihandle * toggle, int state)
{
	IupSetInt(NULL, "RENDER_LIVE", state);
	
	if(state)
	{	
		__refresh_canvas();
	}
	 
	return IUP_DEFAULT;
}

static Ihandle* create_live_view_toggle()
{
	Ihandle * liveview = IupToggle("live view", NULL);
	IupSetCallback(liveview, "ACTION",  (Icallback)toggle_live_view);
	
	return liveview;
}

static void unmap_frame(Ihandle * ih)
{
	cdCanvas *canvas = (cdCanvas*)IupGetAttribute(ih, "_CD_CANVAS");
	cdKillCanvas(canvas);
}

static Ihandle * create__render_frame()
{
	Ihandle * canvas = create__render_canvas();
	Ihandle * liveview = create_live_view_toggle();
	Ihandle * container = IupVbox(canvas, liveview, NULL);
	Ihandle *frame = IupFrame(container);
	IupSetAttribute(frame, "TITLE", "Render Window");
	IupSetCallback(frame, "UNMAP_CB", (Icallback)unmap_frame);
	return frame;
}

static int render_view_zoom(float zoom)
{
	render_context_t * rctx = (render_context_t *)IupGetGlobal("RCTX");
	vec3_t * from = &rctx->from;
	vec3_t normalized;
	vec3_normalize_dest( &normalized, from);
	//vec3_mul(&normalized, zoom);
	normalized.x *= zoom;
	normalized.y *= zoom;
	normalized.z *= zoom;
	//vec3_add(from, &normalized);
	from->x += normalized.x;
	from->y += normalized.y;
	from->z += normalized.z;
	
	render_scene_again_and_refresh_canvas();
	return IUP_DEFAULT;
}

static int render_view_zoom_factor_changed(Ihandle * ih)
{
	float lz = IupGetFloat(NULL, "ZOOM");
	float cz = IupGetFloat(ih, "VALUE");
	IupSetFloat(NULL, "ZOOM", cz);
	render_view_zoom(cz-lz);
	return IUP_DEFAULT;
}

static Ihandle * create_render_zoom_options_frame()
{
	Ihandle * zoomfactor = IupVal(IUP_HORIZONTAL);
	render_context_t * rctx = (render_context_t *)IupGetGlobal("RCTX");
	vec3_t * from = &rctx->from;
	float length = vec3_length(from);
	IupSetFloat(zoomfactor, "MIN", 0.001f);
	IupSetFloat(zoomfactor, "MAX", length*4);
	IupSetFloat(zoomfactor, "VALUE", length);
	IupSetFloat(NULL, "ZOOM", length);
	IupSetCallback(zoomfactor, "VALUECHANGED_CB",(Icallback)render_view_zoom_factor_changed);
	
 	Ihandle * container = IupHbox(IupLabel("Zoom:"), zoomfactor, NULL);
	Ihandle *frame = IupFrame(container);
	IupSetAttribute(frame, "TITLE", "Zoom Options");
	return frame;
}

static int render_view_rot(float rot, mat3_t * (* matcreate)( mat3_t * dest, const float rot) )
{
	mat3_t rotmat;
	render_context_t * rctx = (render_context_t *)IupGetGlobal("RCTX");
	vec3_t * from = &rctx->from;
	(* matcreate)(&rotmat, rot);
	mat_vec_mul_3(&rotmat, from);
	
	render_scene_again_and_refresh_canvas();

	return IUP_DEFAULT;
}

static int render_view_rotx_factor_changed(Ihandle * ih)
{
	float lr = IupGetFloat(ih, "ROT");
	float cr = IupGetFloat(ih, "VALUE");
	IupSetFloat(ih, "ROT", cr);
	
	render_view_rot(cr-lr, create_rot_x_mat_dest);
	return IUP_DEFAULT;
}

static int render_view_roty_factor_changed(Ihandle * ih)
{
	float lr = IupGetFloat(ih, "ROT");
	float cr = IupGetFloat(ih, "VALUE");
	IupSetFloat(ih, "ROT", cr);
	
	render_view_rot(cr-lr, create_rot_y_mat_dest);
	return IUP_DEFAULT;
}

static int render_view_rotz_factor_changed(Ihandle * ih)
{	
	float lr = IupGetFloat(ih, "ROT");
	float cr = IupGetFloat(ih, "VALUE");
	IupSetFloat(ih, "ROT", cr);
	
	render_view_rot(cr-lr, create_rot_z_mat_dest);
	return IUP_DEFAULT;
}

static Ihandle * create_render_rotation_options_frame()
{
	#if 0
		//rotation over x axis elements
	#endif
	Ihandle * rotxfactor = IupVal(IUP_HORIZONTAL);
	IupSetFloat(rotxfactor, "MAX", 179.9f);
	IupSetFloat(rotxfactor, "VALUE", 89.9f);
	IupSetFloat(rotxfactor, "ROT", 89.9f);
	IupSetCallback(rotxfactor, "VALUECHANGED_CB",(Icallback)render_view_rotx_factor_changed);
	
	Ihandle * rotx_container = IupHbox(IupLabel("x-Axis: "), rotxfactor, NULL);
	
	#if 0
		//rotation over y axis elements
	#endif
	Ihandle * rotyfactor = IupVal(IUP_HORIZONTAL);
	IupSetFloat(rotyfactor, "MAX", 179.9f);
	IupSetFloat(rotyfactor, "VALUE", 89.9f);
	IupSetFloat(rotyfactor, "ROT", 89.9f);
	IupSetCallback(rotyfactor, "VALUECHANGED_CB",(Icallback)render_view_roty_factor_changed);
	
	Ihandle * roty_container = IupHbox(IupLabel("y-Axis: "), rotyfactor, NULL);
	
	#if 0
		//rotation over z axis elements
	#endif
	Ihandle * rotzfactor = IupVal(IUP_HORIZONTAL);
	IupSetFloat(rotzfactor, "MAX", 179.9f);
	IupSetFloat(rotzfactor, "VALUE", 89.9f);
	IupSetFloat(rotzfactor, "ROT", 89.9f);
	IupSetCallback(rotzfactor, "VALUECHANGED_CB",(Icallback)render_view_rotz_factor_changed);
	
	Ihandle * rotz_container = IupHbox(IupLabel("z-Axis: "), rotzfactor, NULL);
	
	Ihandle *frame = IupFrame(IupVbox(rotx_container, roty_container, rotz_container, NULL));
	IupSetAttribute(frame, "TITLE", "Rotation Options");
	return frame;
}

Ihandle * create_and_show_dialog()
{
	render_context_t *render_ctx = create_test_renderer();
	Ihandle *renderer_1 = create_renderer_context(render_ctx);
	
	Ihandle *render_frame = create__render_frame();
	Ihandle *render_zoom_options_frame = create_render_zoom_options_frame();
	Ihandle *render_rotation_options_frame = create_render_rotation_options_frame();
	Ihandle *move_opt_box = IupVbox(render_zoom_options_frame, render_rotation_options_frame, NULL);
	Ihandle *render_0 = IupHbox( render_frame, move_opt_box, NULL);
	

	Ihandle *maindlg = IupVbox(render_0, renderer_1, NULL);
	IupSetAttribute(maindlg, "RCTX", (void*)render_ctx);
	
	
	return maindlg;
}

//###########

static void _render_init_(void * data) {
	printf("render init\n");
	#if 0
		/** init context here
			All needed things:
		  */
	#endif
	render_ctx_t * mctx = (render_ctx_t *)data;
	mctx->frame=NULL;
	
}

static void free_render_context(render_context_t **_rctx) {
	
	if (_rctx != NULL && *_rctx != NULL) {
		render_context_t *rctx = *_rctx;
		if (rctx->renderer)  
			renderer_free(rctx->renderer);
		if(rctx->scene)
			free_scene(rctx->scene);
		free(rctx);
	}
	
}

static void _render_free_(void * data) {
	printf("render free\n");
	render_context_t * rctx = (render_context_t *)IupGetGlobal("RCTX");
	free_render_context(&rctx);
}

static const char * _render_name_(void * data) {
	printf("renderer name\n");
	return "SW-Renderer";
}

void * _render_frame_(void * data) {
	printf("render frame\n");
	render_ctx_t * mctx = (render_ctx_t *)data;
	if ( mctx->frame == NULL ) {
		printf("render frame create new\n");
		mctx->frame = create_and_show_dialog();
	}
	return mctx->frame;
}

void _render_prepare_(void * data) {
	printf("handle render prepare\n");
	render_scene_again_and_refresh_canvas();
}

void _render_cleanup_(void * data) {
	printf("render plugin cleanup\n");
}

/**
	void * data;
	const char * (*name)(void * data);
	void * (*frame)(void * data);
	void (*init)(void * data);
	void (*free)(void * data);
	void (*prepare)(void * data);
	void (*cleanup)(void * data);
*/
plugin_t * render_plugin(plugin_t * plugin) {
	plugin->name = _render_name_;
	plugin->frame = _render_frame_;
	plugin->init = _render_init_;
	plugin->free = _render_free_;
	plugin->prepare = _render_prepare_;
	plugin->cleanup = _render_cleanup_;
	plugin->data = malloc(sizeof(render_ctx_t));;
	return plugin;
}

