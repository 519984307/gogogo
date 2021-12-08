#ifndef __I_REDIS_CLIENT_XX__
#define __I_REDIS_CLIENT_XX__

#include "../lib3rd/redis/deps/hiredis/hiredis.h"
#include "../include/platform.h"
#include "../include/refable.h"
#include <vector>


namespace redis {

	typedef std::vector< std::string > str_vec_t;

	typedef struct command_t {
		const char *	name;
		const size_t	size;
		command_t(const char * p, size_t l) : name(p), size(l) {
		}
	} command_t;

	typedef const command_t & rcommand_t;

#define DEF_CMD(x)		static const command_t x( #x, sizeof(#x)-1 );
	DEF_CMD(GET)
		DEF_CMD(MGET)
		DEF_CMD(SET)
		DEF_CMD(SETEX)
		DEF_CMD(SUBSCRIBE)
		DEF_CMD(HMGET)
#undef DEF_CMD


	class reply_t {
	public:
		typedef size_t klass_t;
		#define t_err REDIS_REPLY_ERROR
		#define t_int REDIS_REPLY_INTEGER
		#define t_str REDIS_REPLY_STRING
		#define t_vec REDIS_REPLY_ARRAY
		#define t_nil REDIS_REPLY_NIL
		#define t_stc REDIS_REPLY_STATUS
	protected:
		bool			need_free_;
		redisReply *	reply_;
	public:
		reply_t() : reply_(0) {
		}
		reply_t(redisReply *x) : reply_(x), need_free_(true) {

		}
		reply_t(redisReply *x, bool is_root) : reply_(x), need_free_(is_root) {

		}
		~reply_t() {
			if (need_free_ && reply_) freeReplyObject(reply_);
		}
		bool attach(redisReply *x, bool neef_free) {
			if (reply_) return false;
			reply_ = x;
			need_free_ = neef_free;
			return true;
		}
		klass_t klass() const {
			if (!reply_) return t_err;
			return reply_->type;
		}
		reply_t operator[](size_t i) {
			if (!reply_ || !is_vec() || i >= reply_->elements) {
				return reply_t();
			}
			return reply_t(reply_->element[i], false);
		}
		bool is_ok() const {
			return klass() != t_err;
		}
		bool is_int() const {
			return klass() == t_int;
		}
		bool is_str() const {
			return klass() == t_str;
		}
		bool is_vec() const {
			return klass() == t_vec;
		}
		bool is_nil() const {
			return klass() == t_nil;
		}
		bool is_status() const {
			return klass() == t_stc;
		}

		size_t vec_size() const {
			return is_vec() ? reply_->elements : 0;
		}

		int64_t as_int() const {
			return reply_->integer;
		}

		const char * as_str() const {
			if (!is_str()) return NULL;
			return reply_->str;
		}

		const char * as_status() const {
			if (!is_status()) return NULL;
			return reply_->str;
		}

		size_t str_len() {
			return reply_->len;
		}

		operator const char * () const {
			return as_str();
		}
	};

	class i_redis_client_t : public RefalbeImp {
	public:
		virtual int set_remote(const char * server, int port) = 0;
		virtual bool is_connected() = 0;
		virtual int aconnect() = 0;
		virtual int connect(bool async_if_failed = true, size_t timeout_ms = 5000) = 0;
		virtual int exec(rcommand_t cmd, str_vec_t & args, reply_t & reply, size_t timeout_ms = 100) = 0;
		virtual int exec(reply_t & reply, size_t timeout_ms, const char * format, ...) = 0;
		virtual int pipe_add_command(const char * format, ...) = 0;
		virtual int pipe_add_command(const char * format, va_list ap) = 0;
		virtual int pipe_add_command(rcommand_t cmd, str_vec_t & args) = 0;
		virtual int pipe_get_reply(reply_t & reply, size_t timeout_ms) = 0;
		virtual const char * err_str() = 0;
		static  int create_instance( i_redis_client_t ** pp);
	};

};


#endif