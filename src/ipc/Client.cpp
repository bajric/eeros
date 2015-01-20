#include <eeros/ipc/Client.hpp>
#include "mpipc_ioctl.h"

#include <string>

namespace eeros {
	namespace ipc {

			Client::Client(MessagePassing &mp) : mp(mp) { }
			Client::Client() : mp(MessagePassing::get_default_impl()) { }
			
			void Client::connect(const char *name) {
				std::string n("/dev/mp-");
				n = n + name;
				fd = mp.open(n.c_str());
				if (fd < 0) throw 0; // TODO
			}
			
			void Client::disconnect() {
				mp.close(fd);
			}
	};
};
