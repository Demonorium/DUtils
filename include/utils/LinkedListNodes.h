#pragma once

#include <xmemory>

namespace demonorium
{
	namespace utils
	{
		template<class T, bool selfdestruction = true>
		class BidirectionalNode {
		};

		template<class T>
		class BidirectionalNode<T, true> {
			using node = BidirectionalNode<T, true>;
			node* m_next;
			node* m_prev;

			void remove();
		public:
			T data;

			template<class ... ARGS>
			BidirectionalNode(node* next, node* prev, ARGS&& ...);

			template<class ... ARGS>
			BidirectionalNode(node* next, ARGS&& ...);

			template<class ... ARGS>
			BidirectionalNode(ARGS&& ...);

			virtual ~BidirectionalNode();

			node* getNext();
			node* getPrev();

			const node* getNext() const;
			const node* getPrev() const;

			template<class ... ARGS>
			node* allocNext(ARGS&&...);

			template<class ... ARGS>
			node* allocPrev(ARGS&&...);

			node* linkNext(node* next);
			node* linkPrev(node* prev);
			
			void unlink();
			node* unlinkForward();
			node* unlinkBackward();

			node* removeForward();
			node* removeBackward();
		};

		template<class T>
		class BidirectionalNode<T, false> {
			using node = BidirectionalNode<T, false>;
			node* m_next;
			node* m_prev;

			void remove();
		public:
			T data;

			template<class ... ARGS>
			BidirectionalNode(node* next, node* prev, ARGS&& ...);

			template<class ... ARGS>
			BidirectionalNode(node* next, ARGS&& ...);

			template<class ... ARGS>
			BidirectionalNode(ARGS&& ...);

			node* getNext();
			node* getPrev();

			const node* getNext() const;
			const node* getPrev() const;

			template<class ... ARGS>
			node* allocNext(ARGS&&...);

			template<class ... ARGS>
			node* allocPrev(ARGS&&...);

			node* linkNext(node* next);
			node* linkPrev(node* prev);

			void unlink();
			node* unlinkForward();
			node* unlinkBackward();

			node* removeForward();
			node* removeBackward();
		};

		template<class T, bool selfdestruction = true>
		class UnidirectionalNode {
		};


		template<class T>
		class UnidirectionalNode<T, true> {
			using node = UnidirectionalNode<T, true>;
			node* m_next;
		public:
			T data;

			template<class ... ARGS>
			UnidirectionalNode(node* next, ARGS&& ...);

			template<class ... ARGS>
			UnidirectionalNode(ARGS&& ...);

			virtual ~UnidirectionalNode();

			node* getNext();

			const node* getNext() const;

			template<class ... ARGS>
			node* allocNext(ARGS&&...);
		};

		template<class T>
		class UnidirectionalNode<T, false> {
			using node = UnidirectionalNode<T, false>;
			node* m_next;
		public:
			T data;

			template<class ... ARGS>
			UnidirectionalNode(node* next, ARGS&& ...);

			template<class ... ARGS>
			UnidirectionalNode(ARGS&& ...);

			node* getNext();

			const node* getNext() const;

			template<class ... ARGS>
			node* allocNext(ARGS&&...);
		};


		template<class T>
		template<class ...ARGS>
		inline BidirectionalNode<T, true>::BidirectionalNode(node* next, node* prev, ARGS&& ... a):
			m_prev(prev), m_next(next), data(std::forward<ARGS>(a)...) {
		}

		template<class T>
		template<class ...ARGS>
		inline BidirectionalNode<T, true>::BidirectionalNode(node* next, ARGS&& ... a):
			m_prev(nullptr), m_next(next), data(std::forward<ARGS>(a)...) {
		}

		template<class T>
		template<class ...ARGS>
		inline BidirectionalNode<T, true>::BidirectionalNode(ARGS&& ... a) :
			m_prev(nullptr), m_next(nullptr), data(std::forward<ARGS>(a)...) {}

		template<class T>
		template<class ...ARGS>
		inline BidirectionalNode<T, true>* BidirectionalNode<T, true>::allocNext(ARGS&& ... a) {
			if (m_next != nullptr) {
				auto ptr = new node(this, m_next, std::forward<ARGS>(a)...);
				m_next->m_prev = ptr;
				return m_next = ptr;
			} else
				return m_next = new node(this, m_next, std::forward<ARGS>(a)...);
		}
		template<class T>
		template<class ...ARGS>
		inline BidirectionalNode<T, true>* BidirectionalNode<T, true>::allocPrev(ARGS&& ... a) {
			if (m_prev != nullptr) {
				auto ptr = new node(m_prev, this, std::forward<ARGS>(a)...);
				m_prev->m_next = ptr;
				return m_prev = ptr;
			} else
				return m_prev = new node(m_prev, this, std::forward<ARGS>(a)...);
		}

		template<class T>
		inline void BidirectionalNode<T, true>::remove() {
			unlink();
			delete this;
		}

		template<class T>
		inline void BidirectionalNode<T, false>::remove() {
			unlink();
			delete this;
		}

		template<class T>
		inline BidirectionalNode<T, true>::~BidirectionalNode() {
			delete m_next;
		}

		template<class T>
		inline BidirectionalNode<T, true>* BidirectionalNode<T, true>::getNext() {
			return m_next;
		}

		template<class T>
		inline BidirectionalNode<T, true>* BidirectionalNode<T, true>::getPrev() {
			return m_prev;
		}

		template<class T>
		inline const BidirectionalNode<T, true>* BidirectionalNode<T, true>::getNext() const {
			return m_next;
		}
		template<class T>
		inline const BidirectionalNode<T, true>* BidirectionalNode<T, true>::getPrev() const {
			return m_prev;
		}

		template<class T>
		template<class ...ARGS>
		inline BidirectionalNode<T, false>::BidirectionalNode(node* next, node* prev, ARGS&& ... a) :
			m_prev(prev), m_next(next), data(std::forward<ARGS>(a)...) {}

		template<class T>
		template<class ...ARGS>
		inline BidirectionalNode<T, false>::BidirectionalNode(node* next, ARGS&& ... a) :
			m_prev(nullptr), m_next(next), data(std::forward<ARGS>(a)...) {}

		template<class T>
		template<class ...ARGS>
		inline BidirectionalNode<T, false>::BidirectionalNode(ARGS&& ... a) :
			m_prev(nullptr), m_next(nullptr), data(std::forward<ARGS>(a)...) {}

		template<class T>
		template<class ...ARGS>
		inline BidirectionalNode<T, false>* BidirectionalNode<T, false>::allocNext(ARGS&& ... a) {
			if (m_next != nullptr) {
				auto ptr = new node(this, m_next, std::forward<ARGS>(a)...);
				m_next->m_prev = ptr;
				return m_next = ptr;
			} else
				return m_next = new node(this, m_next, std::forward<ARGS>(a)...);
		}
		template<class T>
		template<class ...ARGS>
		inline BidirectionalNode<T, false>* BidirectionalNode<T, false>::allocPrev(ARGS&& ... a) {
			if (m_prev != nullptr) {
				auto ptr = new node(m_prev, this, std::forward<ARGS>(a)...);
				m_prev->m_next = ptr;
				return m_prev = ptr;
			} else
				return m_prev = new node(m_prev, this, std::forward<ARGS>(a)...);
		}


		template<class T>
		inline BidirectionalNode<T, false>* BidirectionalNode<T, false>::getNext() {
			return m_next;
		}

		template<class T>
		inline BidirectionalNode<T, false>* BidirectionalNode<T, false>::getPrev() {
			return m_prev;
		}

		template<class T>
		inline const BidirectionalNode<T, false>* BidirectionalNode<T, false>::getNext() const {
			return m_next;
		}

		template<class T>
		inline const BidirectionalNode<T, false>* BidirectionalNode<T, false>::getPrev() const {
			return m_prev;
		}

		template<class T>
		inline BidirectionalNode<T, false>* BidirectionalNode<T, false>::linkNext(node* next) {
			m_next->m_prev = next;
			next->m_next = m_next;
			m_next = next;
			next->m_prev = this;
			return next;
		}

		template<class T>
		inline BidirectionalNode<T, false>* BidirectionalNode<T, false>::linkPrev(node* prev) {
			m_prev->m_next = prev;
			prev->m_prev = m_prev;
			m_prev = prev;
			prev->m_next = this;
			return prev;
		}

		template<class T>
		inline BidirectionalNode<T, true>* BidirectionalNode<T, true>::linkNext(node* next) {
			m_next->m_prev = next;
			next->m_next = m_next;
			m_next = next;
			next->m_prev = this;
			return next;
		}

		template<class T>
		inline BidirectionalNode<T, true>* BidirectionalNode<T, true>::linkPrev(node* prev) {
			m_prev->m_next = prev;
			prev->m_prev = m_prev;
			m_prev = prev;
			prev->m_next = this;
			return prev;
		}

		template<class T>
		inline void BidirectionalNode<T, true>::unlink() {
			if (m_next != nullptr)
				m_next->m_prev = m_prev;
			if (m_prev != nullptr)
				m_prev->m_next = m_next;
			m_prev = nullptr;
			m_next = nullptr;
		}

		template<class T>
		inline BidirectionalNode<T, true>* BidirectionalNode<T, true>::unlinkForward() {
			auto nd = m_next;
			unlink();
			return nd;
		}

		template<class T>
		inline BidirectionalNode<T, true>* BidirectionalNode<T, true>::unlinkBackward() {
			auto nd = m_prev;
			unlink();
			return nd;
		}

		template<class T>
		inline BidirectionalNode<T, true>* BidirectionalNode<T, true>::removeForward() {
			auto nd = m_next;
			remove();
			return nd;
		}

		template<class T>
		inline BidirectionalNode<T, true>* BidirectionalNode<T, true>::removeBackward() {
			auto nd = m_prev;
			remove();
			return nd;
		}


		template<class T>
		inline void BidirectionalNode<T, false>::unlink() {
			if (m_next != nullptr)
				m_next->m_prev = m_prev;
			if (m_prev != nullptr)
				m_prev->m_next = m_next;
			m_prev = nullptr;
			m_next = nullptr;
		}

		template<class T>
		inline BidirectionalNode<T, false>* BidirectionalNode<T, false>::unlinkForward() {
			auto nd = m_next;
			unlink();
			return nd;
		}

		template<class T>
		inline BidirectionalNode<T, false>* BidirectionalNode<T, false>::unlinkBackward() {
			auto nd = m_prev;
			unlink();
			return nd;
		}

		template<class T>
		inline BidirectionalNode<T, false>* BidirectionalNode<T, false>::removeForward() {
			auto nd = m_next;
			remove();
			return nd;
		}

		template<class T>
		inline BidirectionalNode<T, false>* BidirectionalNode<T, false>::removeBackward() {
			auto nd = m_prev;
			remove();
			return nd;
		}


		template<class T>
		template<class ...ARGS>
		inline UnidirectionalNode<T, true>::UnidirectionalNode(node* next, ARGS&& ... a):
			m_next(next), data(std::forward<ARGS>(a)...) {}

		template<class T>
		template<class ...ARGS>
		inline UnidirectionalNode<T, true>::UnidirectionalNode(ARGS&& ... a):
			m_next(nullptr), data(std::forward<ARGS>(a)...){}

		template<class T>
		template<class ...ARGS>
		inline UnidirectionalNode<T, true>* UnidirectionalNode<T, true>::allocNext(ARGS&& ... a) {
			return m_next = new node(m_next, std::forward<ARGS>(a)...);
		}
		template<class T>
		inline UnidirectionalNode<T, true>::~UnidirectionalNode() {
			if (m_next != nullptr)
				delete m_next;
		}

		template<class T>
		inline UnidirectionalNode<T, true>* UnidirectionalNode<T, true>::getNext() {
			return m_next;
		}
		template<class T>
		inline const UnidirectionalNode<T, true>* UnidirectionalNode<T, true>::getNext() const {
			return m_next;
		}


		template<class T>
		template<class ...ARGS>
		inline UnidirectionalNode<T, false>::UnidirectionalNode(node* next, ARGS&& ... a) :
			m_next(next), data(std::forward<ARGS>(a...)) {}
		template<class T>
		template<class ...ARGS>
		inline UnidirectionalNode<T, false>::UnidirectionalNode(ARGS&& ... a) :
			m_next(nullptr), data(std::forward<ARGS>(a...)) {}

		template<class T>
		template<class ...ARGS>
		inline UnidirectionalNode<T, false>* UnidirectionalNode<T, false>::allocNext(ARGS&& ... a) {
			return m_next = new node(m_next, std::forward<ARGS>(a...));
		}

		template<class T>
		inline UnidirectionalNode<T, false>* UnidirectionalNode<T, false>::getNext() {
			return m_next;
		}
		template<class T>
		inline const UnidirectionalNode<T, false>* UnidirectionalNode<T, false>::getNext() const {
			return m_next;
		}

	}
}
