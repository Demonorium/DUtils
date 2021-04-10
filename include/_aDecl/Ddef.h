#pragma once

#include <cstddef>
#include <type_traits>


#ifndef DEMONORIUM_LOCAL_USE
#define DEMONORIUM_LOCAL_USE(...) namespace { using namespace __VA_ARGS__ ; }
#endif //DEMONORIUM_LOCAL_USE

#ifndef DEMONORIUM_ALIASES
#define DEMONORIUM_ALIASES DEMONORIUM_LOCAL_USE(demonorium::aliases)
#endif //DEMONORIUM_ALIASES

namespace demonorium
{
	namespace aliases
	{
		using size_t = std::size_t;

		//Тип ошибки, процедуры поиска возвращают его, если не нашли
		struct undeclared_type {};

		template<class T>
		constexpr bool isDeclared() {
			return !std::is_same_v<T, undeclared_type>;
		}

		template<class T>
		constexpr bool is_declared = isDeclared<T>();
	}

}
