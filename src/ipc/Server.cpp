#include <eeros/ipc/Server.hpp>
#include <eeros/ipc/MessagePassing.hpp>
#include "mpipc_ioctl.h"

namespace eeros {
	namespace ipc {

			Server::Server(const char *name) : name(name) {
				stop_requested = false;
				
			}
			
			Server::~Server() { }
			
			void Server::stop() {
				stop_requested = true;
			}
		
			int Server::call(int id, const void *args, int args_size, void *result, int result_size) {
				auto&& m = methods[id];
				log.trace() << "calling method (" << id << "): " << m.name;
				try {
					return m.method->call(args, args_size, result, result_size);
				}
				catch (int &ex) {
					log.error() << "exception: " << ex;
				}
				catch (unsigned long &ex) {
					log.error() << "exception: " << (long)ex;
				}
				catch (std::exception &ex) {
					log.error() << "exception: " << ex.what();
				}
			}
		
			void Server::run() {
				const char *mp_control_device = "/dev/mp";
				MessagePassing &mp = MessagePassing::get_default_impl();
				int fd = mp.open(mp_control_device);
				if (fd < 0) {
					log.error() << "cannot open device file: " << mp_control_device;
					return;
				}
				if (mp.register_server(fd, name) < 0) {
					log.error() << "cannot register server name: " << name;
					mp.close(fd);
					return;
				}
				
				struct {
					int type;
					int extra;
					char data[1024];
				} request;
				struct {
					int type;
					int extra;
					char data[1024];
				} response;
				int coid;
				
				while (!stop_requested) {
					int request_size;
					int r = mp.receive(fd, coid, &request, sizeof(request), request_size);
					if (r < 0) {
						log.error() << "receive error: " << r;
						break;
					}
					
					if (r == RRPS_OPEN) 
					{
						log.trace() << "client connected";
						mp.accept(fd, 1);
					}
					else if (r == RRPS_CLOSE)
					{
						log.trace() << "client disconnected";
					}
					else if (r == RRPS_MSG) {
						log.trace() << "message received, calling method...";
						int reply_size = call(request.type, request.data, (request_size - 2*sizeof(int)), response.data, sizeof(response.data));
						log.trace() << "done, replying... (" << (int)response.data[0] << ")";
						response.type = 0;
						mp.reply(fd, coid, &response, (reply_size + 2*sizeof(int)));
						log.trace() << "done";
					}
					else {
						log.warn() << "ignoring unknown message: " << r;
					}
				}
				
				mp.close(fd);
			}
	};
};
