#ifndef PLUGIN_UI_RENDERER_CONTEXT_H
#define PLUGIN_UI_RENDERER_CONTEXT_H

#include <stdlib.h>
#include <stdio.h>
#include <iup.h>
#include <cd.h>
#include <cdiup.h>
#include <cddbuf.h>
#include <wd.h>

#include "scene_builder.h"
#include "renderer.h"
#include "texture.h"

typedef struct {
	renderer_t *renderer;
	scene_t *scene;
	vec3_t from, to;
	float l,r,t,b,f,n;
	cRGB_t  bgcolor;
} ui_render_context_t;

Ihandle*	create_renderer_context(void *render_context);

#endif