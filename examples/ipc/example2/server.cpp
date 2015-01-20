#include <iostream>

#include "PathPlanner.hpp"
#include <eeros/ipc/Server.hpp>
#include <eeros/logger/Logger.hpp>
#include <eeros/logger/StreamLogWriter.hpp>

#include <unistd.h>

int main(int argc, char **argv)
{
	using namespace eeros::logger;
	using namespace eeros::ipc;
	
	StreamLogWriter w(std::cout);
	w.show();
	Logger<LogWriter>::setDefaultWriter(&w);
	
	std::cout << "eeros ipc example 2: server" << std::endl;
	
	eeros::ipc::Server server("example2");
	PathPlanner pathPlanner(server);
	
	sleep(10);
	
	std::cout << "stopping server" << std::endl;
	server.stop();
	server.join();
	
	std::cout << "example finished" << std::endl;
	return 0;
}
