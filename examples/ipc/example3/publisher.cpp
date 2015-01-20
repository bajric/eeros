#include <iostream>

#include <eeros/control/SignalLogger.hpp>
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
	
	std::cout << "eeros ipc example 3: publisher" << std::endl;
	
	eeros::ipc::Server server("example3");
	
	eeros::control::SignalLogger slogger("example3-logger");
	eeros::control::Signal<double> s1;
	eeros::control::Signal<int> s2;
	
	slogger.addSignal(s1);
	slogger.addSignal(s2);
	
	server.add_method(eeros::control::SignalLoggerInterface::subscribeID, "subscribe", &slogger, &eeros::control::SignalLogger::subscribe);
	
	for (int i = 0; i < 100; i++) {
		s1.setValue(i + 0.1);
		s1.setTimestamp(i);
		s2.setValue(i + 1);
		s2.setTimestamp(i + 2);
		slogger.run();
		sleep(1);
	}
	
	std::cout << "stopping server" << std::endl;
	server.stop();
	server.join();
	
	std::cout << "example finished" << std::endl;
	return 0;
}
