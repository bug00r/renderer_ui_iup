#include "plugin.h"

Plugin * new_plugin() {
	Plugin * new_plugin = malloc(sizeof(Plugin));
	new_plugin->name = NULL;
	new_plugin->frame = NULL;
	new_plugin->init = NULL;
	new_plugin->free = NULL;
	new_plugin->data = NULL;
	new_plugin->prepare = NULL;
	new_plugin->cleanup = NULL;
	return new_plugin;
}
void free_plugin(Plugin * plugin) {
	free(plugin);
}