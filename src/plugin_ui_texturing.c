#include "plugin_ui_texturing.h"

#define CANVAS_SIZE 513
void texture_manipulation_brigthness_contrast(Texture * texture)
{
	ArrayIterator * it = array_iterator_new(texture->buffer);
	ColorRGB * curcolor;
	ColorRGB temp;
	
	float contrast_factor = crgb_contrast_factor_255(seedrndlh(75.f, 80.f));
	float brightness = seedrndlh(-35.f, -25.f);
	while(array_iterator_has_next(it))
	{
		curcolor = (ColorRGB *)array_iterator_next(it);
		crgb_brightness_255_dest(&temp, curcolor, brightness);
		crgb_contrast_255_dest(curcolor, &temp, contrast_factor);
	}
	array_iterator_free(it);
}


static void convertTexture_iupCanvas(RenderTextureContext * rtctx, cdCanvas * canvas) 
{
	Texture * texture = rtctx->texture;

	ColorRGB * cref;
	ArrayError array_res;
	
	for (unsigned int h = 0; h < texture->height; ++h){
	  unsigned int ch = ((texture->height-1) - h);
	  for (unsigned int w = 0; w < texture->width; ++w){
			array_res = crgb_array2D_get_ref(texture->buffer, w, ch, &cref);
			if(array_res==ARRAY_ERR_OK){
				cdCanvasPixel(canvas, w, ch, ((unsigned char) cref->r << 16) |
											 ((unsigned char) cref->g << 8) |
											 ((unsigned char) cref->b));
			}
	    }
	}
}

#if 0
//########### Algorithms #########
#endif

static void process_texture_ds() {
	printf("process diamond square\n");
	RenderTextureContext * rtctx = (RenderTextureContext *)IupGetGlobal("RTCTX");
	int w = CANVAS_SIZE;
	int h = CANVAS_SIZE;
	Noise * noise = rtctx->noise;
	DiamondSquare ds_param;
	float maxreduction = IupGetFloat(IupGetHandle("seed"), "VALUE");
//	printf("seed: %f", maxreduction);
	float reduction = IupGetFloat(IupGetHandle("reduction"), "VALUE");//maxreduction/((w-1)*(w-1));
//	printf("reduction: %f", reduction);
	noise->min = FLT_MAX;
	noise->max = FLT_MIN;
	ds_param.noise = noise;
	ds_param.length = w-1;
	ds_param.startseed = IupGetFloat(IupGetHandle("startseed"), "VALUE");
//	printf("startseed: %f", ds_param.startseed);
	ds_param.seed = maxreduction;
	ds_param.reduction = reduction;
	ds_param.middlefunc = middle_arithmetic;
	ds_param.seedreducefunc = seed_reduction_mul;
	Ihandle * m_list = IupGetHandle("texture_middle4_list");
	int middle = IupGetInt(m_list, "VALUE");
	if ( --middle >= 0 ) {
		MiddleFunc4 * middlef = &rtctx->funcs4[middle];
		if (middlef->name && middlef->middlefunc) {
			ds_param.middlefunc = middlef->middlefunc;
		}
	}
	create_diamond_square(&ds_param);
} 

static void process_texture_md() {
	printf("process midpoint displacement\n");
	RenderTextureContext * rtctx = (RenderTextureContext *)IupGetGlobal("RTCTX");
	int w = CANVAS_SIZE;
	int h = CANVAS_SIZE;
	Noise * noise = rtctx->noise;
	MidpointDisplacement md_param;
	float maxreduction = IupGetFloat(IupGetHandle("seed"), "VALUE");
//	printf("seed: %f", maxreduction);
	float reduction = IupGetFloat(IupGetHandle("reduction"), "VALUE");//maxreduction/((w-1)*(w-1))
//	printf("reduction: %f", reduction);
	noise->min = FLT_MAX;
	noise->max = FLT_MIN;
	md_param.noise = noise;
	md_param.length = w-1;
	md_param.startseed = IupGetFloat(IupGetHandle("startseed"), "VALUE");
//	printf("startseed: %f", md_param.startseed);
	md_param.seed = maxreduction;
	md_param.reduction = reduction;
	md_param.middlefunc = middle_arithmetic;
	md_param.middlefunc2 = middle_arithmetic2;
	md_param.seedreducefunc = seed_reduction_mul;
	Ihandle * m_list = IupGetHandle("texture_middle4_list");
	int middle = IupGetInt(m_list, "VALUE");
	if ( --middle >= 0 ) {
		MiddleFunc4 * middlef = &rtctx->funcs4[middle];
		if (middlef->name && middlef->middlefunc) {
			md_param.middlefunc = middlef->middlefunc;
		}
	}

	m_list = IupGetHandle("texture_middle2_list");
	middle = IupGetInt(m_list, "VALUE");
	if ( --middle >= 0 ) {
		MiddleFunc2 * middlef2 = &rtctx->funcs2[middle];
		if (middlef2->name && middlef2->middlefunc) {
			md_param.middlefunc2 = middlef2->middlefunc;
		}
	}
	
	create_midpoint_displacement(&md_param);
} 

#if 0
//########### EO Algorithms #########
#endif

void create_texture()
{
	Ihandle * tex_algo_list = IupGetHandle("texture_algo_list");
	int algorithm = IupGetInt(tex_algo_list, "VALUE");
	
	if ( --algorithm >= 0 ) {
		RenderTextureContext * rtctx = (RenderTextureContext *)IupGetGlobal("RTCTX");
		TexAlgorithm * algo = &rtctx->algorithm[algorithm];
		if (algo->name && algo->func) {
			algo->func();
			Noise * noise = rtctx->noise;
			Texture * texture = rtctx->texture;
			Noiseo_texture( noise, texture);
			
			//texture filtering
			Ihandle * m_list = IupGetHandle("texture_filter_list");
			int item = IupGetInt(m_list, "VALUE");
			if ( --item >= 0 ) {
				TextureFilter * filter = &rtctx->tex_filter[item];
				if (filter->name && filter->filter) {
					unsigned int mediancross = IupGetInt(IupGetHandle("mediancross"), "VALUE");
					float arithfactor = IupGetFloat(IupGetHandle("arithfactor"), "VALUE");
					//printf("mc: %i af: %f\n", mediancross, arithfactor);
					filter->filter(texture, mediancross, arithfactor);
				}
			}
			//texture manipulation
			m_list = IupGetHandle("texture_manipulation_list");
			item = IupGetInt(m_list, "VALUE");
			if ( --item >= 0 ) {
				TextureManipulation * manip = &rtctx->tex_manip[item];
				if (manip->name && manip->manipulation) {
					manip->manipulation(texture);
				}
			}
		} else {
			printf("- no selected - algorithm :(\n");
		}
	} else {
		printf("no algorithm selected :(\n");
	}
}

static void render_texture_canvas(cdCanvas * _canvas)
{
	cdCanvas *canvas = _canvas;
	RenderTextureContext * rtctx = (RenderTextureContext *)IupGetGlobal("RTCTX");
	
	if ( canvas != NULL && rtctx != NULL)
	{
		convertTexture_iupCanvas(rtctx, canvas);
	} 
	else 
	{
		printf("something is missing: canvas: %i ,rtctx: %i\n",
							canvas != NULL, rtctx != NULL);
	}
}

static void render_texture_and_refresh_canvas()
{
	create_texture();
	cdCanvas *canvas = (cdCanvas*)IupGetAttribute(NULL, "TEXTURE_CD_CANVAS_DBUFFER");
	cdCanvasActivate(canvas);
	render_texture_canvas(canvas);
	cdCanvasFlush(canvas);
}

static int resize_texture_cb(Ihandle *ih, int canvas_w, int canvas_h)
{
	cdCanvas *canvas = (cdCanvas*)IupGetAttribute(ih, "_CD_CANVAS");
	cdCanvasActivate(canvas);
	return IUP_DEFAULT;
}

static int action_texture(Ihandle *ih, float fposx, float fposy)
{	
	cdCanvas *canvas = (cdCanvas*)IupGetAttribute(ih, "_CD_CANVAS_DBUFFER");
	cdCanvasActivate(canvas);

	render_texture_canvas(canvas);
	
	cdCanvasFlush(canvas); 
	return IUP_DEFAULT;
}

static void map_texture_canvas(Ihandle * ih)
{
	cdCanvas * cd_canvas = cdCreateCanvas(CD_IUPDBUFFERRGB, ih); 
	IupSetAttribute(NULL, "TEXTURE_CD_CANVAS_DBUFFER", (void*)cd_canvas);
}

static Ihandle * create_texture_canvas()
{
	Ihandle *canvas = IupCanvas(NULL);
	IupSetAttribute(canvas, "RASTERSIZE", "513x513");
	IupSetAttribute(canvas, "EXPAND", "NO");
	
	IupSetCallback(canvas, "RESIZE_CB",  (Icallback)resize_texture_cb);
	IupSetCallback(canvas, "ACTION",  (Icallback)action_texture);
	IupSetCallback(canvas, "MAP_CB",  (Icallback)map_texture_canvas);
	
	return canvas;
}

static void unmap_frame(Ihandle * ih)
{
	cdCanvas *canvas = (cdCanvas*)IupGetAttribute(ih, "_CD_CANVAS");
	cdKillCanvas(canvas);
}

static int create_texture_cb(Ihandle * ih)
{
	render_texture_and_refresh_canvas();
	return IUP_DEFAULT;
}

static int on_algorithm_changed_cb(Ihandle *ih, char *text, int item, int state) {
	//printf("change state %i to element %i\n", state, item);
	Ihandle * m_list = IupGetHandle("texture_middle2_list");
	if (state) {
		if ( item == 2 ) {
			IupSetAttribute(m_list, "ACTIVE", "yes");
		} else {
			IupSetAttribute(m_list, "ACTIVE", "NO");
		}
	}
	IupRefresh(m_list);
	return IUP_DEFAULT;
}

static Ihandle * create_algo_frame() {
	Ihandle * algolist = IupList(NULL);
	IupSetAttribute(algolist, "DROPDOWN", "YES");
	IupSetCallback(algolist, "ACTION",  (Icallback)on_algorithm_changed_cb);
	IupSetHandle("texture_algo_list", algolist);
	/**
			md_param.startseed = 1.f;
			md_param.seed = 2.;
			md_param.reduction = reduction; //maxreduction((w-1)*(w-1));
			md_param.middlefunc = curfunc4->middlefunc;
			md_param.middlefunc2 = curfunc2->middlefunc;
	*/
	
	Ihandle * middle4list = IupList(NULL);
	IupSetAttribute(middle4list, "DROPDOWN", "YES");
	IupSetHandle("texture_middle4_list", middle4list);
	
	Ihandle * middle2list = IupList(NULL);
	IupSetAttribute(middle2list, "DROPDOWN", "YES");
	IupSetHandle("texture_middle2_list", middle2list);
	
	Ihandle * startseed = IupText(NULL);
	IupSetAttribute(startseed, "MASK", IUP_MASK_FLOAT);
	IupSetAttribute(startseed, "VALUEMASKED", "1.0");
	IupSetHandle("startseed", startseed);
	
	Ihandle * seed = IupText(NULL);
	IupSetAttribute(seed, "MASK", IUP_MASK_FLOAT);
	IupSetAttribute(seed, "VALUEMASKED", "1.0");
	IupSetHandle("seed", seed);
	
	Ihandle * reduction = IupText(NULL);
	IupSetAttribute(reduction, "MASK", IUP_MASK_FLOAT);
	IupSetAttribute(reduction, "VALUEMASKED", "0.5");
	IupSetHandle("reduction", reduction);
	
	Ihandle * gbox = IupGridBox( 
						IupLabel("Algorithm:"), algolist	,
						IupLabel("Average1:"), 	middle4list	,
						IupLabel("Average2:"), 	middle2list	,
						IupLabel("start-seed:"),startseed	,
						IupLabel("seed:"), 		seed		,
						IupLabel("reduction:"), reduction	,
						NULL);
	//IupSetAttribute(gbox, "EXPANDCHILDREN", "HORIZONTAL");
	IupSetAttribute(gbox, "NUMDIV", "2");
	IupSetAttribute(gbox, "MARGIN", "10x10");
	IupSetAttribute(gbox, "GAPLIN", "5");
	IupSetAttribute(gbox, "GAPCOL", "5");
	IupSetAttribute(gbox, "CGAPLIN", "10");
	IupSetAttribute(gbox, "CGAPCOL", "10");
	Ihandle * frame = IupFrame(gbox);
	IupSetAttribute(frame, "TITLE", "Noise Algorithm");
	return frame;
}

static Ihandle * create_filter_frame() {
	Ihandle * filterlist = IupList(NULL);
	IupSetAttribute(filterlist, "DROPDOWN", "YES");
	IupSetHandle("texture_filter_list", filterlist);
	
	Ihandle * mediancross = IupText(NULL);
	IupSetAttribute(mediancross, "MASK", IUP_MASK_UINT);
	IupSetAttribute(mediancross, "VALUEMASKED", "2");
	IupSetHandle("mediancross", mediancross);
	
	Ihandle * arithfactor = IupText(NULL);
	IupSetAttribute(arithfactor, "MASK", IUP_MASK_FLOAT);
	IupSetAttribute(arithfactor, "VALUEMASKED", "1.0");
	IupSetHandle("arithfactor", arithfactor);
	
	Ihandle * gbox = IupGridBox( 
						IupLabel("Type:"), filterlist	,
						IupLabel("px-size:"), mediancross	,
						IupLabel("scale:"), arithfactor	,
						NULL);
	//IupSetAttribute(gbox, "EXPANDCHILDREN", "HORIZONTAL");
	IupSetAttribute(gbox, "NUMDIV", "2");
	IupSetAttribute(gbox, "MARGIN", "10x10");
	IupSetAttribute(gbox, "GAPLIN", "5");
	IupSetAttribute(gbox, "GAPCOL", "5");
	IupSetAttribute(gbox, "CGAPLIN", "10");
	IupSetAttribute(gbox, "CGAPCOL", "10");
	Ihandle * frame = IupFrame(gbox);
	IupSetAttribute(frame, "TITLE", "Filter");
	IupSetAttribute(frame, "HOMOGENEOUS", "YES");
	IupSetAttribute(frame, "EXPANDCHILDREN", "YES");
	return frame;
}

static Ihandle * create_manipulation_frame() {
	Ihandle * manipulationlist = IupList(NULL);
	IupSetAttribute(manipulationlist, "DROPDOWN", "YES");
	IupSetHandle("texture_manipulation_list", manipulationlist);
	
	Ihandle * frame = IupFrame(IupVbox(
							IupHbox(IupLabel("Type:"), manipulationlist, NULL),
							NULL));
	IupSetAttribute(frame, "TITLE", "Manipulation");
	return frame;
}

static Ihandle * create_texture_algorithm_frame() {
		
	Ihandle * frame = IupVbox(
						   create_algo_frame(),
						   create_filter_frame(),
						   create_manipulation_frame(),						   
						   NULL
						);					
	return frame;
}

static Ihandle * create_texture_frame()
{
	Ihandle * canvas = create_texture_canvas();
	Ihandle * btn = IupButton("create new", NULL);
	IupSetCallback(btn, "ACTION", (Icallback)create_texture_cb);
	
	Ihandle * algorithm_frame = create_texture_algorithm_frame();
	
	Ihandle * container = IupVbox(canvas, btn, algorithm_frame, NULL);
	Ihandle *frame = IupFrame(container);
	IupSetAttribute(frame, "TITLE", "Texture Window");
	IupSetCallback(frame, "UNMAP_CB", (Icallback)unmap_frame);
	return frame;
}

Ihandle * create_texturing_ui()
{
	Ihandle * tex_frame = create_texture_frame();
	Ihandle * maindlg = IupScrollBox(
						tex_frame);
	return maindlg;
}

//###########

static void _texture_init_(void * data) {
	printf("texture init\n");
	#if 0
		/** init context here
			All needed things:
		  */
	#endif
	TextureCtx * mctx = (TextureCtx *)data;
	mctx->frame=NULL;
	
	RenderTextureContext * rtctx = malloc(sizeof(RenderTextureContext));
	int w = CANVAS_SIZE;
	int h = CANVAS_SIZE;
	rtctx->texture = texture_new(w,h);
	rtctx->noise = noise_new(w, h);
	
	int size = 4;
	rtctx->algorithm = malloc(size * sizeof(TexAlgorithm));
	rtctx->algorithm[--size] = (TexAlgorithm){NULL,NULL};
	rtctx->algorithm[--size] = (TexAlgorithm){"Diamond Square", process_texture_ds};
	rtctx->algorithm[--size] = (TexAlgorithm){"Midpoint Displacement", process_texture_md};
	rtctx->algorithm[--size] = (TexAlgorithm){"- no selection -",NULL};
	
	size = 3;
	rtctx->tex_manip = malloc(size * sizeof(TextureManipulation));
	rtctx->tex_manip[--size] = (TextureManipulation){NULL,NULL};
	rtctx->tex_manip[--size] = (TextureManipulation){"Brightness and Contrast",texture_manipulation_brigthness_contrast};
	rtctx->tex_manip[--size] = (TextureManipulation){"- no selection -",NULL};

	size = 8;
	rtctx->tex_filter = malloc(size * sizeof(TextureFilter));
	rtctx->tex_filter[--size] = (TextureFilter){NULL,NULL};
	rtctx->tex_filter[--size] = (TextureFilter){"filter_gauss",filter_gaussian};
	rtctx->tex_filter[--size] = (TextureFilter){"filter_middle_arith",filter_middle_arith};
	rtctx->tex_filter[--size] = (TextureFilter){"filter_middle_median_box",filter_middle_median_box};
	rtctx->tex_filter[--size] = (TextureFilter){"filter_middle_median_cross",filter_middle_median_cross};
	rtctx->tex_filter[--size] = (TextureFilter){"filter_middle_median_diag_cross",filter_middle_median_diag_cross};
	rtctx->tex_filter[--size] = (TextureFilter){"filter_middle_cubic_box",filter_middle_cubic_box};
	rtctx->tex_filter[--size] = (TextureFilter){"- no selection -",NULL};
	
	size = 10;
	rtctx->funcs4 = malloc(size * sizeof(MiddleFunc4));
	rtctx->funcs4[--size] = (MiddleFunc4){NULL,NULL};
	rtctx->funcs4[--size] = (MiddleFunc4){"middle_hoelder",middle_hoelder};
	rtctx->funcs4[--size] = (MiddleFunc4){"middle_geometric",middle_geometric};
	rtctx->funcs4[--size] = (MiddleFunc4){"middle_cubic",middle_cubic};
	rtctx->funcs4[--size] = (MiddleFunc4){"middle_quad",middle_quad};
	rtctx->funcs4[--size] = (MiddleFunc4){"middle_median",middle_median};
	rtctx->funcs4[--size] = (MiddleFunc4){"middle_quantil",middle_quantil};
	rtctx->funcs4[--size] = (MiddleFunc4){"middle_harmonic",middle_harmonic};
	rtctx->funcs4[--size] = (MiddleFunc4){"middle_arithmetic",middle_arithmetic};
    rtctx->funcs4[--size] = (MiddleFunc4){"- no selection -",NULL};
	
	size = 8;
	rtctx->funcs2 = malloc(size * sizeof(MiddleFunc2));
	rtctx->funcs2[--size] = (MiddleFunc2){NULL,NULL};
	rtctx->funcs2[--size] = (MiddleFunc2){"middle_hoelder",middle_hoelder2};
	rtctx->funcs2[--size] = (MiddleFunc2){"middle_geometric",middle_geometric2};
	rtctx->funcs2[--size] = (MiddleFunc2){"middle_cubic",middle_cubic2};
	rtctx->funcs2[--size] = (MiddleFunc2){"middle_quad",middle_quad2};
	rtctx->funcs2[--size] = (MiddleFunc2){"middle_harmonic",middle_harmonic2};
	rtctx->funcs2[--size] = (MiddleFunc2){"middle_arithmetic",middle_arithmetic2};
	rtctx->funcs2[--size] = (MiddleFunc2){"- no selection -",NULL};
	
	IupSetGlobal("RTCTX", (void*)rtctx); 
}

static void _texture_free_(void * data) {
	printf("texture free\n");
	RenderTextureContext * rtctx = (RenderTextureContext *)IupGetGlobal("RTCTX");
	if (rtctx) {
		texture_free(rtctx->texture);
		noise_free(rtctx->noise);
		free(rtctx->algorithm);
		free(rtctx->tex_manip);
		free(rtctx->tex_filter);
		free(rtctx->funcs4);
		free(rtctx->funcs2);
		free(rtctx);
	}
	TextureCtx * mctx = (TextureCtx *)data;
	free(mctx);
}

static const char * _texture_name_(void * data) {
	printf("texturer name\n");
	return "Texturing";
}

void * _texture_frame_(void * data) {
	printf("texture frame\n");
	TextureCtx * mctx = (TextureCtx *)data;
	Ihandle * frame = mctx->frame;
	if ( mctx->frame == NULL ) {
		printf("texture frame create new\n");
		frame = create_texturing_ui();
		mctx->frame = frame;
	}
	return frame;
}

void _texture_prepare_(void * data) {
	printf("handle texture prepare\n");
	RenderTextureContext * rtctx = (RenderTextureContext *)IupGetGlobal("RTCTX");
	Ihandle * tex_algo_list = IupGetHandle("texture_algo_list");
	for(unsigned int i = 0; rtctx->algorithm[i].name ; i++) {
		IupSetAttribute(tex_algo_list, "APPENDITEM", rtctx->algorithm[i].name);
	}	
	IupSetAttribute(tex_algo_list, "VALUE", "2");
	IupRefresh(tex_algo_list);
	
	tex_algo_list = IupGetHandle("texture_middle4_list");
	for(unsigned int i = 0; rtctx->funcs4[i].name ; i++) {
		IupSetAttribute(tex_algo_list, "APPENDITEM", rtctx->funcs4[i].name);
	}
	IupSetAttribute(tex_algo_list, "VALUE", "2");
	IupRefresh(tex_algo_list);
	
	tex_algo_list = IupGetHandle("texture_middle2_list");
	for(unsigned int i = 0; rtctx->funcs2[i].name ; i++) {
		IupSetAttribute(tex_algo_list, "APPENDITEM", rtctx->funcs2[i].name);
	}
	IupSetAttribute(tex_algo_list, "VALUE", "2");
	IupRefresh(tex_algo_list);
	
	tex_algo_list = IupGetHandle("texture_filter_list");
	for(unsigned int i = 0; rtctx->tex_filter[i].name ; i++) {
		IupSetAttribute(tex_algo_list, "APPENDITEM", rtctx->tex_filter[i].name);
	}
	IupSetAttribute(tex_algo_list, "VALUE", "1");
	IupRefresh(tex_algo_list);
	
	tex_algo_list = IupGetHandle("texture_manipulation_list");
	for(unsigned int i = 0; rtctx->tex_manip[i].name ; i++) {
		IupSetAttribute(tex_algo_list, "APPENDITEM", rtctx->tex_manip[i].name);
	}
	IupSetAttribute(tex_algo_list, "VALUE", "1");
	IupRefresh(tex_algo_list);
	
}

void _texture_cleanup_(void * data) {
	printf("texture plugin cleanup\n");
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
Plugin * texture_plugin(Plugin * plugin) {
	plugin->name = _texture_name_;
	plugin->frame = _texture_frame_;
	plugin->init = _texture_init_;
	plugin->free = _texture_free_;
	plugin->prepare = _texture_prepare_;
	plugin->cleanup = _texture_cleanup_;
	plugin->data = malloc(sizeof(TextureCtx)); //here malloc
	return plugin;
}
