#ifndef __HIREDIS_CLIENT_CXX__
#define __HIREDIS_CLIENT_CXX__

#include <atomic>
#include <vector>
#include <string>
#include <future>
#include "iredisxx.hpp"
#ifdef _WIN32
#include <winsock2.h>
#endif

namespace redis { 


	enum {
		STATE_UNCONNECTED,
		STATE_CONNECTED,
		STATE_BUSY,
	};

	class client_t : public i_redis_client_t
	{

	protected:

		void async_reconnect( void * p ) {
			refp<client_t> client;
			client.attach((client_t*)p);
			int err = client->connect(false);
			if (!err) return;
		}

	protected:

		redisContext *		context_;
		std::atomic<int>	state_;
		std::string			server_;
		int					port_;
		std::string			errstr_;

	protected:
		void clear_context() {
			state_ = STATE_UNCONNECTED;
			if (context_) redisFree(context_);
			context_ = 0;
		}
		void pre_operation() {
			errstr_ = "";
		}
		int acquire() {
			int sexp = STATE_CONNECTED;
			if (!state_.compare_exchange_strong(sexp, STATE_BUSY)) {
				errstr_ = "acquire redisContext failed.";
				return -1;
			}
			return 0;
		}
		int recycle( int err ) {
			if (!err) {
				state_ = STATE_CONNECTED;
				return err;
			}
			
			clear_context();

			try {
				refp<client_t> slef(this);
				std::thread athread([this]() {
					refp<client_t> client;
					client.attach(this);
					while (true) {
						if (get_ref_count() == 1) break;
						int err = client->connect(false,1000);
						fprintf( stderr, "info: redis::reconnect return %d\n", err);
						if (!err) return;
						std::this_thread::sleep_for(std::chrono::seconds(1));
					}
					
				});
				slef.detach();
				athread.detach();
			}
			catch (...) {
				return -2;
			}

			return 0;
		}
	public:

		client_t() : context_(0), state_(0), port_(6379) {
			server_ = "127.0.0.1";
		}
		~client_t() {
			clear_context();
		}
		int set_remote(const char * server, int port) {
			server_ = server;
			port_ = port;
			return 0;
		}
		bool is_connected() {
			return state_ == STATE_CONNECTED;
		}
		int aconnect() {
			return recycle(-1982);
		}
		int connect( bool async_if_failed = true, size_t timeout_ms = 5000) {
			pre_operation();
			int ret = -1;
			clear_context();
			struct timeval tv = { timeout_ms / 1000, timeout_ms % 1000 };
			redisContext * p = redisConnectWithTimeout(server_.c_str(), port_, tv);
			if (!p) return ret;
			if (p->err) {
				errstr_ = p->errstr;
				ret = - p->err;
				redisFree(p);
				if (async_if_failed) {
					ret = aconnect();
					if (ret) return ret;
					return 1;
				}
				return ret;
			}
			context_ = p;
			state_ = STATE_CONNECTED;
			return 0;
		}
		/*int connect2( size_t timeout_ms = 5000 ) {
			pre_operation();
			int ret = -1;
			clear_context();
			timeval tv = { timeout_ms / 1000, timeout_ms % 1000 };
			redisContext * p = redisConnectWithTimeout(server_.c_str(), port_, tv);
			if (!p) return ret;
			if (p->err) {
				errstr_ = p->errstr;
				ret = -p->err;
				redisFree(p);
				return ret;
			}
			context_ = p;
			state_ = STATE_CONNECTED;
			return 0;
		}*/

		int exec(rcommand_t cmd, str_vec_t & args, reply_t & reply, size_t timeout_ms = 100) {
			int i = pipe_add_command(cmd, args);
			if (i < 0) return i;
			return pipe_get_reply(reply,timeout_ms);
		}
		int exec(reply_t & reply, size_t timeout_ms, const char * format, ...) {
			va_list ap;
			int ret;
			va_start(ap, format);
			ret = pipe_add_command(format, ap);
			va_end(ap);
			if (ret < 0) return ret;
			return pipe_get_reply(reply, timeout_ms);
		}
		int pipe_add_command( const char * format, ...) {
			va_list ap;
			int ret;
			va_start(ap, format);
			ret = pipe_add_command(format, ap);
			va_end(ap);
			return ret;
		}
		int pipe_add_command( const char * format, va_list ap ) {
			pre_operation();
			int err = acquire();
			if (err < 0) return err;
			err = redisvAppendCommand(context_, format, ap);
			recycle(err);
			return err;
		}
		int pipe_add_command( rcommand_t cmd, str_vec_t & args ) {
			pre_operation();
			int err = acquire();
			if (err < 0) return err;
			std::vector< const char*> argv;
			std::vector< size_t > argvl;
			int argc = 1 + args.size();
			argv.reserve(argc);
			argvl.reserve(argc);
			argv.push_back(cmd.name);
			argvl.push_back(cmd.size);
			for (auto & k : args) {
				argv.push_back(k.c_str());
				argvl.push_back(k.size());
			}
			err = redisAppendCommandArgv(context_, argc, &argv[0], &argvl[0]);
			recycle(err);
			return err;
		}

		int pipe_get_reply( reply_t & reply, size_t timeout_ms ) {
			pre_operation();
			int err = acquire();
			if (err < 0) return err;

			timeval tv = { timeout_ms / 1000, timeout_ms % 1000 };
			err = redisSetTimeout(context_, tv);

			if (err < 0) {
				recycle(0);	// redisSetTimeout not tigger re-connect
				errstr_ = "redisSetTimeout error.";
				return err;
			}

			redisReply * reply_ptr = 0;
			err = redisGetReply(context_, (void**)&reply_ptr);
			
			if (!err && !reply_ptr) err = -1;
			recycle(err);

			if (err) return err;

			reply.attach(reply_ptr, true);

			return 0;
		}

		virtual const char * err_str() {
			if (!context_) return "redis unconnected.";
			return context_->errstr;
		}

		static int create_instance (i_redis_client_t ** pp) {
			if (!pp) return -1;
			client_t*  p = new client_t();
			if (!p) return -2;
			*pp = p;
			p->_reference();
			return 0;
		}

	};

	

};

#endif