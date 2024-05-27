#ifndef PLUGIN_UI_GFX_ALGO_TEST_H
#define PLUGIN_UI_GFX_ALGO_TEST_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <iup.h>
#include <cd.h>
#include <cdiup.h>
#include <im_util.h>
#include <wd.h>

#include "defs.h"
#include "dl_list.h"
#include "xml_utils.h"
#include "resource.h"
#include "iup_xml_builder.h"
#include "defs.h"
#include "geometry.h"
#include "r_font.h"
#include "font_provider_default.h"

#include "plugin.h"

//rename all "proto" with your own plugin name

typedef void (*DRAW_TRIGGER) (Ihandle *handle);

typedef struct _gfx_algo_test_ctx_ {
	Ihandle *frame;
	Ihandle *cur_algo;
} GfxAlgoTestCtx;

Plugin * gfx_algo_test_plugin(Plugin * plugin);

#endif