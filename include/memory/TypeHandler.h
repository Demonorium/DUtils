#pragma once


#include <assert.h>

#include "..\Aliases.h"

DEMONORIUM_ALIASES;
DEMONORIUM_LOCAL_USE(demonorium::utils::templates);

namespace demonorium
{
	namespace memory
	{
		namespace
		{
			template<class T, bool destr = std::is_destructible<T>()>
			struct _destr {};

			template<class T>
			struct _destr<T, true> {
				static constexpr void destr(T* object) {
					object->~T();
				}
			};
			template<class T>
			struct _destr<T, false> {
				static constexpr void destr(T* object) noexcept {}
			};
		}


		template<class T>
		void utilize(void* object) {
			_destr<T>::destr((T*)object);
		}
		
		template<class T>
		void utilize(T& object) {
			_destr<T>::destr(&object);
		}

#ifndef DEMONORIUM_UNSAFE
		namespace
		{
			//UNSAFE!
			template<class T, class ... ARGS>
			void construct(T& object, ARGS&& ... args) {
				::new (&object) T(std::forward<ARGS>(args)...);
			}
		}
		
#elif DEMONORIUM_UNSAFE
		//UNSAFE but hidden
		template<class T, class ... ARGS>
		void construct(T& object, ARGS&& ... args) {
			::new (&object) T(std::forward<ARGS>(args)...);
		}
#endif
		namespace
		{
			//Max size of type
			template<class ... ARGS>
			constexpr size_t typesSize = sizeof(selectByCondition<conditions::haveMaxSize, undeclared_type, ARGS...>);
		}

		namespace
		{
			constexpr unsigned int greatest_common_divisor(size_t a, size_t b) {
				if (a % b == 0)
					return b;
				if (b % a == 0)
					return a;
				if (a > b)
					return greatest_common_divisor(a % b, b);
				return greatest_common_divisor(a, b % a);
			}

			constexpr size_t least_common_multiple(size_t a, size_t b) {
				return (a * b) / greatest_common_divisor(a, b);
			}
			
			template<class T, class ... Args>
			struct AlignHelper {
				constexpr size_t align = least_common_multiple(alignof(T), AlignHelper<Args...>::align);
			};
			template<class T>
			struct AlignHelper<T> {
				constexpr size_t align = alignof(T);
			};
		}

		template<class T, class ... TYPES>
		class TypeHandler {
			template<class _T>
			class _TypePointer {
			public:
				constexpr _TypePointer() noexcept = default;
			};
		public:
			//Class used to specify type
			template<class _T>
			using TypePointer = const _TypePointer<_T>&;

			template<class _T>
			static constexpr _TypePointer<_T> Type = _TypePointer<_T>();


			static constexpr size_t BYTE_LENGTH = typesSize<T, TYPES...>;
			static constexpr size_t TYPE_COUNT = count<T, TYPES...>;

			template<class D, class ... ARGS>
			constexpr TypeHandler(TypePointer<D> _p, ARGS&& ... args);
			constexpr TypeHandler() noexcept;

			//Safe get
			template<class D>
			constexpr D& get();

			//Safe get (const)
			template<class D>
			constexpr const D& get() const;

			//Safe set
			template<class D, class ... ARGS>
			constexpr void set(TypePointer<D> _p, ARGS&&...);

			//Safe set (move)
			template<class D>
			constexpr void set(D&& obj);

			//Check type in container
			template<class D>
			constexpr bool isType() const noexcept;
			
			//Check if container may contain type D
			template<class D>
			constexpr bool contains() const noexcept;

			//Free container
			constexpr void remove();

			//Check if other TypeHandler may contains same type
			constexpr bool sameType(const TypeHandler<T, TYPES...>& other) const noexcept;

		private:
			template<class D>
			constexpr static bool _contains() noexcept;

			template<class D, class ... ARGS>
			constexpr void _utilize();

			constexpr bool isType(const size_t id) const noexcept;

			alignas(AlignHelper<T, TYPES...>::align) byte	m_container[BYTE_LENGTH];
			size_t	m_type;
		};


		template<class T, class ...TYPES>
		template<class D, class ...ARGS>
		inline constexpr TypeHandler<T, TYPES...>::TypeHandler(TypePointer<D> _p, ARGS&& ...args) :
			m_type(ERROR_MEMORY) {
			set<D, ARGS...>(_p, std::forward<ARGS>(args) ...);
		}

		template<class T, class ...TYPES>
		template<class D>
		inline constexpr D& TypeHandler<T, TYPES...>::get() {
			static_assert(_contains<D>(), "No such type in pack");
			assert(isType<D>());

			return (D&)m_container;
		}
		template<class T, class ...TYPES>
		template<class D>
		inline constexpr const D& TypeHandler<T, TYPES...>::get() const {
			static_assert(_contains<D>(), "No such type in pack");
			assert(isType<D>());

			return (D&)m_container;
		}
		
		template<class T, class ...TYPES>
		template<class D, class ...ARGS>
		inline constexpr void TypeHandler<T, TYPES...>::set(TypePointer<D> _p, ARGS&&... args) {
			static_assert(indexOf<D, T, TYPES...> != ERROR_MEMORY, "No such type in pack");
			
			remove();

			m_type = indexOf<D, T, TYPES...>;
			::new((void*)&m_container) D(std::forward<ARGS>(args) ...);
		}

		template<class T, class ...TYPES>
		template<class D>
		inline constexpr bool TypeHandler<T, TYPES...>::isType() const noexcept {
			return _contains<D>() && (m_type == indexOf<D, T, TYPES...>);
		}

		template<class T, class ...TYPES>
		template<class D>
		inline constexpr void TypeHandler<T, TYPES...>::set(D&& obj) {
			get<D>() = obj;
		}

		template<class T, class ...TYPES>
		template<class D>
		inline constexpr bool TypeHandler<T, TYPES...>::contains() const noexcept {
			return _contains<D>();
		}

		template<class T, class ... TYPES>
		template<class D>
		inline constexpr bool TypeHandler<T, TYPES...>::_contains() noexcept {
			return utils::templates::contains<D, T, TYPES...>;
		}

		template<class T, class ...TYPES>
		inline constexpr TypeHandler<T, TYPES...>::TypeHandler() noexcept :
			m_type(ERROR_MEMORY) {}

		template<class T, class ...TYPES>
		inline constexpr bool TypeHandler<T, TYPES...>::isType(const size_t id) const noexcept {
			return m_type == id;
		}

		template<class T, class ...TYPES>
		inline constexpr void TypeHandler<T, TYPES...>::remove() {
			if (m_type != ERROR_MEMORY)
				_utilize<T, TYPES...>();
		}

		template<class T, class ...TYPES>
		inline constexpr bool TypeHandler<T, TYPES...>::sameType(const TypeHandler<T, TYPES...>& other) const noexcept {
			return isType(other.m_type);
		}


		template<class T, class ... TYPES>
		template<class D, class ... ARGS>
		inline constexpr void TypeHandler<T, TYPES...>::_utilize() {
			if ((--m_type) == ERROR_MEMORY)
				utilize<D>(&m_container);
			else
				_utilize<TYPES...>();
		}
	}
}