#ifndef _PANZER_INTERFACE_H_
#define _PANZER_INTERFACE_H_

#define MAX_PATHNAME 1024

#define PANZER_PROTO 28
#define PANZER_USER_PID 1

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack (push, 1)

/* verdict returned by user (if VM_ask masked) */
enum user_verdict {
	UV_pass = 0,
	UV_deny,
};

enum action {
	ACTION_execve = 0,
	ACTION_create,
	ACTION_unlink,
	ACTION_written,
	ACTION_chmod,
	ACTION_rename,
	ACTION_write,

	ACTION_maximum,
};

static inline
const char *s_action(enum action c) {
	const char *strings[] = {
		"ACTION_execve",
		"ACTION_create",
		"ACTION_unlink",
		"ACTION_written",
		"ACTION_chmod",
		"ACTION_rename",
		"ACTION_write",

		"ACTION_maximum",
	};
	return strings[c];
}

/* common header of all info buffers */
struct info_header {
	int pid;

	enum action action;

	enum user_verdict verdict;

	char curdir[MAX_PATHNAME + 1];
	char pathname[MAX_PATHNAME + 1];

	union {
		struct {
			char cmdline[MAX_PATHNAME];
		} execve;

		struct {
			char newname[MAX_PATHNAME];
		} rename;
	};
};


#ifdef __cplusplus
}
#endif

#pragma pack (pop)

#endif /* _PANZER_INTERFACE_H_ */
