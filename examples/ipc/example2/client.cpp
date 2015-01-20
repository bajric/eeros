#include <iostream>

#include "MoveInterface.hpp"
#include <eeros/ipc/Client.hpp>
#include <eeros/logger/Logger.hpp>
#include <eeros/logger/StreamLogWriter.hpp>

#include <unistd.h>


class MoveClient : public MoveInterface {
public:
	MoveClient(eeros::ipc::Client &client) : client(client) { }
	virtual ~MoveClient() { }
	
	virtual void move(double position) {
		client.call(moveID, position);
	}
	
private:
	eeros::ipc::Client &client;
};



int main(int argc, char **argv)
{
	using namespace eeros::logger;
	using namespace eeros::ipc;
	
	StreamLogWriter w(std::cout);
	w.show();
	Logger<LogWriter>::setDefaultWriter(&w);
	
	std::cout << "eeros ipc example 2: client" << std::endl;
	
	eeros::ipc::Client client;
	client.connect("example2");
	
	MoveClient moveIPCInvoker(client);
	sleep(1);
	moveIPCInvoker.move(0.3);
	sleep(1);
	moveIPCInvoker.move(0.4);
	sleep(1);
	moveIPCInvoker.move(-2.3);
	sleep(1);
	
	client.disconnect();
	std::cout << "example finished" << std::endl;
	return 0;
}
