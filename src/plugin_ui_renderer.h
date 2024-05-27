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
} RenderCtx;

Plugin * render_plugin(Plugin * plugin);

#endif