#ifndef __EOG_MAP_PLUGIN_H__
#define __EOG_MAP_PLUGIN_H__

#include <glib.h>
#include <glib-object.h>
#include <eog/eog-plugin.h>

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define EOG_TYPE_MAP_PLUGIN		(eog_map_plugin_get_type ())
#define EOG_MAP_PLUGIN(o)		(G_TYPE_CHECK_INSTANCE_CAST ((o), EOG_TYPE_MAP_PLUGIN, EogMapPlugin))
#define EOG_MAP_PLUGIN_CLASS(k)		(G_TYPE_CHECK_CLASS_CAST((k),     EOG_TYPE_MAP_PLUGIN, EogMapPluginClass))
#define EOG_IS_MAP_PLUGIN(o)		(G_TYPE_CHECK_INSTANCE_TYPE ((o), EOG_TYPE_MAP_PLUGIN))
#define EOG_IS_MAP_PLUGIN_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE ((k),    EOG_TYPE_MAP_PLUGIN))
#define EOG_MAP_PLUGIN_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o),  EOG_TYPE_MAP_PLUGIN, EogMapPluginClass))

/* Private structure type */
typedef struct _EogMapPluginPrivate	EogMapPluginPrivate;

/*
 * Main object structure
 */
typedef struct _EogMapPlugin		EogMapPlugin;

struct _EogMapPlugin
{
	EogPlugin parent_instance;
};

/*
 * Class definition
 */
typedef struct _EogMapPluginClass	EogMapPluginClass;

struct _EogMapPluginClass
{
	EogPluginClass parent_class;
};

/*
 * Public methods
 */
GType	eog_map_plugin_get_type		(void) G_GNUC_CONST;

/* All the plugins must implement this function */
G_MODULE_EXPORT GType register_eog_plugin (GTypeModule *module);

G_END_DECLS

#endif /* __EOG_MAP_PLUGIN_H__ */
