#include <iostream>
#include <eeros/ipc/apply.hpp>
#include <eeros/ipc/Server.hpp>
#include <eeros/ipc/Signature.hpp>
#include <eeros/logger/Logger.hpp>
#include <eeros/logger/StreamLogWriter.hpp>
#include <unistd.h>

void example1(int a, double b) {
	std::cout << "[example1]   int: " << a << "   double: " << b << std::endl;
}

void example2(int a, double b, const char *string) {
	std::cout << "[example2]   int: " << a << "   double: " << b << "    str:" << string << std::endl;
}


bool example3(int a, double b) {
	std::cout << "[example3]   int: " << a << "   double: " << b << std::endl;
	return ((a % 2) == 0);
}

double example4(int a, double b, const char *string) {
	std::cout << "[example4]   int: " << a << "   double: " << b << "    str:" << string << std::endl;
	return a * b;
}

class example_class {
public:
	void example5(int a, double b, const char *string) {
		std::cout << "[example5]   int: " << a << "   double: " << b << "    str:" << string << std::endl;
	}
	double example6(int a, double b, const char *string) {
		std::cout << "[example6]   int: " << a << "   double: " << b << "    str:" << string << std::endl;
		return a / b;
	}
};



class TestService {
public:
	using MoveRequest = std::tuple<double, double, double>;
	
	TestService(eeros::ipc::Server& server) {
		server.add_method(27, "sort", this, &TestService::sort);
		server.add_method(58, "move", this, &TestService::move);
	}
	
	void sort(double x, double y, double z) {
		std::cout << "service sort " << x << ", " << y << ", " << z << std::endl;
	}
	
	bool move(double x, double y, double z) {
		std::cout << "service move " << x << ", " << y << ", " << z  << std::endl;
		return (x > y);
	}
};



int main(int argc, char **argv)
{
	using namespace eeros::logger;
	using namespace eeros::ipc;
	
	StreamLogWriter w(std::cout);
	w.show();
	Logger<LogWriter>::setDefaultWriter(&w);
	
	std::cout << "eeros ipc example 1" << std::endl;
	
	std::tuple<int,double> args1(30,-0.5);
	apply(&example1, args1);
	
	std::tuple<int,double, const char*> args2(30,-0.2, "a b c d E F G");
	apply(&example2, args2);
	
	bool result3 = apply(&example3, args1);
	if (!result3) std::cout << "ERROR" << std::endl;
	std::cout << "result: " << result3 << std::endl;
	
	double result4 = apply(&example4, args2);
	if (result4 != -6) std::cout << "ERROR" << std::endl;
	std::cout << "result: " << result4 << std::endl;
	
	example_class t;
	
	apply(&t, &example_class::example5, args2);
	
	double result6 = apply(&t, &example_class::example6, args2);
	if (result6 != -150) std::cout << "ERROR" << std::endl;
	std::cout << "result: " << result6 << std::endl;
	
	eeros::ipc::Server server("test");
	sleep(1);
	TestService service(server);
	
	std::tuple<double,double,double> args( 44.1, 22.7, -33.9 );
	std::tuple<bool> result;
	
	server.call(27, args);
	server.call(58, args, result);
	std::cout << "result: " << std::get<0>(result) << std::endl;

	std::cout << "signature:" << std::endl;
	Signature s = create_signature<double, double, double>::value();
	std::cout << "return_size: " << s.return_size << std::endl;
	std::cout << "arg_size:    " << s.arg_size << std::endl;
	std::cout << "arg_count:   " << s.arg_count << std::endl;
	
	server.stop();
	server.join();
	
	std::cout << "example finished" << std::endl;
	return 0;
}
