#ifndef _RSUTILS_INTERFACE_H_
#define _RSUTILS_INTERFACE_H_

#include "gendef.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack (push, 1)

#ifndef __x86_64__
# define rsutils_decl __cdecl
#else
# define rsutils_decl
#endif
#define rsutils_if(type) type rsutils_decl

#define MAX_URL_LENGTH	1024
#define MAX_OBJNAME_LENGTH	1024

#ifndef __KERNEL__

/* get kernel module version */
rsutils_if(uint32_t) get_version(void);
rsutils_if(bool_t) set_work_path(wchar_t *path);
rsutils_if(bool_t) is_file_handle(unsigned __int64 handle, pid_t pid);
rsutils_if(bool_t) set_homepage0(wchar_t *homepage);
rsutils_if(bool_t) inject_dll_auto(wchar_t *dll_path);
rsutils_if(bool_t) inject_dll_auto_wow64(wchar_t *dll_path);
rsutils_if(bool_t) inject_dll(pid_t pid, wchar_t *dll_path);
rsutils_if(ptr64_t) mmap(pid_t pid, ptr64_t target_addr, uint32_t target_sz);
rsutils_if(void) munmap(pid_t pid, ptr64_t target_addr, uint32_t target_sz, ptr64_t va);

/* fs api */

#define FILE_SUPERSEDE                  0x00000000
#define FILE_OPEN                       0x00000001
#define FILE_CREATE                     0x00000002
#define FILE_OPEN_IF                    0x00000003
#define FILE_OVERWRITE                  0x00000004
#define FILE_OVERWRITE_IF               0x00000005
#define FILE_MAXIMUM_DISPOSITION        0x00000005

rsutils_if(ptr64_t) f_create(wchar_t *file_path, uint32_t disposition);
rsutils_if(void) f_close(ptr64_t handle);
/* origin: FILE_BEGIN, FILE_CURRENT, FILE_END */
rsutils_if(int) f_seek(ptr64_t handle, uint64_t offset, int origin);
rsutils_if(int) f_tell(ptr64_t handle, uint64_t *offset);
rsutils_if(uint32_t) f_write(ptr64_t handle, void *buffer, uint32_t len);
rsutils_if(uint32_t) f_read(ptr64_t handle, void *buffer, uint32_t len);
rsutils_if(int) f_write_ex(ptr64_t handle, const void *buffer, uint32_t len, uint32_t *retlen);
rsutils_if(int) f_read_ex(ptr64_t handle, void *buffer, uint32_t len, uint32_t *retlen);
rsutils_if(int) f_flush(ptr64_t handle);
rsutils_if(int) f_set_eof(ptr64_t handle);
rsutils_if(int) f_size(ptr64_t handle, uint64_t *size);
rsutils_if(int) f_get_attrib(ptr64_t handle, uint32_t *attrib);
rsutils_if(int) f_set_attrib(ptr64_t handle, uint32_t attrib);
rsutils_if(int) f_delete(ptr64_t handle, bool_t force);
rsutils_if(int) f_rename(ptr64_t handle, wchar_t *file_name, bool_t replace_exist);
rsutils_if(int) f_first(ptr64_t handle, void *info, uint32_t len);
rsutils_if(int) f_next(ptr64_t handle, void *info, uint32_t len);

/* reg api  ²»Ö§³Ö 64bit */

rsutils_if(ptr64_t) r_create_key(wchar_t *key_name);
rsutils_if(ptr64_t) r_open_key(wchar_t *key_name);
rsutils_if(void) r_close_key(ptr64_t handle);
rsutils_if(int) r_set_valuekey(ptr64_t handle, wchar_t *value_name, uint32_t type, void *data, uint32_t len);
rsutils_if(int) r_enum_valuekey(ptr64_t handle, uint32_t index, wchar_t *value_name, uint32_t *value_name_len, uint32_t *reserved, uint32_t *type, void *data, uint32_t *data_len);
rsutils_if(int) r_delete_valuekey(ptr64_t handle, wchar_t *value_name);
rsutils_if(int) r_delete_key(ptr64_t handle);
rsutils_if(int) r_query_value(ptr64_t handle, wchar_t *value_name, uint32_t *type, uint8_t *data, uint32_t *data_len);
rsutils_if(int) r_enum_key(ptr64_t handle, uint32_t index, wchar_t *name, uint32_t *name_len);
rsutils_if(int) r_query_key(ptr64_t handle, uint32_t *subkeys, uint32_t *max_subkey_len, uint32_t *values, uint32_t *max_valuename_len, uint32_t *max_value_len);

#endif /* __KERNEL__ */

#ifdef __cplusplus
}
#endif

#pragma pack (pop)

#endif /* _RSUTILS_INTERFACE_H_ */
