#include "plugin_ui_renderer.h"

typedef struct {
	renderer_t *renderer;
	scene_t *scene;
	vec3_t from, to;
	float l,r,t,b,f,n;
	cRGB_t  bgcolor;
	Ihandle* cam_from[3];
	Ihandle* cam_to[3];
	Ihandle* cam_left[3];
	Ihandle* cam_up[3];
	Ihandle* cam_forward[3];
	Ihandle* mat4_view[16];
	Ihandle* mat4_proj[16];
	Ihandle* mat4_trans[16];
	Ihandle* frustum_near[15];
	Ihandle* frustum_far[15];
	Ihandle* frustum_left[15];
	Ihandle* frustum_right[15];
	Ihandle* frustum_top[15];
	Ihandle* frustum_bottom[15];
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

		for (unsigned int sample = us; sample--;)
		{
			c = fb + samplestart + sample;
			fc.r += c->r;
			fc.g += c->g;
			fc.b += c->b;
		}
		
		cdCanvasPixel(canvas, i, cj, ((unsigned char)(fc.r * 255.f) << 16) |
									 ((unsigned char)(fc.g * 255.f) << 8) |
									 ((unsigned char)(fc.b * 255.f)));
	  }
    }



}

static void update_debug_mat4(Ihandle **targets, mat4_t *data) {
	IupSetFloat(targets[0], "TITLE", data->_11);
	IupSetFloat(targets[1], "TITLE", data->_12);
	IupSetFloat(targets[2], "TITLE", data->_13);
	IupSetFloat(targets[3], "TITLE", data->_14);
	IupSetFloat(targets[4], "TITLE", data->_21);
	IupSetFloat(targets[5], "TITLE", data->_22);
	IupSetFloat(targets[6], "TITLE", data->_23);
	IupSetFloat(targets[7], "TITLE", data->_24);
	IupSetFloat(targets[8], "TITLE", data->_31);
	IupSetFloat(targets[9], "TITLE", data->_32);
	IupSetFloat(targets[10], "TITLE", data->_33);
	IupSetFloat(targets[11], "TITLE", data->_34);
	IupSetFloat(targets[12], "TITLE", data->_41);
	IupSetFloat(targets[13], "TITLE", data->_42);
	IupSetFloat(targets[14], "TITLE", data->_43);
	IupSetFloat(targets[15], "TITLE", data->_44);
}

static void update_debug_vec3(Ihandle **targets, vec3_t *data) {
	IupSetFloat(targets[0], "TITLE", data->x);
	IupSetFloat(targets[1], "TITLE", data->y);
	IupSetFloat(targets[2], "TITLE", data->z);
}

static void update_debug_frustum_plane(Ihandle **targets, plane_t *plane)
{
	vec3_t *vec = &plane->lt;
	IupSetFloat(targets[0], "TITLE", vec->x);
	IupSetFloat(targets[1], "TITLE", vec->y);
	IupSetFloat(targets[2], "TITLE", vec->z);

	vec = &plane->rt;
	IupSetFloat(targets[3], "TITLE", vec->x);
	IupSetFloat(targets[4], "TITLE", vec->y);
	IupSetFloat(targets[5], "TITLE", vec->z);

	vec = &plane->lb;
	IupSetFloat(targets[6], "TITLE", vec->x);
	IupSetFloat(targets[7], "TITLE", vec->y);
	IupSetFloat(targets[8], "TITLE", vec->z);

	vec = &plane->rb;
	IupSetFloat(targets[9], "TITLE", vec->x);
	IupSetFloat(targets[10], "TITLE", vec->y);
	IupSetFloat(targets[11], "TITLE", vec->z);

	vec = &plane->normal;
	IupSetFloat(targets[12], "TITLE", vec->x);
	IupSetFloat(targets[13], "TITLE", vec->y);
	IupSetFloat(targets[14], "TITLE", vec->z);
}

static void update_debug_view(renderer_t* renderer) {
	camera_t *cam = &renderer->camera;
	render_context_t * rctx = (render_context_t *)IupGetGlobal("RCTX");

	update_debug_vec3(&rctx->cam_from[0], &cam->from);
	update_debug_vec3(&rctx->cam_to[0], &cam->to);
	update_debug_vec3(&rctx->cam_left[0], &cam->left);
	update_debug_vec3(&rctx->cam_up[0], &cam->up);
	update_debug_vec3(&rctx->cam_forward[0], &cam->forward);

	update_debug_mat4(&rctx->mat4_view[0], &cam->view);
	update_debug_mat4(&rctx->mat4_proj[0], &cam->projection);
	update_debug_mat4(&rctx->mat4_trans[0], &cam->transformation);

	update_debug_frustum_plane(&rctx->frustum_near[0], &cam->frustum.near);
	update_debug_frustum_plane(&rctx->frustum_far[0], &cam->frustum.far);
	update_debug_frustum_plane(&rctx->frustum_left[0], &cam->frustum.left);
	update_debug_frustum_plane(&rctx->frustum_right[0], &cam->frustum.right);
	update_debug_frustum_plane(&rctx->frustum_top[0], &cam->frustum.top);
	update_debug_frustum_plane(&rctx->frustum_bottom[0], &cam->frustum.bottom);
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
	//printf("NEW CAM POSITION\n\n");
	//print_camera((const camera_t *)curcam);
	mat4_mul_dest(&curcam->transformation ,&curcam->view, &curcam->projection);
	renderer_clear_frame(renderer);
	render_scene(renderer, scene);
	update_debug_view(renderer);
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

static scene_t* __renderer_textured_cube(renderer_t *_renderer)
{
	renderer_t * renderer = _renderer;

	unsigned int w_ = 512;
	unsigned int h_ = 512;
	
	texture_t * texture_fractals = texture_new(w_,h_);
	
	mandelbrot_t *mb = mandelbrot_new(w_, h_);
	mb->minreal = -2.f;//-1.3f;
	mb->maxreal = 0.5f;//-1.f;
	mb->minimag = -1.f;//-.3f;
	mb->maximag = 1.f;//0.f;
	mb->cntiterations = 20;
	create_mandelbrot(mb);
	
	mandelbrot_to_texture(mb, texture_fractals, mandelbrot_color_line_int_rgb);

	int texId = texture_cache_register(renderer->texture_cache, texture_fractals);
    
	mandelbrot_free(mb);

	return scene_create_texture_test();
}

static render_context_t* create_test_renderer()
{
	printf("create_test_renderer\n");
	render_context_t * render_ctx = malloc(sizeof(render_context_t));
	render_ctx->bgcolor = (cRGB_t){0.7f, 0.7f, 0.7f};
	render_ctx->from = (vec3_t){-3.5f, 3.f, -4.5f };
	//render_ctx->from = (vec3_t){0.f, 0.f, 2.0f }; //perspective
	render_ctx->to = (vec3_t){0.f, 0.0f, 0.0f};
	render_ctx->renderer = renderer_new(512, 512, &render_ctx->bgcolor, 2);
	render_ctx->renderer->projection = RP_PERSPECTIVE;
	float view = 1.5f;
	render_ctx->l = -view;
	render_ctx->r = view;
	render_ctx->t = view;
	render_ctx->b = -view;
	render_ctx->f = 3.5f;
	render_ctx->n = 0.2f;
	scene_t * scene;
	texture_cache_t *texCache = render_ctx->renderer->texture_cache;
	float waterfall_data[240] = { 
		//0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.010309f,0.041237f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.989691f,0.773196f,0.000000f,0.000000f,0.350515f,0.030928f,0.515464f,0.000000f,0.979381f,0.876289f,0.000000f,0.000000f,0.412371f,0.020619f,0.659794f,0.000000f,0.989583f,0.906250f,0.000000f,0.000000f,0.354167f,0.197917f,0.697917f,0.000000f,0.989691f,0.948454f,0.000000f,0.010309f,0.783505f,0.360825f,0.773196f,0.000000f,0.979381f,0.958763f,0.000000f,0.010309f,0.721649f,0.268041f,0.804124f,0.000000f,0.989691f,0.969072f,0.000000f,0.000000f,0.742268f,0.412371f,0.762887f,0.000000f,1.000000f,0.938144f,0.000000f,0.092784f,0.989691f,0.608247f,0.927835f,0.000000f,0.989362f,1.000000f,0.000000f,0.095745f,0.914894f,0.531915f,0.904255f,0.000000f,1.000000f,0.989796f,0.000000f,0.040816f,0.938776f,0.693878f,0.948980f,0.000000f,1.000000f,1.000000f,0.000000f,0.247423f,0.989691f,0.762887f,0.969072f,0.000000f,1.000000f,1.000000f,0.000000f,0.312500f,0.989583f,0.750000f,0.968750f,0.000000f,1.000000f,1.000000f,0.000000f,0.239583f,1.000000f,0.843750f,0.958333f,0.000000f,1.000000f,1.000000f,0.000000f,0.479167f,1.000000f,0.885417f,1.000000f,0.000000f,1.000000f,1.000000f,0.000000f,0.391753f,1.000000f,0.793814f,0.989691f,0.000000f,1.000000f,1.000000f,0.000000f,0.593750f,1.000000f,0.927083f,1.000000f,0.000000f,1.000000f,1.000000f,0.000000f,0.625000f,1.000000f,0.989583f,1.000000f,0.000000f,1.000000f,1.000000f,0.000000f,0.731959f,0.979381f,0.989691f,1.000000f,0.000000f,1.000000f,1.000000f,0.010417f,0.802083f,0.989583f,1.000000f,0.989583f,0.000000f,1.000000f,1.000000f,0.000000f,0.865979f,1.000000f,1.000000f,1.000000f,0.000000f,1.000000f,1.000000f,0.000000f,0.750000f,1.000000f,0.979167f,1.000000f,0.000000f,1.000000f,1.000000f,0.000000f,0.865979f,1.000000f,1.000000f,1.000000f
		//0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.927835f,0.742268f,0.000000f,0.000000f,0.237113f,0.000000f,0.525773f,0.000000f,0.876289f,0.804124f,0.000000f,0.000000f,0.432990f,0.000000f,0.536082f,0.000000f,0.927835f,0.886598f,0.000000f,0.000000f,0.247423f,0.092784f,0.556701f,0.000000f,0.969072f,0.896907f,0.000000f,0.010309f,0.659794f,0.226804f,0.680412f,0.000000f,0.979381f,0.958763f,0.000000f,0.020619f,0.783505f,0.144330f,0.670103f,0.000000f,0.989691f,0.917526f,0.000000f,0.000000f,0.618557f,0.134021f,0.711340f,0.000000f,1.000000f,0.979381f,0.000000f,0.030928f,0.896907f,0.711340f,0.927835f,0.000000f,0.989691f,0.989691f,0.000000f,0.030928f,0.907216f,0.494845f,0.865979f,0.000000f,0.969072f,0.979381f,0.000000f,0.030928f,0.824742f,0.536082f,0.824742f,0.000000f,0.989691f,0.989691f,0.000000f,0.134021f,0.948454f,0.752577f,0.979381f,0.000000f,1.000000f,0.989691f,0.000000f,0.195876f,0.948454f,0.670103f,1.000000f,0.000000f,1.000000f,0.989691f,0.000000f,0.144330f,0.989691f,0.804124f,0.979381f,0.000000f,1.000000f,1.000000f,0.000000f,0.309278f,1.000000f,0.865979f,1.000000f,0.000000f,1.000000f,1.000000f,0.000000f,0.422680f,1.000000f,0.711340f,1.000000f,0.000000f,1.000000f,1.000000f,0.000000f,0.247423f,0.979381f,0.752577f,0.969072f,0.000000f,1.000000f,1.000000f,0.000000f,0.412371f,1.000000f,0.783505f,1.000000f,0.000000f,1.000000f,1.000000f,0.010417f,0.479167f,1.000000f,0.927083f,1.000000f,0.000000f,0.989691f,1.000000f,0.010309f,0.567010f,0.979381f,0.989691f,1.000000f,0.000000f,1.000000f,1.000000f,0.000000f,0.546392f,1.000000f,1.000000f,1.000000f,0.000000f,1.000000f,1.000000f,0.000000f,0.541667f,1.000000f,0.989583f,1.000000f,0.000000f,1.000000f,1.000000f,0.000000f,0.474227f,0.979381f,0.958763f,1.000000f,0.000000f,1.000000f,1.000000f,0.000000f,0.541667f,1.000000f,0.989583f,1.000000f,0.000000f,1.000000f,1.000000f,0.000000f,0.505155f,0.989691f,1.000000f,1.000000f,0.000000f,1.000000f,1.000000f,0.010309f,0.463918f,1.000000f,1.000000f,1.000000f,0.000000f,1.000000f,1.000000f,0.000000f,0.577320f,1.000000f,1.000000f,1.000000f,0.000000f,1.000000f,1.000000f,0.010309f,0.546392f,1.000000f,0.969072f,1.000000f,0.000000f,1.000000f,1.000000f,0.000000f,0.639175f,1.000000f,0.989691f,1.000000f,0.000000f,1.000000f,1.000000f,0.041237f,0.742268f,1.000000f,0.979381f,1.000000f
		//0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.010309f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.865979f,0.536082f,0.000000f,0.000000f,0.030928f,0.020619f,0.298969f,0.000000f,0.948454f,0.628866f,0.000000f,0.000000f,0.082474f,0.000000f,0.381443f,0.000000f,0.690722f,0.701031f,0.000000f,0.000000f,0.113402f,0.010309f,0.278351f,0.000000f,1.000000f,0.783505f,0.000000f,0.000000f,0.216495f,0.082474f,0.432990f,0.000000f,0.948454f,0.659794f,0.000000f,0.000000f,0.298969f,0.092784f,0.422680f,0.000000f,1.000000f,0.814433f,0.000000f,0.000000f,0.319588f,0.144330f,0.505155f,0.000000f,0.989691f,0.917526f,0.000000f,0.000000f,0.711340f,0.443299f,0.670103f,0.000000f,0.989691f,0.896907f,0.000000f,0.000000f,0.773196f,0.288660f,0.670103f,0.000000f,1.000000f,0.927835f,0.000000f,0.000000f,0.814433f,0.422680f,0.680412f,0.000000f,1.000000f,0.958333f,0.000000f,0.010417f,0.916667f,0.697917f,0.927083f,0.000000f,1.000000f,0.989583f,0.000000f,0.010417f,0.833333f,0.687500f,0.895833f,0.000000f,1.000000f,0.989691f,0.000000f,0.020619f,0.948454f,0.556701f,0.948454f,0.000000f,1.000000f,0.989691f,0.000000f,0.061856f,0.958763f,0.742268f,0.927835f,0.000000f,1.000000f,1.000000f,0.000000f,0.041237f,0.958763f,0.804124f,0.969072f,0.000000f,1.000000f,1.000000f,0.000000f,0.031250f,0.937500f,0.666667f,0.968750f,0.000000f,1.000000f,0.989583f,0.000000f,0.177083f,0.989583f,0.833333f,0.937500f,0.000000f,1.000000f,0.989691f,0.000000f,0.247423f,0.979381f,0.824742f,1.000000f,0.000000f,1.000000f,0.989691f,0.000000f,0.247423f,0.824742f,0.896907f,1.000000f,0.000000f,1.000000f,1.000000f,0.000000f,0.395833f,1.000000f,0.885417f,1.000000f,0.000000f,1.000000f,1.000000f,0.000000f,0.416667f,0.937500f,0.937500f,1.000000f,0.000000f,1.000000f,1.000000f,0.000000f,0.474227f,0.989691f,0.896907f,0.989691f,0.000000f,1.000000f,0.989691f,0.000000f,0.587629f,1.000000f,0.989691f,1.000000f,0.000000f,1.000000f,0.989691f,0.000000f,0.597938f,1.000000f,1.000000f,1.000000f,0.000000f,1.000000f,1.000000f,0.000000f,0.484536f,0.969072f,1.000000f,1.000000f,0.000000f,1.000000f,0.989691f,0.000000f,0.463918f,0.989691f,0.989691f,1.000000f,0.000000f,1.000000f,1.000000f,0.000000f,0.484536f,0.979381f,0.927835f,0.989691f,0.000000f,1.000000f,1.000000f,0.000000f,0.536082f,0.969072f,0.979381f,1.000000f,0.000000f,1.000000f,1.000000f,0.000000f,0.546392f,1.000000f,0.876289f,1.000000f
		0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.000000f,0.468750f,0.218750f,0.000000f,0.000000f,0.020833f,0.000000f,0.114583f,0.000000f,0.447917f,0.187500f,0.000000f,0.000000f,0.000000f,0.000000f,0.135417f,0.000000f,0.422680f,0.206186f,0.000000f,0.000000f,0.010309f,0.000000f,0.247423f,0.000000f,0.597938f,0.288660f,0.000000f,0.000000f,0.020619f,0.000000f,0.226804f,0.000000f,0.587629f,0.360825f,0.000000f,0.000000f,0.061856f,0.000000f,0.216495f,0.000000f,0.670103f,0.391753f,0.000000f,0.000000f,0.072165f,0.000000f,0.309278f,0.000000f,0.855670f,0.546392f,0.000000f,0.000000f,0.113402f,0.020619f,0.360825f,0.000000f,0.948454f,0.680412f,0.000000f,0.000000f,0.185567f,0.010309f,0.567010f,0.000000f,0.927835f,0.597938f,0.000000f,0.000000f,0.309278f,0.072165f,0.453608f,0.000000f,0.927835f,0.835052f,0.000000f,0.000000f,0.463918f,0.072165f,0.639175f,0.000000f,0.989691f,0.886598f,0.000000f,0.000000f,0.288660f,0.185567f,0.618557f,0.000000f,0.979381f,0.855670f,0.000000f,0.000000f,0.412371f,0.113402f,0.804124f,0.000000f,1.000000f,0.907216f,0.000000f,0.000000f,0.659794f,0.247423f,0.907216f,0.000000f,1.000000f,0.907216f,0.000000f,0.000000f,0.587629f,0.185567f,0.886598f,0.000000f,1.000000f,0.885417f,0.000000f,0.010417f,0.625000f,0.270833f,0.895833f,0.000000f,1.000000f,0.979381f,0.000000f,0.072165f,0.711340f,0.257732f,0.876289f,0.000000f,1.000000f,0.948454f,0.000000f,0.072165f,0.556701f,0.278351f,0.855670f,0.000000f,1.000000f,0.948454f,0.000000f,0.041237f,0.927835f,0.298969f,0.958763f,0.000000f,1.000000f,1.000000f,0.000000f,0.288660f,0.948454f,0.432990f,0.979381f,0.000000f,1.000000f,0.958763f,0.000000f,0.309278f,0.886598f,0.371134f,0.989691f,0.000000f,0.989691f,0.989691f,0.000000f,0.175258f,0.855670f,0.505155f,0.969072f,0.000000f,1.000000f,1.000000f,0.000000f,0.350515f,0.876289f,0.505155f,0.979381f,0.000000f,1.000000f,0.989691f,0.000000f,0.412371f,0.876289f,0.670103f,1.000000f,0.000000f,1.000000f,0.969072f,0.000000f,0.412371f,0.855670f,0.752577f,1.000000f,0.000000f,1.000000f,0.989691f,0.000000f,0.360825f,0.886598f,0.773196f,1.000000f,0.000000f,1.000000f,1.000000f,0.000000f,0.298969f,0.938144f,0.711340f,0.979381f,0.000000f,1.000000f,1.000000f,0.000000f,0.432990f,0.938144f,0.835052f,0.969072f,0.000000f,1.000000f,1.000000f,0.000000f,0.288660f,0.824742f,0.804124f,1.000000f
	};

	scene = scene_create_waterfall_diagram(texCache, &waterfall_data[0], 30, 8);
	
	//scene = scene_create_triangle();
	//scene = scene_create_test_all(1.f);
	
	//cRGB_t txtCol = {1.f, 0.f, 0.f};

	//scene = scene_create_polys(); //created from triangualtion
	//scene = __renderer_textured_cube(render_ctx->renderer);
	//scene = __renderer_font_quad(render_ctx->renderer);
	//scene = scene_create_text_quad(texCache, 0.5f, 120.f, "Hallo Testwelt!!", &txtCol);
	
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

int button_cb_canvas(Ihandle* ih, int button, int pressed, int x, int y, char* status) {
	printf("x: %i, y: %i btn: %i, pressed: %i\n", x, y, button, pressed);
}

static Ihandle * create__render_canvas()
{
	Ihandle *canvas = IupCanvas(NULL);
	IupSetAttributes(canvas, "RASTERSIZE=512x512, EXPAND=NO, CANFOCUS=NO");
	
	//IupSetCallback(canvas, "RESIZE_CB",  (Icallback)resize_cb);
	IupSetCallback(canvas, "ACTION",  (Icallback)action);
	IupSetCallback(canvas, "MAP_CB",  (Icallback)map_canvas);
	IupSetCallback(canvas, "BUTTON_CB",  (Icallback)button_cb_canvas);
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

static int render_view_zoom(float zoom)
{
	render_context_t * rctx = (render_context_t *)IupGetGlobal("RCTX");
	vec3_t * from = &rctx->from;
	vec3_t normalized;
	vec3_normalize_dest( &normalized, from);
	vec3_mul(&normalized, zoom);
	vec3_add(from, &normalized);
	
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

typedef void (* VMODE_CHANGE_FN)(renderer_t* renderer);

static int __renderer_change_vmode(VMODE_CHANGE_FN modechangefn)
{
	render_context_t * rctx = (render_context_t *)IupGetGlobal("RCTX");
	renderer_t * renderer = rctx->renderer;
	modechangefn(renderer);
	render_scene_again_and_refresh_canvas();

	return IUP_DEFAULT;
}

int choose_vmode_solid(Ihandle* ih, int state) 
{
	return __renderer_change_vmode(renderer_set_vmode_solid);
}

int choose_vmode_wireframe(Ihandle* ih, int state) 
{
	return __renderer_change_vmode(renderer_set_vmode_line);
}

int choose_vmode_points(Ihandle* ih, int state) 
{
	return __renderer_change_vmode(renderer_set_vmode_point);
}

static Ihandle * create_render_vmode_frame() {
	Ihandle *solid	= IupToggle ("Solid", "");
	IupSetCallback(solid, "ACTION",(Icallback)choose_vmode_solid);
  	
	Ihandle *wireframe = IupToggle ("Wireframe", "");
	IupSetCallback(wireframe, "ACTION",(Icallback)choose_vmode_wireframe);
	
	Ihandle *points	= IupToggle ("Points", "");
	IupSetCallback(points, "ACTION",(Icallback)choose_vmode_points);
	
	Ihandle *container = IupVbox( solid, wireframe, points, NULL );
	
	Ihandle * vmodes = IupRadio( container );

	Ihandle *frame = IupFrame(vmodes);
	IupSetAttribute(frame, "TITLE", "View Modes");
	return frame;
}

static Ihandle * create_render_zoom_options_frame()
{
	Ihandle * zoomfactor = IupVal(IUP_HORIZONTAL);
	render_context_t * rctx = (render_context_t *)IupGetGlobal("RCTX");
	vec3_t * from = &rctx->from;
	float length = vec3_length(from);
	IupSetFloat(zoomfactor, "MIN", 0.001f);
	IupSetFloat(zoomfactor, "MAX", length*8);
	IupSetFloat(zoomfactor, "VALUE", length);
	IupSetFloat(NULL, "ZOOM", length);
	IupSetCallback(zoomfactor, "VALUECHANGED_CB",(Icallback)render_view_zoom_factor_changed);
	
 	Ihandle * container = IupHbox(IupLabel("Zoom:"), zoomfactor, NULL);
	IupSetAttribute(container, "EXPAND", "HORIZONTAL");
	IupSetAttribute(container, "EXPANDCHILDREN", "YES");
	
	Ihandle *frame = IupFrame(container);
	IupSetAttribute(frame, "TITLE", "Zoom Options");
	return frame;
}

typedef mat3_t * (* MATCREATE_FN)( mat3_t * dest, const float rot);

static int render_view_rot(float rot, MATCREATE_FN matcreate  )
{
	mat3_t rotmat;
	render_context_t * rctx = (render_context_t *)IupGetGlobal("RCTX");
	vec3_t * from = &rctx->from;
	(* matcreate)(&rotmat, rot);
	mat_vec_mul_3(&rotmat, from);
	
	render_scene_again_and_refresh_canvas();

	return IUP_DEFAULT;
}

static int render_view_factor_changed(Ihandle * ih)
{	
	float lr = IupGetFloat(ih, "ROT");
	float cr = IupGetFloat(ih, "VALUE");
	IupSetFloat(ih, "ROT", cr);
	
	MATCREATE_FN matcreate = (MATCREATE_FN)IupGetAttribute(ih, "MATCREATE");

	render_view_rot(cr-lr, matcreate);
	return IUP_DEFAULT;
}

static Ihandle* __renderer_create_rot_val(const char* label, MATCREATE_FN matcreate)
{
	Ihandle * new_rot_val = IupVal(IUP_HORIZONTAL);
	IupSetAttributes(new_rot_val, "MIN=-180, MAX=180, VALUE=0, ROT=0");
	IupSetAttribute(new_rot_val, "MATCREATE", (void*)matcreate);

	IupSetCallback(new_rot_val, "VALUECHANGED_CB",(Icallback)render_view_factor_changed);
	
	Ihandle * rotx_container = IupHbox(IupLabel(label), new_rot_val, NULL);

	return rotx_container;
}

static Ihandle * create_render_rotation_options_frame()
{

	/* rotation slider for different axis */
	Ihandle* rotx_container = __renderer_create_rot_val("x-Axis: ", create_rot_x_mat_dest);
	Ihandle* roty_container = __renderer_create_rot_val("y-Axis: ", create_rot_y_mat_dest);
	Ihandle* rotz_container = __renderer_create_rot_val("z-Axis: ", create_rot_z_mat_dest);

	Ihandle *frame = IupFrame(IupVbox(rotx_container, roty_container, rotz_container, NULL));
	IupSetAttribute(frame, "TITLE", "Rotation Options");
	return frame;
}

static void __gbox_set_default_attrs(Ihandle *gbox) {
	IupSetAttribute(gbox, "ALIGNMENTLIN", "ACENTER");
	IupSetAttribute(gbox, "MARGIN", "10x10");
	IupSetAttribute(gbox, "GAPLIN", "5");
	IupSetAttribute(gbox, "GAPCOL", "5");
	IupSetAttribute(gbox, "NORMALIZESIZE", "BOTH");
	IupSetAttribute(gbox, "NORMALIZESIZE", "HORIZONTAL");
	IupSetAttribute(gbox, "EXPANDCHILDREN", "YES");
}

static Ihandle* __renderer_empty_label()
{
	return IupSetAttributes(IupLabel("-"), "");
}

Ihandle* create_render_debug_cam_vecs() {
	
	Ihandle *handles[] = {
		IupSetAttributes(IupLabel(""), ""), 
		IupSetAttributes(IupLabel("x"), "FONTSTYLE=Bold"), 
		IupSetAttributes(IupLabel("y"), "FONTSTYLE=Bold"), 
		IupSetAttributes(IupLabel("z"), "FONTSTYLE=Bold"),
		
		IupSetAttributes(IupLabel("from"), "FONTSTYLE=Bold"), 
		/*[5]*/__renderer_empty_label(), 
		/*[6]*/__renderer_empty_label(), 
		/*[7]*/__renderer_empty_label(), 
		
		IupSetAttributes(IupLabel("to"), "FONTSTYLE=Bold"),
		/*[9]*/__renderer_empty_label(), 
		/*[10]*/__renderer_empty_label(), 
		/*[11]*/__renderer_empty_label(), 
		
		IupSetAttributes(IupLabel("forward"), "FONTSTYLE=Bold"), 
		/*[13]*/__renderer_empty_label(), 
		/*[14]*/__renderer_empty_label(), 
		/*[15]*/__renderer_empty_label(), 

		IupSetAttributes(IupLabel("left"), "FONTSTYLE=Bold"), 
		/*[17]*/__renderer_empty_label(), 
		/*[18]*/__renderer_empty_label(), 
		/*[19]*/__renderer_empty_label(), 


		IupSetAttributes(IupLabel("up"), "FONTSTYLE=Bold"),
		/*[21]*/IupSetAttributes(IupLabel("-"), "HANDLENAME=up_x"), 
		/*[22]*/IupSetAttributes(IupLabel("-"), "HANDLENAME=up_y"), 
		/*[23]*/IupSetAttributes(IupLabel("-"), "HANDLENAME=up_z"),
		NULL
	};

	render_context_t * rctx = (render_context_t *)IupGetGlobal("RCTX");
	rctx->cam_from[0] = handles[5]; rctx->cam_from[1] = handles[6]; rctx->cam_from[2] = handles[7];
	rctx->cam_to[0] = handles[9]; rctx->cam_to[1] = handles[10]; rctx->cam_to[2] = handles[11];
	rctx->cam_forward[0] = handles[13]; rctx->cam_forward[1] = handles[14]; rctx->cam_forward[2] = handles[15];
	rctx->cam_left[0] = handles[17]; rctx->cam_left[1] = handles[18]; rctx->cam_left[2] = handles[19];
	rctx->cam_up[0] = handles[21]; rctx->cam_up[1] = handles[22]; rctx->cam_up[2] = handles[23];

	Ihandle *frame = IupGridBoxv( &handles[0] );
	
	IupSetAttribute(frame, "NUMDIV", "4");
	IupSetAttribute(frame, "EXPANDCHILDREN", "YES");
	__gbox_set_default_attrs(frame);

	return IupSetAttributes(IupFrame(frame), "TITLE=Camera-Vectors, EXPANDCHILDREN=YES");
}

Ihandle *create_render_debug_cam_mat4(const char* title, Ihandle **save_handles) {
	Ihandle *handles[] = {
		IupSetAttributes(IupLabel(""), ""), 
		IupSetAttributes(IupLabel("1"), "FONTSTYLE=Bold"), 
		IupSetAttributes(IupLabel("2"), "FONTSTYLE=Bold"), 
		IupSetAttributes(IupLabel("3"), "FONTSTYLE=Bold"),
		IupSetAttributes(IupLabel("4"), "FONTSTYLE=Bold"),
		
		IupSetAttributes(IupLabel("1"), "FONTSTYLE=Bold"), 
		/*[6]*/__renderer_empty_label(), 
		/*[7]*/__renderer_empty_label(), 
		/*[8]*/__renderer_empty_label(), 
		/*[9]*/__renderer_empty_label(), 
		
		IupSetAttributes(IupLabel("2"), "FONTSTYLE=Bold"),
		/*[11]*/__renderer_empty_label(), 
		/*[12]*/__renderer_empty_label(), 
		/*[13]*/__renderer_empty_label(), 
		/*[14]*/__renderer_empty_label(), 
		
		IupSetAttributes(IupLabel("3"), "FONTSTYLE=Bold"), 
		/*[16]*/__renderer_empty_label(), 
		/*[17]*/__renderer_empty_label(), 
		/*[18]*/__renderer_empty_label(), 
		/*[19]*/__renderer_empty_label(), 

		IupSetAttributes(IupLabel("4"), "FONTSTYLE=Bold"), 
		/*[21]*/__renderer_empty_label(), 
		/*[22]*/__renderer_empty_label(), 
		/*[23]*/__renderer_empty_label(), 
		/*[24]*/__renderer_empty_label(), 
		NULL
	};

	Ihandle ** hnd = save_handles;
	hnd[0] = handles[6]; hnd[1] = handles[7]; hnd[2] = handles[8]; hnd[3] = handles[9];
	hnd[4] = handles[11]; hnd[5] = handles[12]; hnd[6] = handles[13]; hnd[7] = handles[14];
	hnd[8] = handles[16]; hnd[9] = handles[17]; hnd[10] = handles[18]; hnd[11] = handles[19];
	hnd[12] = handles[21]; hnd[13] = handles[22]; hnd[14] = handles[23]; hnd[15] = handles[24];

	Ihandle *frame = IupGridBoxv( &handles[0] );

	IupSetAttribute(frame, "NUMDIV", "5");
	IupSetAttribute(frame, "EXPANDCHILDREN", "YES");
	__gbox_set_default_attrs(frame);
	Ihandle *mat_frame = IupFrame(frame);
	IupSetAttribute(mat_frame, "TITLE", title);
	return mat_frame;
}

Ihandle* create_render_debug_frustum_plane(const char *label, Ihandle** save_handles)
{

	Ihandle *handles[] = {
		IupSetAttributes(IupLabel(""), ""), 
		IupSetAttributes(IupLabel("x"), "FONTSTYLE=Bold"), 
		IupSetAttributes(IupLabel("y"), "FONTSTYLE=Bold"), 
		IupSetAttributes(IupLabel("z"), "FONTSTYLE=Bold"),
		
		IupSetAttributes(IupLabel("LT"), "FONTSTYLE=Bold"), 
		/*[5]*/__renderer_empty_label(), 
		/*[6]*/__renderer_empty_label(), 
		/*[7]*/__renderer_empty_label(), 
		
		IupSetAttributes(IupLabel("RT"), "FONTSTYLE=Bold"),
		/*[9]*/__renderer_empty_label(), 
		/*[10]*/__renderer_empty_label(), 
		/*[11]*/__renderer_empty_label(), 
		
		IupSetAttributes(IupLabel("LB"), "FONTSTYLE=Bold"), 
		/*[13]*/__renderer_empty_label(), 
		/*[14]*/__renderer_empty_label(), 
		/*[15]*/__renderer_empty_label(), 

		IupSetAttributes(IupLabel("RB"), "FONTSTYLE=Bold"), 
		/*[17]*/__renderer_empty_label(), 
		/*[18]*/__renderer_empty_label(), 
		/*[19]*/__renderer_empty_label(), 


		IupSetAttributes(IupLabel("NORMAL"), "FONTSTYLE=Bold"),
		/*[21]*/__renderer_empty_label(), 
		/*[22]*/__renderer_empty_label(), 
		/*[23]*/__renderer_empty_label(),
		NULL
	};

	Ihandle ** hnd = save_handles;
	hnd[0] = handles[5]; hnd[1] = handles[6]; hnd[2] = handles[7];
	hnd[3] = handles[9]; hnd[4] = handles[10]; hnd[5] = handles[11];
	hnd[6] = handles[13]; hnd[7] = handles[14]; hnd[8] = handles[15];
	hnd[9] = handles[17]; hnd[10] = handles[18]; hnd[11] = handles[19];
	hnd[12] = handles[21]; hnd[13] = handles[22]; hnd[14] = handles[23];


	Ihandle *container = IupGridBoxv( &handles[0] );

	IupSetAttribute(container, "NUMDIV", "4");
	__gbox_set_default_attrs(container);
	Ihandle *frame = IupFrame(container);
	IupSetAttribute(frame, "TITLE", label);

	return frame;
}

Ihandle* create_render_debug_frustum()
{
	render_context_t * rctx = (render_context_t *)IupGetGlobal("RCTX");

	Ihandle* frustum_container = IupVbox(
		create_render_debug_frustum_plane("Near", &rctx->frustum_near[0]),
		create_render_debug_frustum_plane("Far", &rctx->frustum_far[0]),
		create_render_debug_frustum_plane("Left", &rctx->frustum_left[0]),
		create_render_debug_frustum_plane("Right", &rctx->frustum_right[0]),
		create_render_debug_frustum_plane("Top", &rctx->frustum_top[0]),
		create_render_debug_frustum_plane("Bottom", &rctx->frustum_bottom[0]),
		NULL
	);

	Ihandle *frame = IupFrame(frustum_container);
	IupSetAttribute(frame, "TITLE", "View Frustum");

	return frame;
}

Ihandle* create_render_debug_view_frame() {

	render_context_t * rctx = (render_context_t *)IupGetGlobal("RCTX");

	Ihandle *container = IupGridBox(
		create_render_debug_cam_vecs(),
		create_render_debug_cam_mat4("View Matrix", &rctx->mat4_view[0]),
		create_render_debug_cam_mat4("Projection Matrix", &rctx->mat4_proj[0]),
		create_render_debug_cam_mat4("Transformation Matrix", &rctx->mat4_trans[0]),
		NULL
	);

	IupSetAttribute(container, "NUMDIV", "2");
	__gbox_set_default_attrs(container);
	//cam vector matrix from, to, forward left up  : label x y z

	Ihandle* debug_frustum = create_render_debug_frustum();

	Ihandle* frame = IupHbox(debug_frustum, container, NULL);
	//view matrix
	//projection matrix
	return frame;
}

Ihandle * create_render_options()
{
	render_context_t * rctx = (render_context_t *)IupGetGlobal("RCTX");

	Ihandle *frame = IupVbox(
		create_render_zoom_options_frame(),
		create_render_vmode_frame(),
		create_render_rotation_options_frame(),
		NULL
	);

	//IupSetAttribute(frame, "NUMDIV", "3");
	//__gbox_set_default_attrs(frame);

	return frame;

}

static Ihandle * create__render_frame()
{
	Ihandle * canvas = create__render_canvas();
	Ihandle * liveview = create_live_view_toggle();
	Ihandle * render_options = create_render_options();
	Ihandle * container = IupVbox(canvas, liveview, NULL);

	Ihandle * render_frame = IupFrame(container);
	IupSetAttribute(render_frame, "TITLE", "Render Window");
	IupSetCallback(render_frame, "UNMAP_CB", (Icallback)unmap_frame);

	Ihandle * frame = IupVbox(render_frame, render_options, NULL);
	return frame;
}

Ihandle* create_and_show_dialog()
{
	render_context_t *render_ctx = create_test_renderer();
	
	Ihandle *render_frame = create__render_frame();
	Ihandle *render_debug_view_frame = create_render_debug_view_frame();
	Ihandle *move_opt_box = IupGridBox(render_debug_view_frame, NULL);
	//__gbox_set_default_attrs(move_opt_box);
	Ihandle *render_0 = IupHbox( render_frame, move_opt_box, NULL);
	

	Ihandle *maindlg = IupVbox(render_0, NULL);

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
	plugin->data = malloc(sizeof(render_ctx_t));
	return plugin;
}

