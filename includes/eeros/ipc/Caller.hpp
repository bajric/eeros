#ifndef ORG_EEROS_IPC_CALLER_HPP_
#define ORG_EEROS_IPC_CALLER_HPP_

#include <eeros/ipc/apply.hpp>

namespace eeros {
	namespace ipc {

		class Caller {
		public:
			virtual int call(const void *request, int request_size, void *response, int response_size) = 0;
			
			template < typename TArgs, typename TResult >
			inline int call(const TArgs &args, TResult &result) {
				return call(reinterpret_cast<const void*>(&args), sizeof(args), reinterpret_cast<void*>(&result), sizeof(result));
			}
			
			template < typename TArgs >
			inline int call(const TArgs &args) {
				return call(reinterpret_cast<const void*>(&args), sizeof(args), nullptr, 0);
			}
		};

	};
};

#endif // ORG_EEROS_IPC_CALLER_HPP_
