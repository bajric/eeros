#ifndef ORG_EEROS_IPC_MEHTODCALLER_HPP_
#define ORG_EEROS_IPC_MEHTODCALLER_HPP_

#include <eeros/ipc/apply.hpp>
#include <eeros/ipc/Caller.hpp>

namespace eeros {
	namespace ipc {

		template < typename TClass, typename TReturn, typename ... TArgs >
		class MethodCaller : public Caller {
		public:
			using Request = std::tuple<TArgs...>;
			using Response = std::tuple<TReturn>;
			using Method = TReturn(TClass::*)(TArgs ...);
			
			MethodCaller(TClass *object, Method method) :
				object(object), method(method) { }
			
			virtual int call(const void *request, int request_size, void *response, int response_size) {	
				if (request_size != sizeof(Request)) throw 1;
				if (response_size < sizeof(Response)) throw 2;
				
				auto *request_msg = reinterpret_cast<const Request*>(request);
				auto *response_msg = reinterpret_cast<Response*>(response);

				std::get<0>(*response_msg) = apply(object, method, *request_msg);
				return sizeof(Response);
			}
			
			TClass *object;
			Method method;
		};


		template < typename TClass, typename ... TArgs >
		class MethodCaller< TClass, void, TArgs... > : public Caller {
		public:
			using Request = std::tuple<TArgs...>;
			using Method = void(TClass::*)(TArgs ...);
			
			MethodCaller(TClass *object, Method method) :
				object(object), method(method) { }
			
			virtual int call(const void *request, int request_size, void *response, int response_size) {
				if (request_size != sizeof(Request)) throw request_size;
				
				auto *request_msg = reinterpret_cast<const Request*>(request);
				
				apply(object, method, *request_msg);
				return 0;
			}
			
			TClass *object;
			Method method;
		};

	};
};

#endif // ORG_EEROS_IPC_MEHTODCALLER_HPP_
