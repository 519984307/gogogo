#ifndef	__MSGUP_H__

#define  __MSGUP_H__

#ifdef EXPORT_SMG_API
#define SMG_API __declspec(dllexport)
#else
#define SMG_API
#endif

SMG_API long msgp_env_init();
SMG_API void msgp_env_uninit();
SMG_API long msgp_env_set(const char* name , const char* value);

SMG_API void* msgp_messager_create(const char* qpath , const char* user_defined_header);
SMG_API long  msgp_messager_post(void* handle,const char* topic , uint8_t* msg , int32_t len);
SMG_API void  msgp_messager_release(void* handle);

#endif