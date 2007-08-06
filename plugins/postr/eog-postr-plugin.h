#ifndef __EOG_POSTR_PLUGIN_H__
#define __EOG_POSTR_PLUGIN_H__

#include <glib.h>
#include <glib-object.h>
#include <eog/eog-plugin.h>

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define EOG_TYPE_POSTR_PLUGIN		(eog_docinfo_plugin_get_type ())
#define EOG_POSTR_PLUGIN(o)		(G_TYPE_CHECK_INSTANCE_CAST ((o), EOG_TYPE_DOCINFO_PLUGIN, EogPostrPlugin))
#define EOG_POSTR_PLUGIN_CLASS(k)	(G_TYPE_CHECK_CLASS_CAST((k), EOG_TYPE_DOCINFO_PLUGIN, EogPostrPluginClass))
#define EOG_IS_DOCINFO_PLUGIN(o)	(G_TYPE_CHECK_INSTANCE_TYPE ((o), EOG_TYPE_DOCINFO_PLUGIN))
#define EOG_IS_DOCINFO_PLUGIN_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE ((k), EOG_TYPE_DOCINFO_PLUGIN))
#define EOG_POSTR_PLUGIN_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o), EOG_TYPE_DOCINFO_PLUGIN, EogPostrPluginClass))

/* Private structure type */
typedef struct _EogPostrPluginPrivate	EogPostrPluginPrivate;

/*
 * Main object structure
 */
typedef struct _EogPostrPlugin		EogPostrPlugin;

struct _EogPostrPlugin
{
	EogPlugin parent_instance;
};

/*
 * Class definition
 */
typedef struct _EogPostrPluginClass	EogPostrPluginClass;

struct _EogPostrPluginClass
{
	EogPluginClass parent_class;
};

/*
 * Public methods
 */
GType	eog_postr_plugin_get_type		(void) G_GNUC_CONST;

/* All the plugins must implement this function */
G_MODULE_EXPORT GType register_eog_plugin (GTypeModule *module);

G_END_DECLS

#endif /* __EOG_POSTR_PLUGIN_H__ */
