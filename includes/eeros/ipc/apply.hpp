#ifndef ORG_EEROS_IPC_APPLY_HPP_
#define ORG_EEROS_IPC_APPLY_HPP_

#include <tuple>

namespace eeros {
	namespace ipc {

		template < int N >
		struct apply_func {

			template <	typename TReturn,
						typename ... TArgsFunction,
						typename ... TArgsTuple,
						typename ... TArgs >
			
			static inline TReturn execute(	TReturn(*function)(TArgsFunction...),
											const std::tuple<TArgsTuple...>& args_tuple,
											TArgs ... args_mapped )
			{
				return apply_func<N-1>::execute(	function,
												args_tuple,
												std::get<N-1>(args_tuple),
												args_mapped... );
			}
		};

		template < >
		struct apply_func<0> {
			
			template <	typename TReturn,
						typename ... TArgsFunction,
						typename ... TArgsTuple,
						typename ... TArgs >
						
			static inline TReturn execute(	TReturn(*function)(TArgsFunction...),
												const std::tuple<TArgsTuple...>& args_tuple,
												TArgs ... args_mapped )
			{
				return (*function)(args_mapped...);
			}
		};


		template < int N >
		struct apply_void_func {
			
			template <	typename ... TArgsFunction,
						typename ... TArgsTuple,
						typename ... TArgs >
			
			static inline void execute(	void(*function)(TArgsFunction...),
										const std::tuple<TArgsTuple...>& args_tuple,
										TArgs ... args_mapped )
			{
				apply_void_func<N-1>::execute(	function,
												args_tuple,
												std::get<N-1>(args_tuple),
												args_mapped... );
			}
		};

		template < >
		struct apply_void_func<0> {
			
			template <	typename ... TArgsFunction,
						typename ... TArgsTuple,
						typename ... TArgs >
						
			static inline void execute(	void(*function)(TArgsFunction...),
										const std::tuple<TArgsTuple...>& args_tuple,
										TArgs ... args_mapped )
			{
				(*function)(args_mapped...);
			}
		};



		template < int N >
		struct apply_method {
			
			template <	typename TClass,
						typename TReturn,
						typename ... TArgsMethod,
						typename ... TArgsTuple,
						typename ... TArgs >
			
			static inline TReturn execute(	TClass *object,
											TReturn(TClass::*method)(TArgsMethod...),
											const std::tuple<TArgsTuple...>& args_tuple,
											TArgs ... args_mapped )
			{
				return apply_method<N-1>::execute(	object,
													method,
													args_tuple,
													std::get<N-1>(args_tuple),
													args_mapped... );
			}
		};

		template < >
		struct apply_method<0> {
			
			template <	typename TClass,
						typename TReturn,
						typename ... TArgsMethod,
						typename ... TArgsTuple,
						typename ... TArgs >
						
			static inline TReturn execute(	TClass *object,
												TReturn(TClass::*method)(TArgsMethod...),
												const std::tuple<TArgsTuple...>& args_tuple,
												TArgs ... args_mapped )
			{
				return (object->*method)(args_mapped...);
			}
		};



		template < int N >
		struct apply_void_method {
			
			template <	typename TClass,
						typename ... TArgsMethod,
						typename ... TArgsTuple,
						typename ... TArgs >
			
			static inline void execute(	TClass *object,
										void(TClass::*method)(TArgsMethod...),
										const std::tuple<TArgsTuple...>& args_tuple,
										TArgs ... args_mapped )
			{
				apply_void_method<N-1>::execute(	object,
												method,
												args_tuple,
												std::get<N-1>(args_tuple),
												args_mapped... );
			}
		};

		template < >
		struct apply_void_method<0> {
			
			template <	typename TClass,
						typename ... TArgsMethod,
						typename ... TArgsTuple,
						typename ... TArgs >
						
			static inline void execute(	TClass *object,
										void(TClass::*method)(TArgsMethod...),
										const std::tuple<TArgsTuple...>& args_tuple,
										TArgs ... args_mapped )
			{
				(object->*method)(args_mapped...);
			}
		};


		template < typename TReturn, typename ... TArgs >
		TReturn apply(TReturn(*function)(TArgs...), const std::tuple<TArgs...> &args) {
			return apply_func<sizeof...(TArgs)>::execute(function, args);
		}

		template < typename ... TArgs >
		void apply(void(*function)(TArgs...), const std::tuple<TArgs...> &args) {
			apply_void_func<sizeof...(TArgs)>::execute(function, args);
		}

		template < typename TClass, typename TReturn, typename ... TArgs >
		TReturn apply(TClass *object, TReturn(TClass::*method)(TArgs...), const std::tuple<TArgs...> &args) {
			return apply_method<sizeof...(TArgs)>::execute(object, method, args);
		}

		template < typename TClass, typename ... TArgs >
		void apply(TClass *object, void(TClass::*method)(TArgs...), const std::tuple<TArgs...> &args) {
			apply_void_method<sizeof...(TArgs)>::execute(object, method, args);
		}

	};
};

#endif // ORG_EEROS_IPC_APPLY_HPP_
