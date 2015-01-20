#ifndef ORG_EEROS_IPC_MESSAGEPASSING_HPP_
#define ORG_EEROS_IPC_CALLER_HPP_

#include <eeros/ipc/apply.hpp>

namespace eeros {
	namespace ipc {

		class MessagePassing {
		public:
			virtual ~MessagePassing() { } 
			
			virtual int open(const char *name) = 0;
			virtual void close(int fd) = 0;
			virtual int register_server(int fd, const char *name) = 0;
			virtual int unregister_server(int fd) = 0;
			virtual int accept(int fd, int value) = 0;
			virtual int receive(int fd, int &coid, void *request, int size) = 0;
			virtual int receive(int fd, int &coid, void *request, int size, int &request_size) = 0;
			virtual int request(int fd, const void *request, int request_size, void *reply, int reply_size) = 0;
			virtual int reply(int fd, int coid, const void *reply, int size) = 0;
			
			static MessagePassing& get_default_impl();
		};

	};
};

#endif // ORG_EEROS_IPC_CALLER_HPP_
