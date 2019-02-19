#include "plugin.h"

plugin_t * new_plugin() {
	plugin_t * new_plugin = malloc(sizeof(plugin_t));
	new_plugin->name = NULL;
	new_plugin->frame = NULL;
	new_plugin->init = NULL;
	new_plugin->free = NULL;
	new_plugin->data = NULL;
	new_plugin->prepare = NULL;
	new_plugin->cleanup = NULL;
	return new_plugin;
}
void free_plugin(plugin_t * plugin) {
	free(plugin);
}