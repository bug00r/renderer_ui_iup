#ifndef PLUGIN_UI_RENDERER_H
#define PLUGIN_UI_RENDERER_H

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

#include "font_provider_default.h"
#include "r_font.h"

#include "plugin.h"

typedef struct _render_ctx_ {
	Ihandle *frame;
} render_ctx_t;

plugin_t * render_plugin(plugin_t * plugin);

#endif