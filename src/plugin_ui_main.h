#ifndef PLUGIN_UI_MAIN_H
#define PLUGIN_UI_MAIN_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <iup.h>
#include <cd.h>
#include <cdiup.h>
#include <im_util.h>
#include <wd.h>

#include "scene_builder.h"
#include "renderer.h"
#include "texture.h"

#include "plugin.h"
//plugins
#include "plugin_ui_renderer.h"
#include "plugin_ui_texturing.h"

typedef struct _main_ctx_ {
	Ihandle *frame;
	unsigned int cntplugins;
	plugin_t * plugins;
} main_ctx_t;

plugin_t * main_plugin(plugin_t * plugin);

#endif