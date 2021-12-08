
#ifndef __OBJ2_TBL_H__
#define __OBJ2_TBL_H__

#include "kobj.h"

#ifdef _MSC_VER
#include <rbtree.h>
#include <win/rwlock.h>
#else
#include <linux/rbtree.h>
#include <linux/rwsem.h>
#endif

struct ot_header {
	struct rb_root head;
	uint32_t cnt;
	uint32_t max_cnt;
	struct rw_semaphore lock;
};

struct ot_entry {
	struct kobj obj;
	struct ot_header *header;
	void *key;
	struct rb_node rb_link;
	void (*free)(struct ot_entry *ote);
};

struct ot_entry *ot_get_obj(struct ot_header *header, void *key);
void ot_release_obj(struct ot_header *header, struct ot_entry *ote);

struct ot_entry *ot_add_obj(struct ot_header *header, void *obj,
	void(*free_ptr)(struct ot_entry *ote), uint32_t ote_sz, int *is_create);
struct ot_entry *ot_del_obj(struct ot_header *header, void *key);

void ot_clear(struct ot_header *header);

int ot_init(struct ot_header *header, uint32_t max_cnt);
void ot_free(struct ot_header *header);

#endif
