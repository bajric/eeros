#ifndef ORG_EEROS_IPC_SERVER_HPP_
#define ORG_EEROS_IPC_SERVER_HPP_

#include <map>
#include <memory>
#include <atomic>

#include <eeros/ipc/Caller.hpp>
#include <eeros/ipc/MethodCaller.hpp>
#include <eeros/ipc/Signature.hpp>

#include <eeros/core/Thread.hpp>

namespace eeros {
	namespace ipc {

		struct ServiceMethod {
			const char *name;
			std::shared_ptr<Caller> method;
			Signature signature;
		};

		class Server : public Thread {
		public:
			Server(const char *name);
			virtual ~Server();
			
			void stop();
			
			template < typename TClass, typename TReturn, typename ... TArgs >
			void add_method(int id, const char *name, TClass *object, TReturn(TClass::*method)(TArgs ...)) {
				ServiceMethod m;
				m.name = name;
				m.method = std::make_shared<MethodCaller<TClass, TReturn, TArgs...>>(object, method);
				m.signature = create_signature<TReturn,TArgs...>::value();
				methods[id] = m;
			}
			
			template < typename TClass, typename ... TArgs >
			void add_method(int id, const char *name, TClass *object, void(TClass::*method)(TArgs ...)) {
				ServiceMethod m;
				m.name = name;
				m.method = std::make_shared<MethodCaller<TClass, void, TArgs...>>(object, method);
				m.signature = create_signature<void,TArgs...>::value();
				methods[id] = m;
			}
			
			template < typename TArgs, typename TResult >
			inline int call(int id, const TArgs &args, TResult &result) {
				return methods[id].method->call(args, result);
			}
			
			template < typename TArgs >
			inline int call(int id, const TArgs &args) {
				return methods[id].method->call(args);
			}
			
			int call(int id, const void *args, int args_size, void *result, int result_size);
			
		private:
			virtual void run();
			
			const char *name;
			std::atomic<bool> stop_requested;
			std::map<int,ServiceMethod> methods;
		};
	};
};

#endif // ORG_EEROS_IPC_SERVER_HPP_
