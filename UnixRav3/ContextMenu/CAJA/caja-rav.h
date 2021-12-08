#ifndef CAJA_RAV_H
#define CAJA_RAV_H

#include <glib-object.h>

G_BEGIN_DECLS

#define CAJA_TYPE_FR  (caja_fr_get_type ())
#define CAJA_FR(o)    (G_TYPE_CHECK_INSTANCE_CAST ((o), CAJA_TYPE_FR, CajaFr))
#define CAJA_IS_FR(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), CAJA_TYPE_FR))

typedef struct _CajaFr      CajaFr;
typedef struct _CajaFrClass CajaFrClass;

struct _CajaFr {
	GObject __parent;
};

struct _CajaFrClass {
	GObjectClass __parent;
};

GType caja_fr_get_type      (void);
void  caja_fr_register_type (GTypeModule *module);

G_END_DECLS

#endif /* CAJA_RAV_H */
