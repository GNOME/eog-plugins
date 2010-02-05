#ifndef __EOG_CHAMPLAIN_PLUGIN_H__
#define __EOG_CHAMPLAIN_PLUGIN_H__

#include <glib.h>
#include <glib-object.h>
#include <eog/eog-plugin.h>

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define EOG_TYPE_CHAMPLAIN_PLUGIN		(eog_champlain_plugin_get_type ())
#define EOG_CHAMPLAIN_PLUGIN(o)			(G_TYPE_CHECK_INSTANCE_CAST ((o), EOG_TYPE_CHAMPLAIN_PLUGIN, EogChamplainPlugin))
#define EOG_CHAMPLAIN_PLUGIN_CLASS(k)		(G_TYPE_CHECK_CLASS_CAST((k),     EOG_TYPE_CHAMPLAIN_PLUGIN, EogChamplainPluginClass))
#define EOG_IS_CHAMPLAIN_PLUGIN(o)	        (G_TYPE_CHECK_INSTANCE_TYPE ((o), EOG_TYPE_CHAMPLAIN_PLUGIN))
#define EOG_IS_CHAMPLAIN_PLUGIN_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE ((k),    EOG_TYPE_CHAMPLAIN_PLUGIN))
#define EOG_CHAMPLAIN_PLUGIN_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o),  EOG_TYPE_CHAMPLAIN_PLUGIN, EogChamplainPluginClass))

/* Private structure type */
typedef struct _EogChamplainPluginPrivate	EogChamplainPluginPrivate;

/*
 * Main object structure
 */
typedef struct _EogChamplainPlugin		EogChamplainPlugin;

struct _EogChamplainPlugin
{
	EogPlugin parent_instance;
};

/*
 * Class definition
 */
typedef struct _EogChamplainPluginClass	EogChamplainPluginClass;

struct _EogChamplainPluginClass
{
	EogPluginClass parent_class;
};

/*
 * Public methods
 */
GType	eog_champlain_plugin_get_type		(void) G_GNUC_CONST;

/* All the plugins must implement this function */
G_MODULE_EXPORT GType register_eog_plugin (GTypeModule *module);

G_END_DECLS

#endif /* __EOG_CHAMPLAIN_PLUGIN_H__ */
