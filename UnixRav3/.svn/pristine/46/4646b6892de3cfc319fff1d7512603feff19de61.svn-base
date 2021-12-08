

#include "obj_tbl.h"

#ifdef _MSC_VER

#include <ntifs.h>
#include <gendef.h>
#include <rbtree.h>
#include <win/rwlock.h>

#else

#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/rbtree.h>
#include <linux/rwsem.h>

#endif

#include "debug.h"

static int __link_ote_rb(struct rb_root *root, struct ot_entry *ote)
{
	struct rb_node **link, *parent = 0;
	link = &root->rb_node;
	while (*link) {
		struct ot_entry *tmp_ote;
		parent = *link;
		tmp_ote = container_of(parent, struct ot_entry, rb_link);
		if (ote->key < tmp_ote->key) {
			link = &(*link)->rb_left;
		} else if (ote->key > tmp_ote->key) {
			link = &(*link)->rb_right;
		} else {
			/* what ?! */
			return -1;
		}
	}
	rb_link_node(&ote->rb_link, parent, link);
	rb_insert_color(&ote->rb_link, root);

	return 0;
}

static struct ot_entry *__ot_find_obj(struct ot_header *header, void *key)
{
	struct rb_node *link = header->head.rb_node;
	while (link) {
		struct ot_entry *ote;
		ote = container_of(link, struct ot_entry, rb_link);
		if (key < ote->key) {
			link = link->rb_left;
		} else if (key > ote->key) {
			link = link->rb_right;
		} else {
			return ote;
		}
	}
	return NULL;
}

static struct ot_entry *__ot_allocate_ote(struct ot_header *header, uint32_t ote_sz)
{
	return kmalloc(ote_sz, 0);
}

static void __ot_free_ote(struct ot_header *header, struct ot_entry *ote)
{
	kfree(ote);
}

static void __ot_clear(struct ot_header *header)
{
	__log_error("full\r\n");
	while (!RB_EMPTY_ROOT(&header->head)) {
		struct rb_node *node = header->head.rb_node;
		struct ot_entry *ote = container_of(node, struct ot_entry, rb_link);
		rb_erase(node, &header->head);
		header->cnt--;
		kobj_put((struct kobj *)ote);
	}
}

static void __ot_lock_write(struct ot_header *header)
{
	down_write(&header->lock);
}

static void __ot_lock_read(struct ot_header *header)
{
	down_read(&header->lock);
}

static void __ot_unlock_read(struct ot_header *header)
{
	up_read(&header->lock);
}

static void __ot_unlock_write(struct ot_header *header)
{
	up_write(&header->lock);
}

static void __ot_release_obj(struct kobj *obj, void *ctx)
{
	struct ot_entry *ote = (struct ot_entry *)obj;

	if (ote->free != NULL) {
		ote->free(ote);
	}

	__ot_free_ote(ote->header, ote);
}

struct ot_entry *ot_add_obj(struct ot_header *header, void *key,
	void(*free_ptr)(struct ot_entry *ote),
	uint32_t ote_sz, int *is_create)
{
	struct ot_entry *ote;

	if (ote_sz < sizeof(struct ot_entry))
		return NULL;

	__ot_lock_read(header);
	ote = __ot_find_obj(header, key);
	if (ote != NULL) {
		kobj_get((struct kobj *)ote);
	}
	__ot_unlock_read(header);

	if (ote != NULL) {
		return (struct ot_entry *)ote;
	}

	ote = __ot_allocate_ote(header, ote_sz);
	if (ote == NULL) {
		return NULL;
	}
	memset(ote, 0, ote_sz);
	kobj_init((struct kobj *)ote, __ot_release_obj, NULL);
	ote->key = key;
	ote->header = header;
	ote->free = free_ptr;

	__ot_lock_write(header);

	if (header->cnt >= header->max_cnt) {
		__ot_clear(header);
	}
	if (__link_ote_rb(&header->head, ote) >= 0) {
		header->cnt++;
	} else {
		/* already link, free */
		__ot_free_ote(header, ote);
		__ot_unlock_write(header);
		return NULL;
	}

	kobj_get((struct kobj *)ote);

	__ot_unlock_write(header);

	return (struct ot_entry *)ote;
}

struct ot_entry *ot_get_obj(struct ot_header *header, void *key)
{
	struct ot_entry *ote;

	__ot_lock_read(header);
	ote = __ot_find_obj(header, key);

	if (ote != NULL) {
		kobj_get((struct kobj *)ote);
	} else {
		ote = NULL;
	}
	__ot_unlock_read(header);

	return (struct ot_entry *)ote;
}

void ot_release_obj(struct ot_header *header, struct ot_entry *ote)
{
	kobj_put((struct kobj *)ote);
}

struct ot_entry *ot_del_obj(struct ot_header *header, void *obj)
{
	struct ot_entry *ote = NULL;

	__ot_lock_write(header);

	ote = __ot_find_obj(header, obj);

	if (ote != NULL) {
		rb_erase(&ote->rb_link, &header->head);
		header->cnt--;
	}

	__ot_unlock_write(header);

	return ote;
}

void ot_clear(struct ot_header *header)
{
	struct rb_node *node;
	unsigned long nb = 0, i = 0;
	struct ot_entry **ote_lst = NULL;

	__ot_lock_write(header);

	do {

		for (node = rb_first(&header->head); node; node = rb_next(node)) {
			nb++;
		}

		if (nb == 0)
			break;

		ote_lst = kmalloc(sizeof(struct ot_entry *) *nb, 0);
		if (ote_lst == NULL)
			break;

		while (!RB_EMPTY_ROOT(&header->head)) {
			struct rb_node *n = header->head.rb_node;
			struct ot_entry *ote = container_of(n, struct ot_entry, rb_link);
			rb_erase(n, &header->head);
			header->cnt--;
			ote_lst[i] = ote; i++;
		}

	} while (0);

	__ot_unlock_write(header);

	for (i = 0; i < nb; i++) {
		kobj_put((struct kobj *)ote_lst[i]);
	}

	if (ote_lst != NULL) {
		kfree(ote_lst);
	}

}

int ot_init(struct ot_header *header, uint32_t max_cnt)
{
	memset(&header->head, 0, sizeof(struct rb_root));
	header->max_cnt = max_cnt;
	header->cnt = 0;

	if (max_cnt == 0) {
		return -1;
	}

	init_rwsem(&header->lock);

	return 0;
}

void ot_free(struct ot_header *header)
{
	ot_clear(header);
}
