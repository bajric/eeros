#include <iostream>

#include <eeros/ipc/Client.hpp>
#include <eeros/control/SignalLogger.hpp>
#include <eeros/logger/Logger.hpp>
#include <eeros/logger/StreamLogWriter.hpp>
#include <eeros/core/SharedMemory.hpp>

#include <unistd.h>


class LoggerClient : public eeros::control::SignalLoggerInterface {
public:
	LoggerClient(eeros::ipc::Client &client) : client(client) { }
	virtual ~LoggerClient() { }
	
	virtual bool subscribe(int sigid) {
		bool result = false;
		client.call(subscribeID, sigid, result);
		return result;
	}
	
private:
	eeros::ipc::Client &client;
};



class Subscriber {
public:
	Subscriber(std::string name) : shmem("/" + name, 4096*1024) {
		mem = reinterpret_cast<mem_header*>(shmem.getMemoryPointer());
		data = reinterpret_cast<char*>(shmem.getMemoryPointer());
		data = &data[sizeof(mem_header)];
		size = 4096*1024 - sizeof(mem_header);
		read_ptr = mem->head_ptr.load();
	}
	
	bool read() {
		int head = mem->head_ptr.load();
		if (read_ptr == head) return false;
		int end = (head + size/2) % size;
		if (read_ptr < end && read_ptr >= head) {
			read_ptr = head;
			return false;
		}
		return true;
	}
	
	void next() {
		read_ptr += sizeof(int) + sizeof(int) + sizeof(timestamp_t) + getSize();
	}
	
	sigid_t getId() {
		int *id = reinterpret_cast<int*>(&data[read_ptr]);
		return *id;
	}
	
	int getSize()
	{
		int *id = reinterpret_cast<int*>(&data[read_ptr]);
		int *sig_size = id + 1;
		return *sig_size;
	}
	
	timestamp_t getTimestamp()
	{
		int *id = reinterpret_cast<int*>(&data[read_ptr]);
		int *sig_size = id + 1;
		timestamp_t *t = reinterpret_cast<timestamp_t*>(sig_size + 1);
		return *t;
	}
	
	template < typename T >
	T getValue()
	{
		int *id = reinterpret_cast<int*>(&data[read_ptr]);
		int *sig_size = id + 1;
		timestamp_t *t = reinterpret_cast<timestamp_t*>(sig_size + 1);
		T *v = reinterpret_cast<T*>(t + 1);
		return *v;
	}
	
private:
	int read_ptr;
	eeros::SharedMemory shmem;
	struct mem_header {
		std::atomic<int> head_ptr;
	} *mem;
	char *data;
	int size;
};


int main(int argc, char **argv)
{
	using namespace eeros::logger;
	using namespace eeros::ipc;
	
	StreamLogWriter w(std::cout);
	w.show();
	Logger<LogWriter>::setDefaultWriter(&w);
	
	std::cout << "eeros ipc example 3: subscriber" << std::endl;
	
	eeros::ipc::Client client;
	client.connect("example3");
	
	Subscriber subscriber("example3-logger");
	sigid_t subscripion_id = 65536;
	//sigid_t subscripion_id = 131072;

	
	LoggerClient lclient(client);
	if (!lclient.subscribe(subscripion_id)){
		std::cout << "subscription failed" << std::endl;
		//return 0;
	}
	
	while (true) {
		while (subscriber.read()) {
			sigid_t id = subscriber.getId();
			if (id == subscripion_id) {
				std::cout << "t = " << subscriber.getTimestamp() << " v = ";
				std::cout << subscriber.getValue<double>();
//				std::cout << subscriber.getValue<int>();
				std::cout << std::endl;
			}
			else {
				std::cout << "unknown signal received " << id << std::endl;
			}
			subscriber.next();
		}
		sleep(5);
	}

	client.disconnect();
	std::cout << "example finished" << std::endl;
	return 0;
}
