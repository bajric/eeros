#ifndef ORG_EEROS_CONTROL_SIGNALLOGGER_HPP_
#define ORG_EEROS_CONTROL_SIGNALLOGGER_HPP_

#include <eeros/core/SharedMemory.hpp>
#include <eeros/control/Block.hpp>
#include <eeros/control/Signal.hpp>
#include <eeros/logger/Logger.hpp>
#include <eeros/logger/LogWriter.hpp>

#include <map>
#include <string>
#include <functional>
#include <atomic>


namespace eeros {
	namespace control {

		class SignalLoggerInterface {
		public:
			virtual ~SignalLoggerInterface() {}
			virtual bool subscribe(int sigid) = 0;
			
			static constexpr int subscribeID = 846;
		};
		
		struct SignalRef {
			std::function<int(void*)> fnc;
		};
		
		class SignalLogger : public Block, public SignalLoggerInterface {
		public:
			SignalLogger(std::string name) : shmem("/" + name, 4096*1024) {
				mem = reinterpret_cast<mem_header*>(shmem.getMemoryPointer());
				data = reinterpret_cast<char*>(shmem.getMemoryPointer());
				data = &data[sizeof(mem_header)];
				size = 4096*1024 - sizeof(mem_header);
			}
			virtual ~SignalLogger() { }
			
			template < typename T >
			void addSignal(Signal<T> &s) {
				log.trace() << "adding signal: " << (int)s.getId();
				available_signals[s.getId()] = SignalRef{ [&s] (void *m) -> int {
					int *id = reinterpret_cast<int*>(m);
					int *sig_size = id + 1;
					timestamp_t *t = reinterpret_cast<timestamp_t*>(sig_size + 1);
					T *v = reinterpret_cast<T*>(t + 1);
					*id = s.getId();
					*sig_size = sizeof(T);
					*t = s.getTimestamp();
					*v = s.getValue();
					return sizeof(int) + sizeof(int) + sizeof(timestamp_t) + sizeof(T);
				}};
			}
			
			virtual void run() {
				if (!available_signals.empty()) {
					int head = mem->head_ptr.load();
					for (auto s : subscribed_signals) {
						head += s.second.fnc(&data[head]);
						head %= size;
					}
					mem->head_ptr.store(head);
				}
			}
			
			virtual bool subscribe(int sigid) {
				auto i = available_signals.find(sigid);
				if (i == available_signals.end())
					return false;
				
				auto j = subscribed_signals.find(sigid);
				if (j == subscribed_signals.end())
					subscribed_signals[sigid] = i->second;
				
				return true;
			}
			
		private:
			SharedMemory shmem;
			std::map<sigid_t,SignalRef> available_signals;
			std::map<sigid_t,SignalRef> subscribed_signals;
			eeros::logger::Logger<eeros::logger::LogWriter> log;
			
			struct mem_header {
				std::atomic<int> head_ptr;
			} *mem;
			char *data;
			int size;
		};

	};
};

#endif // ORG_EEROS_CONTROL_SIGNALLOGGER_HPP_
