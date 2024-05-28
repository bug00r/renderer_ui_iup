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
} MiddleFunc4;

typedef struct {
	const char *name;
	float (*middlefunc)(float x1, float x2);
} MiddleFunc2;

typedef struct {
	const char *name;
	void (*filter)(Texture * texture, unsigned int pxrange, float factor);
} TextureFilter;

typedef struct {
	const char *name;
	void (*manipulation)(Texture * texture);
} TextureManipulation;

typedef struct {
	const char *name;
	float complex c;
} JuliaC;

typedef struct {
	const char *name;
	float complex (*func)(const float complex *cp, const float complex *c);
} JuliaPolyFunc;

typedef struct {
	const char *name;
	void (*func)(void);
} TexAlgorithm;

typedef struct _texture_ctx_ {
	Ihandle *frame;
} TextureCtx;

typedef struct {
	Noise * noise;
	Texture * texture;
	TexAlgorithm * algorithm;
	TextureManipulation *tex_manip;
	TextureFilter *tex_filter;
	MiddleFunc4 *funcs4;
	MiddleFunc2 *funcs2;
} RenderTextureContext;

Plugin * texture_plugin(Plugin * plugin);

#endif