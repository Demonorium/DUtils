#pragma once

#include <assert.h>

#include <functional>
#include "..\Aliases.h"

DEMONORIUM_ALIASES;
DEMONORIUM_LOCAL_USE(demonorium::memory);
DEMONORIUM_LOCAL_USE(memory_declarations);

namespace demonorium
{
	namespace utils
	{
		template<class T>
		class Grid {
			T* m_data;
			size_t m_w, m_h;
			
			T* subarray(size_t x);
			const T* subarray(size_t x) const;
		public:
			Grid(size_t w, size_t h);
			template<class ... ARGS>
			Grid(size_t w, size_t h, ARGS&& ... args);

			~Grid();

			bool inside(int x, int y) const;

			void	 set(size_t x, size_t y, const T& value);
			void	 set(size_t x, size_t y, T&& value);

			T&		 get(size_t x, size_t y);
			const T& get(size_t x, size_t y) const;

			void setCut(int x, int y, const T& value);
			void setCut(int x, int y, T&& value);

			void setCicle(size_t x, size_t y, const T& value);
			void setCicle(size_t x, size_t y, T&& value);

			T&	 getCicle(int x, int y);
			const T& getCicle(int x, int y) const;

			template<class shape>
			void apply(const shape& shape, std::function<void(T&)> funct);

			template<class shape>
			void fill(const shape& shape, const T& obj);

			template<class shape>
			void fill(const shape& shape, T&& obj);

			template<class shape>
			void applyCut(const shape& shape, std::function<void(T&)> funct);
			
			template<class shape>
			void applyCicle(const shape& shape, std::function<void(T&)> funct);

			template<class shape>
			void fillCicle(const shape& shape, const T& obj);

			template<class shape>
			void fillCicle(const shape& shape, T&& obj);


			T* operator[](size_t x);
			const T* operator[](size_t x) const;

			size_t getWidth() const;
			size_t getHeight() const;

			T* getRawData();
			void initWithRawData(T* arr, size_t w, size_t h);

		};

		template<class T>
		inline const T* Grid<T>::subarray(size_t x) const {
			return (T*)as_pointer(address_cast(m_data) + m_h * x);
		}
		template<class T>
		inline T* Grid<T>::subarray(size_t x) {
			return (T*)as_pointer(address_cast(m_data) + m_h * x);
		}
		template<class T>
		inline Grid<T>::Grid(size_t w, size_t h): m_data(nullptr) {
			initWithRawData(new T[w*h], w, h);
		}
		
		template<class T>
		inline Grid<T>::~Grid() {
			delete[] m_data;
		}
		template<class T>
		inline bool Grid<T>::inside(int x, int y) const {
			return ((0 <= x) && (x < m_w)) && ((0 <= y) && (y < m_h));
		}

		template<class T>
		inline void Grid<T>::set(size_t x, size_t y, const T& value) {
			assert(inside(x, y));
			get(x, y) = value;
		}
		template<class T>
		inline void Grid<T>::set(size_t x, size_t y, T&& value) {
			assert(inside(x, y));
			get(x, y) = value;
		}

		template<class T>
		inline T& Grid<T>::get(size_t x, size_t y) {
			assert(inside(x, y));
			return subarray(x)[y];
		}
		template<class T>
		inline const T& Grid<T>::get(size_t x, size_t y) const {
			assert(inside(x, y));
			return subarray(x)[y];
		}
		template<class T>
		inline void Grid<T>::setCut(int x, int y, const T& value) {
			if (inside(x, y)) {
				set(x, y, value);
			}
		}
		template<class T>
		inline void Grid<T>::setCut(int x, int y, T&& value) {
			if (inside(x, y)) {
				set(x, y, value);
			}
		}

		template<class T>
		inline void Grid<T>::setCicle(size_t x, size_t y, const T& value) {
			getCicle(x, y) = value;
		}

		template<class T>
		inline void Grid<T>::setCicle(size_t x, size_t y, T&& value) {
			getCicle(x, y) = value;
		}



		template<class T>
		inline T& Grid<T>::getCicle(int x, int y) {
			return get(x % m_w, y % m_h);
		}
		template<class T>
		inline const T& Grid<T>::getCicle(int x, int y) const {
			return get(x % m_w, y % m_h);
		}
		template<class T>
		inline T* Grid<T>::operator[](size_t x) {
			return subarray(x);
		}

		template<class T>
		inline const T* Grid<T>::operator[](size_t x) const {
			return subarray(x);
		}
		template<class T>
		inline size_t Grid<T>::getWidth() const {
			return m_w;
		}
		template<class T>
		inline size_t Grid<T>::getHeight() const {
			return m_h;
		}
		template<class T>
		inline T* Grid<T>::getRawData() {
			return m_data;
		}

		template<class T>
		inline void Grid<T>::initWithRawData(T* arr, size_t w, size_t h) {
			m_data = arr;
			m_w = w;
			m_h = h;
		}

		template<class T>
		template<class ...ARGS>
		inline Grid<T>::Grid(size_t w, size_t h, ARGS&& ...args): m_data(nullptr) {
			initWithRawData(new T[w * h](std::forward<ARGS>(args)...), w, h);
		}

		template<class T>
		template<class shape>
		inline void Grid<T>::apply(const shape& shape, std::function<void(T&)> funct) {
			for (const auto& point : shape)
				if (inside(point.x, point.y))
					funct(get(point.x, point.y));
		}

		template<class T>
		template<class shape>
		inline void Grid<T>::fill(const shape& shape, const T& obj) {
			apply(shape, [&obj](const T& o) { o = obj; });
		}

		template<class T>
		template<class shape>
		inline void Grid<T>::fill(const shape& shape, T&& obj) {
			apply(shape, [&obj](const T& o) { o = obj; });
		}

		template <class T>
		template <class shape>
		void Grid<T>::applyCut(const shape& shape, std::function<void(T&)> funct) {
			for (const auto& point : shape)
				if (inside(point.x, point.y))
					funct(get(point.x, point.y));
		}

		template<class T>
		template<class shape>
		inline void Grid<T>::applyCicle(const shape& shape, std::function<void(T&)> funct) {
			for (const auto& point : shape)
				funct(get(point.x % m_w, point.y % m_h));
		}

		template<class T>
		template<class shape>
		inline void Grid<T>::fillCicle(const shape& shape, const T& obj) {
			applyCicle(shape, [&obj](const T& o) { o = obj; });
		}

		template<class T>
		template<class shape>
		inline void Grid<T>::fillCicle(const shape& shape, T&& obj) {
			applyCicle(shape, [&obj](const T& o) { o = obj; });
		}
	}
}