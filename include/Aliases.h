#pragma once

#include <limits>

#include "_aDecl/Ddef.h"
#include "Templates.h"

DEMONORIUM_LOCAL_USE(demonorium::utils::templates)

#define DEMONORIUM_LITDELC(name, type, parameter) constexpr type operator"" name(unsigned long long int parameter)
#define DEMONORIUM_SIMPLE_FIND(container, method, data, it_name) auto (it_name) = (container) . method (data) if ((it_name) != (container).end())


#ifndef DEMONORIUM_DECLARE_PLATFORM_TYPES
#define DEMONORIUM_DECLARE_PLATFORM_TYPES

namespace demonorium
{
	namespace aliases
	{
		namespace type_finder
		{
			template<size_t SIZE, class T, class ... ARGS>
			using typeBySize = selectByCondition<typename conditions::haveSizeFactory<SIZE>::type, undeclared_type, T, ARGS...>;

			template<size_t size>
			using signedBySize = typeBySize<size,
				signed char,
				signed short,
				signed int,
				signed long,
				signed long long>;

			template<size_t size>
			using unsignedBySize = typeBySize<size,
				unsigned char,
				unsigned short,
				unsigned int,
				unsigned long,
				unsigned long long>;

			template<size_t size>
			using realBySize = typeBySize<size,
				float,
				double,
				long double>;

			template<class ... ARGS>
			using declaredType = selectByCondition<conditions::declared, undeclared_type, ARGS...>;

			template<class T>
			using signedSynonym = std::make_signed_t<T>;

			template<class T>
			using unsignedSynonym = std::make_unsigned_t<T>;


			template<class F>
			constexpr bool isSigned = std::is_signed_v<F>;

			template<class F>
			constexpr bool isUnsigned = std::is_unsigned_v<F>;

			template<class F>
			constexpr bool isReal = std::is_floating_point_v<F>;

			template<class F>
			constexpr bool isInteger = std::numeric_limits<F>::is_integer;
		}

		using signed_size_t = type_finder::signedSynonym<size_t>;
		
		using int8 =  type_finder::signedBySize<1>;
		using int16 = type_finder::signedBySize<2>;
		using int32 = type_finder::signedBySize<4>;
		using int64 = type_finder::signedBySize<8>;

		using uint8 =  type_finder::unsignedBySize<1>;
		using uint16 = type_finder::unsignedBySize<2>;
		using uint32 = type_finder::unsignedBySize<4>;
		using uint64 = type_finder::unsignedBySize<8>;

		using uint = type_finder::unsignedSynonym<int>;

		using max_int = type_finder::declaredType<
			type_finder::signedBySize<8>,
			type_finder::signedBySize<4>,
			type_finder::signedBySize<2>,
			type_finder::signedBySize<1>
		>;
		using max_uint = type_finder::declaredType<
			type_finder::unsignedBySize<8>,
			type_finder::unsignedBySize<4>,
			type_finder::unsignedBySize<2>,
			type_finder::unsignedBySize<1>
		>;

		using max_real = type_finder::declaredType<
			type_finder::realBySize<10>,
			type_finder::realBySize<8>,
			type_finder::realBySize<6>,
			type_finder::realBySize<4>
		>;

		//using real = type_finder::declaredType<type_finder::realBySize<8>, type_finder::realBySize<4>>;

		constexpr bool isX64 = isDeclared<uint64>();
		constexpr bool isX32 = isDeclared<uint32>() && !isX64;
		constexpr bool isX16 = isDeclared<uint16>() && !isX64 && !isX32;
		
		constexpr bool longDoubleAvailable = sizeof(long double) > 8;

		using byte = uint8;

		namespace
		{
			template<class TO, class FROM, bool tsign = type_finder::isSigned<TO>, bool fsing = type_finder::isSigned<FROM>>
			struct _cast_help {};

			template<class TO, class FROM, bool fsign>
			struct _cast_help<TO, FROM, false, fsign> {
				static auto&& cast(FROM&& value) {
					return (TO)value;
				}
			};

			template<class TO, class FROM>
			struct _cast_help<TO, FROM, false, true> {
				static auto&& cast(FROM&& value) {
					return (TO)value;
				}
			};

			template<class TO, class FROM>
			struct _cast_help<TO, FROM, true, true> {
				static auto&& cast(FROM&& value) {
					return (TO)(type_finder::unsignedSynonym<TO>)value;
				}
			};
		}

		template<class TO, class FROM>
		constexpr auto&& byte_cast(FROM&& value) {
			return _cast_help<TO, FROM>::cast(std::forward<FROM>(value));
		}
	}
}
#endif //DEMONORIUM_DECLARE_PLATFORM_TYPES

#include <complex>

#ifndef DEMONORIUM_DECLARE_MATHA
#define DEMONORIUM_DECLARE_MATHA
namespace demonorium
{
	namespace math
	{
		using Angle = aliases::max_real;
		using Angled = double;
		using Anglef = float;
		
		using Complex = std::complex<double>;
		using Complexf = std::complex<float>;

		
		
		namespace constants
		{
			constexpr auto E = 2.71828182845904523536;			// e
			constexpr auto LOG2E = 1.44269504088896340736;		// log2(e)
			constexpr auto LOG10E = 0.434294481903251827651;	// log10(e)
			constexpr auto LN2 = 0.693147180559945309417;		// ln(2)
			constexpr auto LN10 = 2.30258509299404568402;		// ln(10)
			constexpr auto PI = 3.14159265358979323846;			// pi
			constexpr auto PI2 = 6.28318530717958647692;		// 2*pi
			constexpr auto PI_D2 = 1.57079632679489661923;		// pi/2
			constexpr auto PI_D4 = 0.785398163397448309616;		// pi/4
			constexpr auto D1_PI = 0.318309886183790671538;		// 1/pi
			constexpr auto D2_PI = 0.636619772367581343076;		// 2/pi
			constexpr auto D2_SQRTPI = 1.12837916709551257390;  // 2/sqrt(pi)
			constexpr auto SQRT2 = 1.41421356237309504880;		// sqrt(2)
			constexpr auto SQRT1_D2 = 0.707106781186547524401;	// 1/sqrt(2)
		}
	}
}
#endif


#ifndef DEMONORIUM_DECLARE_MEMA
#define DEMONORIUM_DECLARE_MEMA
namespace demonorium
{
	namespace memory
	{
		using address = size_t;
		using signed_address = type_finder::signedSynonym<address>;
		constexpr size_t MAX_MEMORY = std::numeric_limits<size_t>::max();
		constexpr size_t ERROR_MEMORY = MAX_MEMORY;

		namespace memory_literals
		{
			namespace
			{
				using ulli = unsigned long long int;
			}

			DEMONORIUM_LITDELC(_B, ulli, size) {
				return size;
			}
			DEMONORIUM_LITDELC(_KB, ulli, size) {
				return size << 10;
			}
			DEMONORIUM_LITDELC(_MB, ulli, size) {
				return size << 20;
			}
			DEMONORIUM_LITDELC(_GB, ulli, size) {
				return size << 30;
			}
			DEMONORIUM_LITDELC(_TB, ulli, size) {
				return size << 40;
			}
			DEMONORIUM_LITDELC(_PB, ulli, size) {
				return size << 50;
			}
			DEMONORIUM_LITDELC(_EB, ulli, size) {
				return size << 60;
			}
		}

		namespace memory_constant
		{
			DEMONORIUM_LOCAL_USE(memory_literals);

			constexpr auto BYTE = 1_B;
			constexpr auto KILOBYTE = 1_KB;
			constexpr auto MEGABYTE = 1_MB;
			constexpr auto GIGABYTE = 1_GB;
			constexpr auto TERABYTE = 1_TB;
			constexpr auto PETABYTE = 1_PB;
			constexpr auto EXABYTE = 1_EB;
			constexpr type_finder::signedSynonym<size_t> address_size = sizeof(address);
		}

		namespace memory_casts
		{
			template<class T>
			constexpr address address_cast(const T* pointer) {
				return reinterpret_cast<address>(pointer);
			}

			constexpr address address_cast(std::nullptr_t pointer) {
				return reinterpret_cast<address>(nullptr);
			}
			///unsigned cast
			constexpr address address_cast(unsigned char pointer) {
				return static_cast<address>(pointer);
			}

			constexpr address address_cast(unsigned short pointer) {
				return static_cast<address>(pointer);
			}

			constexpr address address_cast(unsigned int pointer) {
				return static_cast<address>(pointer);
			}

			constexpr address address_cast(unsigned long long pointer) {
				return static_cast<address>(pointer);
			}
			
			static void* as_pointer(address pointer) {
				return reinterpret_cast<void*>(pointer);
			}

			template<class T>
			T& as_reference(void* pointer) {
				return *static_cast<T*>(pointer);
			}

			template<class T>
			T& as_reference(address pointer) {
				return as_reference<T>(as_pointer(pointer));
			}

			template<class T>
			const T& as_reference(const void* pointer) {
				return *reinterpret_cast<const T*>(pointer);
			}

			constexpr address address_cast(signed char pointer) {
				return static_cast<address>(pointer);
			}

			constexpr address address_cast(signed short pointer) {
				return static_cast<address>(pointer);
			}

			constexpr address address_cast(signed int pointer) {
				return static_cast<address>(pointer);
			}

			constexpr address address_cast(signed long long pointer) {
				return static_cast<address>(pointer);
			}
		}

		namespace memory_operations
		{
			template<class T, class INT>
			constexpr T* shift(T* pointer, INT value) {
				return reinterpret_cast<T*>(reinterpret_cast<INT>(pointer) + value);
			}

			template<class T>
			constexpr void write(void* target, const T& value) {
				memory_casts::as_reference<T>(target) = value;
			}
		}


		namespace memory_declarations
		{
			using namespace memory_literals;
			using namespace memory_constant;
			using namespace memory_casts;
			using namespace memory_operations;
		}
	}
}
#endif //DEMONORIUM_DECLARE_MEMA