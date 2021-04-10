#pragma once



#include "_aDecl/Ddef.h"

DEMONORIUM_ALIASES;


namespace demonorium
{
	namespace utils
	{
		namespace templates
		{
			namespace
			{
				template<bool value, class T1, class T2>
				struct _condition {};

				template<class T1, class T2>
				struct _condition<true, T1, T2> {
					using type = T1;
				};

				template<class T1, class T2>
				struct _condition<false, T1, T2> {
					using type = T2;
				};
			}

			template<bool cond, class T1, class T2>
			using condition = typename _condition<cond, T1, T2>::type;


			namespace
			{
				template<class T, class ... LIST>
				struct __counter {
					static constexpr size_t count = 1 + __counter<LIST...>::count;
				};

				template<class T>
				struct __counter<T> {
					static constexpr size_t count = 1;
				};

				template<class ... LIST>
				struct _count {
					static constexpr size_t count = __counter<LIST...>::count;
				};

				template<>
				struct _count<> {
					static constexpr size_t count = 0;
				};
			}
			
			template<class ... LIST>
			constexpr size_t count = _count<LIST ...>::count;


			namespace
			{
				template<size_t i, size_t MAX>
				constexpr bool __inList = (i >= 0) && (i < MAX);

				template<int i, class T, class ... LIST>
				struct __select {
					using type = condition<i == 0, T, typename __select<i - 1, LIST...>::type>;
				};

				template<int i, class T>
				struct __select<i, T> {
					using type = T;
				};

				template<bool correct, size_t i, class ... LIST>
				struct __selectProxy {
					using type = typename __select<i, LIST...>::type;
				};

				template<size_t i, class ... LIST>
				struct __selectProxy<false, i, LIST...> {
					using type = undeclared_type;
				};


				template<size_t i, class ... LIST>
				struct _select {
					using type = typename __selectProxy<__inList<i, count<LIST...>>, i, LIST...>::type;
				};
				
			}
			template<size_t i, class ...LIST>
			using select = typename _select<i, LIST...>::type;

			namespace
			{
				template<template<class T1, class T2> class COND, class DEFAULT, class T, class ... LIST>
				struct _selectFirst {
					using type = COND < T, typename _selectFirst<COND, DEFAULT, LIST...>::type>;
				};

				template<template<class T1, class T2> class COND, class DEFAULT, class T>
				struct _selectFirst<COND, DEFAULT, T> {
					using type = COND<T, DEFAULT>;
				};

				template<size_t size, template<class T1, class T2> class COND, class DEFAULT, class ... LIST>
				struct _selectFirstProxy {
					using type = typename _selectFirst<COND, DEFAULT, LIST...>::type;
				};

				template<template<class T1, class T2> class COND, class DEFAULT, class ... LIST>
				struct _selectFirstProxy<0, COND, DEFAULT, LIST...> {
					using type = DEFAULT;
				};
			}

			template<template<class T1, class T2> class COND, class DEFAULT, class ... LIST>
			using selectByCondition = typename _selectFirstProxy<count<LIST...>, COND, DEFAULT, LIST...>::type;

			

			namespace conditions
			{
				template<class T1, class T2>
				using declared = condition<isDeclared<T1>(), T1, T2>;

				template<class T1, class T2, size_t SIZE>
				using haveSize = condition<sizeof(T1) == SIZE, T1, T2>;
				
				template<size_t SIZE>
				struct haveSizeFactory {
					template<class T1, class T2>
					using type = haveSize<T1, T2, SIZE>;
				};

				template<class T1, class T2>
				using haveMaxSize = condition<sizeof(T1) >= sizeof(T2), T1, T2>;

				template<class T1, class T2>
				using haveMinSize = condition<sizeof(T1) <= sizeof(T2), T1, T2>;
			}
			DEMONORIUM_LOCAL_USE(conditions);
			
			namespace
			{
				template<class T, class ... LIST>
				struct __firstType {
					using type = T;
				};

				template<class ... LIST>
				struct _firstType {
					using type = typename __firstType<LIST...>::type;
				};

				template<>
				struct _firstType<> {
					using type = undeclared_type;
				};
			}

			template<class ... LIST>
			using firstType = typename _firstType<LIST...>::type;

			

			namespace
			{
				template<template<class ... ARGS> class OBJ, class T, class ... LIST>
				struct __setNextType {
					using type = OBJ<LIST...>;
				};

				template<template<class ... ARGS> class OBJ, class ... LIST>
				struct _setNextType {
					using type = typename __setNextType<OBJ, LIST...>::type;
				};

				template<template<class ... ARGS> class OBJ>
				struct _setNextType<OBJ> {
					using type = undeclared_type;
				};
			}

			template<template<class ... ARGS> class OBJ, class ... LIST>
			using nextTypes = typename _setNextType<OBJ, LIST...>::type;

			

			namespace
			{
				template<class F, class ... ARGS>
				struct _groupContains {
					template<class ... A>
					using _nc = _groupContains<F, A...>;

					static constexpr bool contains = std::is_same_v<F, firstType<ARGS...>> || nextTypes<_nc, ARGS...>::contains;
				};

				template<class F>
				struct _groupContains<F> {
					static constexpr bool contains = false;
				};
			}

			template<class F, class ... ARGS>
			constexpr bool contains = _groupContains<F, ARGS...>::contains;

			namespace
			{


				template<class F, size_t index, class ... ARGS>
				struct _groupIndex {
					template<class ... A>
					using _nc = _groupIndex<F, index + 1, A...>;

					static constexpr size_t getIndex() {
						if (std::is_same_v<F, firstType<ARGS...>>)
							return index;
						else
							return nextTypes<_nc, ARGS...>::getIndex();
					}
				};

				template<class F, size_t index>
				struct _groupIndex<F, index> {
					static constexpr size_t getIndex() {
						return (size_t)-1;
					}
				};
			}
			template<class F, class ... ARGS>
			constexpr size_t indexOf = _groupIndex<F, 0, ARGS...>::getIndex();


			namespace
			{
				template<template<class... VS> class T, class ... ARGS>
				struct _reverse {
					template<class ... A>
					using nT = T<A..., firstType<ARGS...>>;
					
					template<class ... A>
					using _nc = _reverse<nT, A...>;

					using type = typename nextTypes<_nc, ARGS...>::type;
				};

				template<template<class... VS> class T>
				struct _reverse<T> {
					using type = T<>;
				};
			}

			template<template<class... VS> class T, class ... ARGS>
			using reverse = _reverse<T, ARGS...>;
		}
	}
}