#include <config.h>
#include <libnemo-extension/nemo-extension-types.h>
#include <libnemo-extension/nemo-column-provider.h>
#include <glib/gi18n-lib.h>

#include "nemo-rav.h"


void
nemo_module_initialize (GTypeModule*module)
{
    nemo_fr_register_type (module);

    bindtextdomain (GETTEXT_PACKAGE, "/usr/share");
    bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
}


void
nemo_module_shutdown (void)
{
}


void
nemo_module_list_types (const GType **types,
                int          *num_types)
{
    static GType type_list[1];

    type_list[0] = NEMO_TYPE_FR;
    *types = type_list;
    *num_types = 1;
}
