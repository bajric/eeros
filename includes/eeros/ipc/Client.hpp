#ifndef ORG_EEROS_IPC_CLIENT_HPP_
#define ORG_EEROS_IPC_CLIENT_HPP_

#include <eeros/ipc/MessagePassing.hpp>

namespace eeros {
	namespace ipc {

		class Client {
		public:
			Client(MessagePassing &mp);
			Client();
			
			void connect(const char *name);
			void disconnect();
			
			
			template < typename TArgs, typename TResult >
			inline int call(int id, const TArgs &args, TResult &result) {
				struct __attribute__ ((__packed__)) {
					int type;
					int extra;
					TArgs data;
				} request;
				struct __attribute__ ((__packed__)) {
					int type;
					int extra;
					TResult data;
				} reply;
				
				request.type = id;
				request.data = args;
				int r = mp.request(fd, &request, sizeof(request), &reply, sizeof(reply));
				result = reply.data;
				return r;
			}
			
			template < typename TArgs >
			inline int call(int id, const TArgs &args) {
				struct __attribute__ ((__packed__)) {
					int type;
					int extra;
					TArgs data;
				} request;
				
				request.type = id;
				request.data = args;
				return mp.request(fd, &request, sizeof(request), nullptr, 0);
			}
			
		private:
			MessagePassing &mp;
			int fd;
		};
	};
};

#endif // ORG_EEROS_IPC_CLIENT_HPP_
