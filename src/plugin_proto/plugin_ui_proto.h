#ifndef PLUGIN_UI_PROTO_H
#define PLUGIN_UI_PROTO_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <iup.h>
#include <cd.h>
#include <cdiup.h>
#include <im_util.h>
#include <wd.h>

#include "plugin.h"

//rename all "proto" with your own plugin name

typedef struct _proto_ctx_ {
	Ihandle *frame;
} ProtoCtx;

Plugin * proto_plugin(Plugin * plugin);

#endif