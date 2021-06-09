#MAKE?=mingw32-make
AR?=ar
ARFLAGS?=rcs
PATHSEP?=/
CC=gcc
BUILDROOT?=build

ifeq ($(CLANG),1)
	export CC=clang
endif

BUILDDIR?=$(BUILDROOT)$(PATHSEP)$(CC)
BUILDPATH?=$(BUILDDIR)$(PATHSEP)

INSTALL_ROOT?=$(BUILDPATH)

ifeq ($(DEBUG),1)
	export debug=-ggdb -Ddebug=1
	export isdebug=1
endif

ifeq ($(ANALYSIS),1)
	export analysis=-Danalysis=1
	export isanalysis=1
endif

ifeq ($(DEBUG),2)
	export debug=-ggdb -Ddebug=2
	export isdebug=1
endif

ifeq ($(DEBUG),3)
	export debug=-ggdb -Ddebug=3
	export isdebug=1
endif

ifeq ($(OUTPUT),1)
	export outimg= -Doutput=1
endif

CFLAGS=-std=c11 -Wpedantic -pedantic-errors -Wall -Wextra -O1 $(debug)
#-ggdb
#-pg for profiling 

LIB?=-L/c/dev/lib
INCLUDE?=-I/c/dev/include -I.

CFLAGS=-std=c11 
#-Wpedantic -pedantic-errors -Wall -Wextra -O1 $(debug) $(analysis)
#-ggdb  -mwindows
#-pg for profiling 

#MATH_ALGORITHM_DIR=$(patsubst %,algorithm/%,fractals noise)
#MATH_LIB_DIR=$(patsubst %,math/%, $(MATH_ALGORITHM_DIR) statistics utils mat vec)
#COLLECTION_LIB_DIR=$(patsubst %,collections/%,array)
#RENDERER_LIB_DIR=renderer scene mesh shape color texture $(MATH_LIB_DIR) $(COLLECTION_LIB_DIR)
#RENDERER_INCLUDE_DIR=renderer/include scene mesh shape color texture $(MATH_LIB_DIR) $(COLLECTION_LIB_DIR)

#IUP_INCLUDE=$(patsubst %,_third_/tec_tools/%/include/,iup cd im)
INCLUDEDIR=-I. $(INCLUDE)
#INCLUDEDIR+=$(patsubst %,-I./../%,$(IUP_INCLUDE) $(RENDERER_INCLUDE_DIR))

_SRC_FILES=test_renderer_ui_iup app iup_app plugin plugin_ui_main plugin_ui_renderer_context plugin_ui_renderer plugin_ui_texturing
TESTSRC=$(patsubst %,src/%,$(patsubst %,%.c,$(_SRC_FILES)))
TESTSRC+=$(patsubst %,src/%,$(patsubst %,%.h,$(_SRC_FILES)))
#$(info $$_TESTSRC is [${_TESTSRC}])
TESTBIN=test_renderer_ui_iup.exe

RENDERER_LIBS=renderer scene mesh shape texture noise fractals crgb_array farray array color statistics utilsmath mat vec
IUP_LIBS=cdcontextplus gdiplus im iupcd iup cd
THIRD_PARTY_LIBS=freetype6 z
OS_LIBS=kernel32 user32 gdi32 winspool comdlg32 advapi32 shell32 uuid ole32 oleaut32 comctl32
		   
TESTLIB=$(patsubst %,-l%,$(RENDERER_LIBS) $(IUP_LIBS) $(THIRD_PARTY_LIBS) $(OS_LIBS))

TESTLIBDIR=-L$(BUILDDIR) $(LIB) 
#$(patsubst %,%/$(BUILDDIR),$(patsubst %,-L./../%,$(RENDERER_LIB_DIR))) -L./../_third_/tec_tools/release/static/ 

all: mkbuilddir $(BUILDPATH)$(TESTBIN)
	
$(BUILDPATH)$(TESTBIN): $(TESTSRC)
	$(CC) $(CFLAGS) $(TESTSRC) -o $(BUILDPATH)$(TESTBIN) $(INCLUDEDIR) $(TESTLIBDIR) $(TESTLIB)
	
.PHONY: clean mkbuilddir

mkbuilddir:
	mkdir -p $(BUILDDIR)

clean:
	-rm -dr $(BUILDROOT)
	