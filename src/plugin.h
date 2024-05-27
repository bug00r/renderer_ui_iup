#ifndef PLUGIN_H
#define PLUGIN_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct {
	void * data;
	const char * (*name)(void * data);
	void * (*frame)(void * data);
	void (*init)(void * data);
	void (*free)(void * data);
	void (*prepare)(void * data);
	void (*cleanup)(void * data);
} Plugin;

Plugin * new_plugin();
void free_plugin(Plugin * plugin);

#endif