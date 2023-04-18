_CFLAGS:=$(CFLAGS)
CFLAGS+=$(_CFLAGS)
_LDFLAGS:=$(LDFLAGS)
LDFLAGS+=$(_LDFLAGS)

ARFLAGS?=rcs
PATHSEP?=/
BUILDROOT?=build

BUILDDIR?=$(BUILDROOT)$(PATHSEP)$(CC)
BUILDPATH?=$(BUILDDIR)$(PATHSEP)

ifndef PREFIX
	INSTALL_ROOT=$(BUILDPATH)
else
	INSTALL_ROOT=$(PREFIX)$(PATHSEP)
	ifeq ($(INSTALL_ROOT),/)
	INSTALL_ROOT=$(BUILDPATH)
	endif
endif

ifdef DEBUG
	CFLAGS+=-ggdb
	ifeq ($(DEBUG),)
	CFLAGS+=-Ddebug=1
	else 
	CFLAGS+=-Ddebug=$(DEBUG)
	endif
endif

ifeq ($(M32),1)
	CFLAGS+=-m32
	BIT_SUFFIX+=32
endif

CFLAGS=-std=c11 -O1

#-Wpedantic -pedantic-errors -Wall -Wextra -O1
#-ggdb  -mwindows
#-pg for profiling 

LDFLAGS+=-L/c/dev/lib
CFLAGS+=-I/c/dev/include -I.

_SRC_FILES=test_renderer_ui_iup app iup_app plugin plugin_ui_main plugin_ui_renderer plugin_ui_texturing plugin_ui_gfx_algo_test font_provider_default
TESTSRC=$(patsubst %,src/%,$(patsubst %,%.c,$(_SRC_FILES)))
TESTSRC+=$(patsubst %,src/%,$(patsubst %,%.h,$(_SRC_FILES)))

TESTBIN=$(BUILDPATH)test_renderer_ui_iup.exe

RENDERER_LIBS= r_font iup_xml_builder geometry renderer scene mesh shape texture noise fractals crgb_array farray array color statistics utilsmath mat vec dl_list utils
IUP_LIBS=cdcontextplus gdiplus im iupcd iup cd
THIRD_PARTY_LIBS=exslt xslt xml2 archive zstd lzma z lz4 bz2 freetype6 iconv pcre2-8
OS_LIBS=kernel32 user32 gdi32 winspool comdlg32 advapi32 shell32 uuid ole32 oleaut32 comctl32 ws2_32

CFLAGS+=-DPCRE2_STATIC -DIN_LIBXML

LDFLAGS+=-L$(BUILDDIR)
LDFLAGS+=$(patsubst %,-l%,$(RENDERER_LIBS) $(IUP_LIBS) $(THIRD_PARTY_LIBS) $(OS_LIBS))

RES=zip_resource
RES_O=$(RES).o
RES_O_PATH=$(BUILDPATH)$(RES_O)
RES_7Z=$(RES).7z
RES_FILES_PATTERN=./data/*
ZIP=7z
ZIP_ARGS=a -t7z
ZIP_CMD=$(ZIP) $(ZIP_ARGS)

all: mkbuilddir mkzip addzip $(TESTBIN)
	
$(TESTBIN): $(TESTSRC)
	$(CC) $(CFLAGS) $(TESTSRC) $(RES_O_PATH) -o $(TESTBIN) $(LDFLAGS)
	
.PHONY: clean mkbuilddir

addzip:
	cd $(BUILDPATH); \
	ld -r -b binary $(RES_7Z) -o $(RES_O)

mkzip:
	-$(ZIP_CMD) $(BUILDPATH)$(RES_7Z) $(RES_FILES_PATTERN)

mkbuilddir:
	mkdir -p $(BUILDDIR)

clean:
	-rm -dr $(BUILDROOT)
	