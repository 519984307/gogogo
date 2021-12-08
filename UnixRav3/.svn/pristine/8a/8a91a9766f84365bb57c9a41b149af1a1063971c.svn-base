#ifndef NEMO_RAV_H
#define NEMO_RAV_H

#include <glib-object.h>

G_BEGIN_DECLS

#define NEMO_TYPE_FR  (nemo_fr_get_type ())
#define NEMO_FR(o)    (G_TYPE_CHECK_INSTANCE_CAST ((o), NEMO_TYPE_FR, NemoFr))
#define NEMO_IS_FR(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), NEMO_TYPE_FR))

typedef struct _NemoFr      NemoFr;
typedef struct _NemoFrClass NemoFrClass;

struct _NemoFr {
    GObject __parent;
};

struct _NemoFrClass {
    GObjectClass __parent;
};

GType nemo_fr_get_type      (void);
void  nemo_fr_register_type (GTypeModule *module);

G_END_DECLS

#endif /* NEMO_RAV_H */
