#pragma once

#include <numeric>

#include "../Aliases.h"

namespace demonorium
{
	namespace utils
	{
		template<class T, size_t Space = 0>
		class NullableResult {
			T m_result;
		public:
			static const T NULLVALUE;
			
			NullableResult(const T& result);
			NullableResult(T&& result) noexcept;
			NullableResult(const NullableResult& result);
			NullableResult(NullableResult&& result) noexcept;
			~NullableResult() = default;
			
			NullableResult& operator= (const T& result);
			NullableResult& operator= (T&& result);
			NullableResult& operator= (NullableResult&& result);
			NullableResult& operator=(NullableResult& result);

			T& get();
			const T& get() const;
			void set(const T& result);
			void set(NullableResult& result);
			void set(T&& result);
			void set(NullableResult&& result);

			bool isNull() const;
			
			bool operator== (const T& result) const;
			bool operator==(const NullableResult& result) const;
			bool operator!= (const T& result) const;
			bool operator!= (const NullableResult& result) const;

			bool operator< (const T& result) const;
			bool operator< (const NullableResult& result) const;
			bool operator<= (const T& result) const;
			bool operator<= (const NullableResult& result) const;

			bool operator> (const T& result) const;
			bool operator> (const NullableResult& result) const;
			bool operator>= (const T& result) const;
			bool operator>= (const NullableResult& result) const;

			NullableResult operator+ (const T& result) const;
			NullableResult operator+ (const NullableResult& result) const;
			NullableResult operator- (const T& result) const;
			NullableResult operator- (const NullableResult& result) const;
			NullableResult operator* (const T& result) const;
			NullableResult operator* (const NullableResult& result) const;
			NullableResult operator/ (const T& result) const;
			NullableResult operator/ (const NullableResult& result) const;

			NullableResult operator& (const T& result) const;
			NullableResult operator& (const NullableResult& result) const;
			NullableResult operator| (const T& result) const;
			NullableResult operator| (const NullableResult& result) const;
			NullableResult operator^ (const T& result) const;
			NullableResult operator^ (const NullableResult& result) const;
			NullableResult operator<< (const T& result) const;
			NullableResult operator<< (const NullableResult& result) const;
			NullableResult operator>> (const T& result) const;
			NullableResult operator>> (const NullableResult& result) const;

			operator T&();
			operator bool() const;
			T* operator->();
		};

		template <class T, size_t space>
		NullableResult<T, space>::NullableResult(const T& result):
			m_result(result){
		}

		template <class T, size_t space>
		NullableResult<T, space>::NullableResult(T&& result) noexcept :
			m_result(result) {
		}

		template <class T, size_t space>
		NullableResult<T, space>::NullableResult(const NullableResult& result) :
			m_result(result.m_result) {
		}

		template <class T, size_t space>
		NullableResult<T, space>::NullableResult(NullableResult&& result) noexcept :
			m_result(std::move(result.m_result)) {
		}

		template <class T, size_t space>
		NullableResult<T, space>& NullableResult<T, space>::operator=(const T& result) {
			m_result = result;
		}

		template <class T, size_t space>
		NullableResult<T, space>& NullableResult<T, space>::operator=(T&& result) {
			m_result = result;
		}

		template <class T, size_t space>
		NullableResult<T, space>& NullableResult<T, space>::operator=(NullableResult&& result) {
			m_result = std::move(result.m_result);
		}

		template <class T, size_t space>
		NullableResult<T, space>& NullableResult<T, space>::operator=(NullableResult& result) {
			m_result = result.m_result;
		}

		template <class T, size_t space>
		T& NullableResult<T, space>::get() {
			return m_result;
		}

		template <class T, size_t space>
		const T& NullableResult<T, space>::get() const {
			return m_result;
		}

		template <class T, size_t space>
		void NullableResult<T, space>::set(const T& result) {
			m_result = result;
		}

		template <class T, size_t space>
		void NullableResult<T, space>::set(NullableResult& result) {
			m_result = result.m_result;
		}

		template <class T, size_t space>
		void NullableResult<T, space>::set(T&& result) {
			m_result = result;
		}

		template <class T, size_t space>
		void NullableResult<T, space>::set(NullableResult&& result) {
			m_result = result.m_result;
		}

		template <class T, size_t space>
		bool NullableResult<T, space>::isNull() const {
			return m_result == NULLVALUE;
		}

		template <class T, size_t space>
		bool NullableResult<T, space>::operator==(const T& result) const {
			return m_result == result;
		}

		template <class T, size_t space>
		bool NullableResult<T, space>::operator==(const NullableResult& result) const {
			return m_result == result.m_result;
		}

		template <class T, size_t space>
		bool NullableResult<T, space>::operator!=(const T& result) const {
			return m_result != result;
		}

		template <class T, size_t space>
		bool NullableResult<T, space>::operator!=(const NullableResult& result) const {
			return m_result != result.m_result;
		}

		template <class T, size_t space>
		bool NullableResult<T, space>::operator<(const T& result) const {
			return m_result < result;
		}

		template <class T, size_t space>
		bool NullableResult<T, space>::operator<(const NullableResult& result) const {
			return m_result < result.m_result;
		}

		template <class T, size_t space>
		bool NullableResult<T, space>::operator<=(const T& result) const {
			return m_result <= result;
		}

		template <class T, size_t space>
		bool NullableResult<T, space>::operator<=(const NullableResult& result) const {
			return m_result <= result.m_result;
		}

		template <class T, size_t space>
		bool NullableResult<T, space>::operator>(const T& result) const {
			return m_result > result;
		}

		template <class T, size_t space>
		bool NullableResult<T, space>::operator>(const NullableResult& result) const {
			return m_result > result.m_result;
		}

		template <class T, size_t space>
		bool NullableResult<T, space>::operator>=(const T& result) const {
			return m_result >= result;
		}

		template <class T, size_t space>
		bool NullableResult<T, space>::operator>=(const NullableResult& result) const {
			return m_result >= result.m_result;
		}

		template <class T, size_t space>
		NullableResult<T, space> NullableResult<T, space>::operator+(const T& result) const {
			return {m_result + result};
		}

		template <class T, size_t space>
		NullableResult<T, space> NullableResult<T, space>::operator+(const NullableResult& result) const {
			return { m_result + result.m_result };
		}

		template <class T, size_t space>
		NullableResult<T, space> NullableResult<T, space>::operator-(const T& result) const {
			return { m_result - result };
		}

		template <class T, size_t space>
		NullableResult<T, space> NullableResult<T, space>::operator-(const NullableResult& result) const {
			return { m_result - result.m_result };
		}

		template <class T, size_t space>
		NullableResult<T, space> NullableResult<T, space>::operator*(const T& result) const {
			return { m_result * result };
		}

		template <class T, size_t space>
		NullableResult<T, space> NullableResult<T, space>::operator*(const NullableResult& result) const {
			return { m_result * result.m_result };
		}

		template <class T, size_t space>
		NullableResult<T, space> NullableResult<T, space>::operator/(const T& result) const {
			return { m_result / result };
		}

		template <class T, size_t space>
		NullableResult<T, space> NullableResult<T, space>::operator/(const NullableResult& result) const {
			return { m_result / result.m_result };
		}

		template <class T, size_t space>
		NullableResult<T, space> NullableResult<T, space>::operator&(const T& result) const {
			return { m_result & result };
		}

		template <class T, size_t space>
		NullableResult<T, space> NullableResult<T, space>::operator&(const NullableResult& result) const {
			return { m_result & result.m_result };
		}

		template <class T, size_t space>
		NullableResult<T, space> NullableResult<T, space>::operator|(const T& result) const {
			return { m_result | result };
		}

		template <class T, size_t space>
		NullableResult<T, space> NullableResult<T, space>::operator|(const NullableResult& result) const {
			return { m_result | result.m_result };
		}

		template <class T, size_t space>
		NullableResult<T, space> NullableResult<T, space>::operator^(const T& result) const {
			return { m_result ^ result };
		}

		template <class T, size_t space>
		NullableResult<T, space> NullableResult<T, space>::operator^(const NullableResult& result) const {
			return { m_result ^ result.m_result };
		}

		template <class T, size_t space>
		NullableResult<T, space> NullableResult<T, space>::operator<<(const T& result) const {
			return { m_result << result };
		}

		template <class T, size_t space>
		NullableResult<T, space> NullableResult<T, space>::operator<<(const NullableResult& result) const {
			return { m_result << result.m_result };
		}

		template <class T, size_t space>
		NullableResult<T, space> NullableResult<T, space>::operator>>(const T& result) const {
			return { m_result >> result };
		}

		template <class T, size_t space>
		NullableResult<T, space> NullableResult<T, space>::operator>>(const NullableResult& result) const {
			return { m_result >> result.m_result };
		}

		template <class T, size_t space>
		NullableResult<T, space>::operator T&() {
			return m_result;
		}

		template <class T, size_t space>
		NullableResult<T, space>::operator bool() const {
			return !isNull();
		}

		template <class T, size_t space>
		T* NullableResult<T, space>::operator->() {
			return &m_result;
		}

		using index_t = NullableResult<size_t>;
		const size_t NullableResult<size_t>::NULLVALUE(std::numeric_limits<size_t>::max());

		using signed_index_t = NullableResult<signed_size_t>;
		const signed_size_t NullableResult<signed_size_t>::NULLVALUE(std::numeric_limits<signed_size_t>::min());
	}
}
