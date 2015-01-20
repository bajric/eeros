#include <eeros/ipc/MessagePassing.hpp>
#include "mpipc_ioctl.h"
#include <fcntl.h>
#include <unistd.h>

namespace {
	
	class MPIPC : public eeros::ipc::MessagePassing {
			int open(const char *name) {
				return ::open(name, 0);
			}
			
			void close(int fd) {
				::close(fd);
			}
			
			int register_server(int fd, const char *name) {
				return ::ioctl(fd, RRPS_REG, name);
			}
			
			int unregister_server(int fd) {
				return ::ioctl(fd, RRPS_UREG, 0);
			}
			
			int accept(int fd, int value) {
				return ::ioctl(fd, RRPS_ACPT, value);
			}
			
			int receive(int fd, int &coid, void *request, int size) {
				int r;
				struct mpipc_recv msg;
				msg.req.buffer = request;
				msg.req.size = size;
				r = ::ioctl(fd, RRPS_RECV, &msg);
				if (r < 0) return r;
				coid = msg.coid;
				return msg.type;
			}
			
			int receive(int fd, int &coid, void *request, int size, int &request_size) {
				int r;
				struct mpipc_recv msg;
				msg.req.buffer = request;
				msg.req.size = size;
				r = ::ioctl(fd, RRPS_RECV, &msg);
				if (r < 0) return r;
				coid = msg.coid;
				request_size = msg.req.size;
				return msg.type;
			}
			
			int request(int fd, const void *request, int request_size, void *reply, int reply_size) {
				struct mpipc_req msg;
				msg.req.buffer = const_cast<void*>(request);
				msg.req.size = request_size;
				msg.rep.buffer = reply;
				msg.rep.size = reply_size;
				return ::ioctl(fd, RRPS_REQ, &msg);
			}
			
			int reply(int fd, int coid, const void *reply, int size) {
				struct mpipc_rep msg;
				msg.coid = coid;
				msg.rep.buffer = const_cast<void*>(reply);
				msg.rep.size = size;
				return ::ioctl(fd, RRPS_REP, &msg);
			}
	} mpipc;
}

namespace eeros {
	namespace ipc {
			MessagePassing& MessagePassing::get_default_impl() {
				return mpipc;
			}
	};
};
