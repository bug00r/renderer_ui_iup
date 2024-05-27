#ifndef PLUGIN_UI_TEXTURING_H
#define PLUGIN_UI_TEXTURING_H

#include <stdlib.h>
#include <stdio.h>
#include <iup.h>
#include <cd.h>
#include <cdiup.h>
#include <wd.h>

#include "texture.h"
#include "plugin.h"

typedef struct {
	Ihandle * frame;	//algorithm frame
	void * context;		//specific algorithm data
} TextureAlgorithm;


//### OLD Interface ###

typedef struct {
	const char *name;
	float (*middlefunc)(float x1, float x2, float x3, float x4);
} middle_func4_t;

typedef struct {
	const char *name;
	float (*middlefunc)(float x1, float x2);
} middle_func2_t;

typedef struct {
	const char *name;
	void (*filter)(Texture * texture, unsigned int pxrange, float factor);
} texture_filter_t;

typedef struct {
	const char *name;
	void (*manipulation)(Texture * texture);
} texture_manipulation_t;

typedef struct {
	const char *name;
	float complex c;
} julia_c_t;

typedef struct {
	const char *name;
	float complex (*func)(const float complex *cp, const float complex *c);
} julia_polyfunc_t;

typedef struct {
	const char *name;
	void (*func)(void);
} tex_algorithm_t;

typedef struct _texture_ctx_ {
	Ihandle *frame;
} texture_ctx_t;

typedef struct {
	Noise * noise;
	Texture * texture;
	tex_algorithm_t * algorithm;
	texture_manipulation_t *tex_manip;
	texture_filter_t *tex_filter;
	middle_func4_t *funcs4;
	middle_func2_t *funcs2;
} RenderTextureContext;

Plugin * texture_plugin(Plugin * plugin);

#endif