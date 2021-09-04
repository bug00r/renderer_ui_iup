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

#include "dl_list.h"
#include "xml_utils.h"
#include "resource.h"
#include "iup_xml_builder.h"
#include "defs.h"
#include "geometry.h"

#include "plugin.h"

//rename all "proto" with your own plugin name

typedef void (*DRAW_TRIGGER) (Ihandle *handle);

typedef struct _gfx_algo_test_ctx_ {
	Ihandle *frame;
	Ihandle *cur_algo;
} gfx_algo_test_ctx_t;

plugin_t * gfx_algo_test_plugin(plugin_t * plugin);

#endif