#include <libcaja-extension/caja-extension-types.h>
#include <libcaja-extension/caja-column-provider.h>

#include "caja-rav.h"

#define GETTEXT_PACKAGE "RAV"

void
caja_module_initialize (GTypeModule *module)
{
	caja_fr_register_type (module);

    bindtextdomain (GETTEXT_PACKAGE, "/usr/share");
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
}


void
caja_module_shutdown (void)
{
}


void 
caja_module_list_types (const GType **types,
			    int          *num_types)
{
	static GType type_list[1];
	
	type_list[0] = CAJA_TYPE_FR;
	*types = type_list;
	*num_types = 1;
}
