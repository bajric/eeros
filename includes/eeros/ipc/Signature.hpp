#ifndef ORG_EEROS_IPC_SIGNATURE_HPP_
#define ORG_EEROS_IPC_SIGNATURE_HPP_

#include <cstdint>
#include <tuple>

namespace eeros {
	namespace ipc {

		struct ParameterType {
			uint8_t type;
			uint8_t count;
			
			template < int N, typename ... TArgs >
			static void setup_args(std::array<ParameterType, N> &args) {
				// TODO: setup args
			}
		};
		
		struct Signature {
			std::size_t return_size;
			std::size_t arg_size;
			std::size_t arg_count;
			std::array<ParameterType, 32> args;
		};

		template < typename TReturn, typename ... TArgs >
		class create_signature {
		public:
			static constexpr std::size_t return_size = sizeof(TReturn);
			static constexpr std::size_t arg_size = sizeof(std::tuple<TArgs...>);
			static constexpr std::size_t arg_count = sizeof...(TArgs);
			
			static Signature value() {
				Signature s;
				s.return_size = create_signature::return_size;
				s.arg_size = create_signature::arg_size;
				s.arg_count = create_signature::arg_count;
				ParameterType::setup_args<32, TArgs...>(s.args);
				return s;
			}
		};

		template < typename ... TArgs >
		class create_signature<void,TArgs...> {
		public:
			static constexpr std::size_t return_size = 0;
			static constexpr std::size_t arg_size = sizeof(std::tuple<TArgs...>);
			static constexpr std::size_t arg_count = sizeof...(TArgs);
			
			static Signature value() {
				Signature s;
				s.return_size = create_signature::return_size;
				s.arg_size = create_signature::arg_size;
				s.arg_count = create_signature::arg_count;
				ParameterType::setup_args<32, TArgs...>(s.args);
				return s;
			}
		};
	};
};

#endif // ORG_EEROS_IPC_SIGNATURE_HPP_
