
#ifndef _KOBJ_H_
#define _KOBJ_H_

#include "atomic.h"

struct kobj {
#ifdef _MSC_VER
	long refcount;
#else
	/* Linux */
	atomic_t refcount;
#endif
	void *release_ctx;
	void (*release)(struct kobj *kobj, void *ctx);
};

static __inline void kobj_init(struct kobj *kobj, void (*release)(struct kobj *kobj, void *ctx), void *ctx)
{
	atomic_set(&kobj->refcount, 1);
	kobj->release = release;
	kobj->release_ctx = ctx;
}

static __inline struct kobj *kobj_get(struct kobj *kobj)
{
	atomic_inc(&kobj->refcount);
	return kobj;
}

static __inline int kobj_put(struct kobj *kobj)
{
	if (atomic_dec_and_test(&kobj->refcount)) {
		kobj->release(kobj, kobj->release_ctx);
		return 1;
	}
	return 0;
}

#endif /* _KOBJ_H_ */
