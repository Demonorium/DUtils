#pragma once

#include "LinkedListNodes.h"


namespace demonorium
{
	namespace utils
	{

		template<class T, bool selfd, class direction>
		class BidirectionalListIterator;

		template<class T, bool selfdestruction>
		class BidirectionalList;

		namespace
		{
			template<bool forward>
			struct _direction {
			};
			template<>
			struct _direction<true> {
				template<class T, bool selfdestruction>
				static inline BidirectionalNode<T, selfdestruction>* next(BidirectionalNode<T, selfdestruction>* node) {
					return node->getNext();
				}
				template<class T, bool selfdestruction>
				static inline BidirectionalNode<T, selfdestruction>* prev(BidirectionalNode<T, selfdestruction>* node) {
					return node->getPrev();
				}
			};

			template<>
			struct _direction<false> {
				template<class T, bool selfdestruction>
				static inline BidirectionalNode<T, selfdestruction>* next(BidirectionalNode<T, selfdestruction>* node) {
					return node->getPrev();
				}

				template<class T, bool selfdestruction>
				static inline BidirectionalNode<T, selfdestruction>* prev(BidirectionalNode<T, selfdestruction>* node) {
					return node->getNext();
				}
			};
		}

		template<class T, bool selfdestruction = true>
		class BidirectionalList {
		public:
			using list = BidirectionalList<T, selfdestruction>;
			using node = BidirectionalNode<T, selfdestruction>;
			using iterator = BidirectionalListIterator<T, selfdestruction, _direction<true>>;
			using const_iterator = const iterator;

			using reverse_iterator = BidirectionalListIterator<T, selfdestruction, _direction<false>>;
			using const_reverse_iterator = const reverse_iterator;


			BidirectionalList();
			BidirectionalList(node* first, node* last);
			BidirectionalList(node* node);
			BidirectionalList(list&& other);
			
			~BidirectionalList();

			bool isEmpty() const;

			void appendEnd(node* nd);
			void appendStart(node* nd);

			void appendEnd(list&&	nd);
			void appendStart(list&& nd);

			template<class ... ARGS>
			void allocEnd(ARGS&& ... a);
			template<class ... ARGS>
			void allocStart(ARGS&& ... a);


			node* getStart();
			node* getEnd();

			
			iterator begin();
			iterator end();
			const_iterator begin() const;
			const_iterator end() const;
			const_iterator cbegin() const;
			const_iterator cend() const;

			reverse_iterator rbegin();
			reverse_iterator rend();
			const_reverse_iterator rbegin() const;
			const_reverse_iterator rend() const;
			const_reverse_iterator crbegin() const;
			const_reverse_iterator crend() const;


			void removeFirst();
			void removeLast();

			node* unlinkFirst();
			node* unlinkLast();
		private:
			node* m_first;
			node* m_last;
		};

		

		template<class T, bool selfd, class direction>
		class BidirectionalListIterator : public std::iterator<std::input_iterator_tag, T> {
			using iter = BidirectionalListIterator<T, selfd, direction>;
			using list = BidirectionalList<T, selfd>;
			using node = typename list::node;
			
			list* m_list;
			node* m_node;
		public:
			BidirectionalListIterator(list* m_list, node* node);
			BidirectionalListIterator(const iter& other);

			bool operator!=(iter const& other) const;
			bool operator==(iter const& other) const; 

			typename iter::reference operator*() const;

			iter remove();
			std::pair<node*, iter> unlink();


			void insertNext(node* nd);
			void insertPrev(node* nd);

			void insertNext(list&& nd);
			void insertPrev(list&& nd);

			node* getNode();

			template<class ... ARGS>
			void allocNext(ARGS&& ... args);
			template<class ... ARGS>
			void allocPrev(ARGS&& ... args);

			T* operator->();
			const T* operator->() const;

			iter& operator++();
			iter& operator--();
		};

		

		template<class T, bool selfd, class direction>
		inline BidirectionalListIterator<T, selfd, direction>::BidirectionalListIterator(list* list, node* node):
			m_node(node), m_list(list){}

		template<class T, bool selfd, class direction>
		inline BidirectionalListIterator<T, selfd, direction>::BidirectionalListIterator(const iter& other):
			m_list(other.m_list), m_node(other.m_node) {}

		template<class T, bool selfd, class direction>
		inline bool BidirectionalListIterator<T, selfd, direction>::operator!=(iter const& other) const {
			return (m_node != other.m_node) || (m_list != other.m_list);
		}

		template<class T, bool selfd, class direction>
		inline bool BidirectionalListIterator<T, selfd, direction>::operator==(iter const& other) const {
			return (m_node == other.m_node) && (m_list == other.m_list);
		}

		template<class T, bool selfd, class direction>
		inline typename BidirectionalListIterator<T, selfd, direction>::reference BidirectionalListIterator<T, selfd, direction>::operator*() const {
			return m_node.data;
		}

		template<class T, bool selfd, class direction>
		inline BidirectionalListIterator<T, selfd, direction> BidirectionalListIterator<T, selfd, direction>::remove() {
			if (m_node == m_list->m_first) {
				m_list->removeFirst();
				return m_list->begin();
			}
			if (m_node == m_list->m_last) {
				m_list->removeLast();
				return m_list->end();
			}

			return iter(m_list, m_node->removeForward());
		}


		template<class T, bool selfd, class direction>
		inline std::pair<typename BidirectionalListIterator<T, selfd, direction>::node*, BidirectionalListIterator<T, selfd, direction>> BidirectionalListIterator<T, selfd, direction>::unlink() {
			std::pair<node*, iter> result;
			if (m_node == m_list->m_first) {
				return std::make_pair(m_list->unlinkFirst(), m_list->begin());
			}
			if (m_node == m_list->m_last) {
				return std::make_pair(m_list->unlinkLast(), m_list->end());
			}

			return std::make_pair(m_node->next(), iter(m_list, m_node->unlinkForward()));
		}


		template<class T, bool selfd, class direction>
		inline void BidirectionalListIterator<T, selfd, direction>::insertNext(node* nd) {
			if (m_node == m_list->m_last) {
				m_list->appendEnd(nd);
			} else
				m_node->linkNext(nd);
		}

		template<class T, bool selfd, class direction>
		inline void BidirectionalListIterator<T, selfd, direction>::insertPrev(node* nd) {
			if (m_node == m_list->m_first) {
				m_list->appendStart(nd);
			} else
				m_node->linkPrev(nd);
		}

		template<class T, bool selfd, class direction>
		inline void BidirectionalListIterator<T, selfd, direction>::insertNext(list&& nd) {
			if (m_node == m_list->m_last) {
				m_list->appendEnd(nd);
			} else {
				nd.appendEnd(m_node->getNext());
				nd.appendStart(m_node);
			}
		}

		template<class T, bool selfd, class direction>
		inline void BidirectionalListIterator<T, selfd, direction>::insertPrev(list&& nd) {
			if (m_node == m_list->m_first) {
				m_list->appendStart(nd);
			} else {
				nd.appendEnd(m_node);
				nd.appendStart(m_node->getPrev());
			}
		}

		template<class T, bool selfd, class direction>
		inline typename BidirectionalListIterator<T, selfd, direction>::node* BidirectionalListIterator<T, selfd, direction>::getNode() {
			return m_node;
		}

		template<class T, bool selfd, class direction>
		inline T* BidirectionalListIterator<T, selfd, direction>::operator->() {
			return &(m_node->data);
		}

		template<class T, bool selfd, class direction>
		inline const T* BidirectionalListIterator<T, selfd, direction>::operator->() const {
			return &(m_node->data);
		}

		template<class T, bool selfd, class direction>
		inline BidirectionalListIterator<T, selfd, direction>& BidirectionalListIterator<T, selfd, direction>::operator++() {
			m_node = direction::next(m_node);
			return *this;
		}

		template<class T, bool selfd, class direction>
		inline BidirectionalListIterator<T, selfd, direction>& BidirectionalListIterator<T, selfd, direction>::operator--() {
			m_node = direction::prev(m_node);
			return *this;
		}

		template<class T, bool selfdestruction>
		inline BidirectionalList<T, selfdestruction>::BidirectionalList():
			m_first(nullptr), m_last(nullptr) {
		}

		template<class T, bool selfdestruction>
		inline BidirectionalList<T, selfdestruction>::BidirectionalList(node* first, node* last) :
			m_first(first), m_last(last) {}

		template<class T, bool selfdestruction>
		inline BidirectionalList<T, selfdestruction>::BidirectionalList(node* node) :
			m_first(node), m_last(node) {}

		template<class T, bool selfdestruction>
		inline BidirectionalList<T, selfdestruction>::BidirectionalList(list&& other) {
			memcpy(this, &other, sizeof(BidirectionalList<T, selfdestruction>));
		}

		template<class T, bool selfdestruction>
		inline BidirectionalList<T, selfdestruction>::~BidirectionalList() {
			if (m_first != nullptr)
				delete m_first;
		}

		template<class T, bool selfdestruction>
		inline bool BidirectionalList<T, selfdestruction>::isEmpty() const {
			return m_first == nullptr;
		}


		template<class T, bool selfdestruction>
		inline void BidirectionalList<T, selfdestruction>::appendEnd(node* nd) {
			if (m_last != nullptr)
				m_last = m_last->linkNext(nd);
			else {
				m_first = nd;
				m_last = nd;
			}
		}

		template<class T, bool selfdestruction>
		inline void BidirectionalList<T, selfdestruction>::appendStart(node* nd) {
			if (m_first != nullptr)
				m_first = m_first->linkPrev(nd);
			else {
				m_first = nd;
				m_last = nd;
			}
		}
		template<class T, bool selfdestruction>
		inline void BidirectionalList<T, selfdestruction>::appendEnd(list&& nd) {
			if (m_last != nullptr) {
				m_last->linkNext(nd.m_first);
				m_last = nd.m_last;
			}
			else {
				m_first = nd.m_first;
				m_last = nd.m_last;
			}
		}
		template<class T, bool selfdestruction>
		inline void BidirectionalList<T, selfdestruction>::appendStart(list&& nd) {
			if (m_first != nullptr) {
				m_first->linkPrev(nd.m_last);
				m_first = nd.m_first;
			} else {
				m_first = nd.m_first;
				m_last = nd.m_last;
			}
		}
		
		template<class T, bool selfdestruction>
		inline typename BidirectionalList<T, selfdestruction>::node* BidirectionalList<T, selfdestruction>::getStart() {
			return m_first;
		}

		template<class T, bool selfdestruction>
		inline typename BidirectionalList<T, selfdestruction>::node* BidirectionalList<T, selfdestruction>::getEnd() {
			return m_last;
		}

		template<class T, bool selfdestruction>
		inline typename BidirectionalList<T, selfdestruction>::iterator BidirectionalList<T, selfdestruction>::begin() {
			return iterator(this, m_first);
		}

		template<class T, bool selfdestruction>
		inline typename BidirectionalList<T, selfdestruction>::iterator BidirectionalList<T, selfdestruction>::end() {
			return iterator(this, nullptr);
		}

		template<class T, bool selfdestruction>
		inline const typename BidirectionalList<T, selfdestruction>::iterator BidirectionalList<T, selfdestruction>::begin() const {
			return iterator(this, m_first);
		}

		template<class T, bool selfdestruction>
		inline const typename BidirectionalList<T, selfdestruction>::iterator BidirectionalList<T, selfdestruction>::end() const {
			return iterator(this, nullptr);
		}

		template<class T, bool selfdestruction>
		inline const typename BidirectionalList<T, selfdestruction>::iterator BidirectionalList<T, selfdestruction>::cbegin() const {
			return iterator(this, m_first);
		}

		template<class T, bool selfdestruction>
		inline const typename BidirectionalList<T, selfdestruction>::iterator BidirectionalList<T, selfdestruction>::cend() const {
			return iterator(this, nullptr);
		}

		template<class T, bool selfdestruction>
		inline typename BidirectionalList<T, selfdestruction>::reverse_iterator BidirectionalList<T, selfdestruction>::rbegin() {
			return reverse_iterator(this, m_last);
		}

		template<class T, bool selfdestruction>
		inline typename BidirectionalList<T, selfdestruction>::reverse_iterator BidirectionalList<T, selfdestruction>::rend() {
			return reverse_iterator(this, nullptr);
		}

		template<class T, bool selfdestruction>
		inline const typename BidirectionalList<T, selfdestruction>::reverse_iterator BidirectionalList<T, selfdestruction>::rbegin() const {
			return reverse_iterator(this, m_last);
		}

		template<class T, bool selfdestruction>
		inline const typename BidirectionalList<T, selfdestruction>::reverse_iterator BidirectionalList<T, selfdestruction>::rend() const {
			return reverse_iterator(this, nullptr);
		}

		template<class T, bool selfdestruction>
		inline const typename BidirectionalList<T, selfdestruction>::reverse_iterator BidirectionalList<T, selfdestruction>::crbegin() const {
			return reverse_iterator(this, m_last);
		}

		template<class T, bool selfdestruction>
		inline const typename BidirectionalList<T, selfdestruction>::reverse_iterator BidirectionalList<T, selfdestruction>::crend() const {
			return reverse_iterator(this, nullptr);
		}

		template<class T, bool selfdestruction>
		inline void BidirectionalList<T, selfdestruction>::removeFirst() {
			if (m_first != nullptr)
				m_first = m_first->removeForward();
		}

		template<class T, bool selfdestruction>
		inline void BidirectionalList<T, selfdestruction>::removeLast() {
			if (m_last != nullptr)
				m_last = m_last->removeBackward();
		}

		template<class T, bool selfdestruction>
		inline typename BidirectionalList<T, selfdestruction>::node* BidirectionalList<T, selfdestruction>::unlinkFirst() {
			auto t = m_first;
			if (m_first != nullptr)
				m_first = m_first->unlinkForward();
			return t;
		}

		template<class T, bool selfdestruction>
		inline typename BidirectionalList<T, selfdestruction>::node* BidirectionalList<T, selfdestruction>::unlinkLast() {
			auto t = m_last;
			if (m_last != nullptr)
				m_last = m_last->unlinkForward();
			return t;
		}

		template<class T, bool selfdestruction>
		template<class ...ARGS>
		inline void BidirectionalList<T, selfdestruction>::allocEnd(ARGS&& ...a) {
			if (m_last != nullptr) {
				m_last = m_last->allocNext(a...);
			} else {
				m_first = new node(a...);
				m_last = m_first;
			}
		}
		template<class T, bool selfdestruction>
		template<class ...ARGS>
		inline void BidirectionalList<T, selfdestruction>::allocStart(ARGS&& ...a) {
			if (m_first != nullptr) {
				m_first = m_first->allocPrev(a...);
			} else {
				m_first = new node(a...);
				m_last = m_first;
			}
		}
		template<class T, bool selfd, class direction>
		template<class ...ARGS>
		inline void BidirectionalListIterator<T, selfd, direction>::allocNext(ARGS&& ...args) {
			if (m_node == m_list->m_last) {
				m_list->allocEnd(std::forward<ARGS>(args)...);
			} else
				m_node->allocNext(std::forward<ARGS>(args)...);
		}

		template<class T, bool selfd, class direction>
		template<class ...ARGS>
		inline void BidirectionalListIterator<T, selfd, direction>::allocPrev(ARGS&& ...args) {
			if (m_node == m_list->m_first) {
				m_list->allocStart(std::forward<ARGS>(args)...);
			} else
				m_node->allocPrev(std::forward<ARGS>(args)...);
		}
	}
}
