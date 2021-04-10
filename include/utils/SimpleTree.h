#pragma once


#include <memory>
#include "../Aliases.h"

namespace demonorium
{
	namespace utils
	{
		template<class T, class Alloc>
		class SimpleTree;

		template<class T>
		class TreeNode {
			template<class T, class Alloc>
			friend class SimpleTree;

			TreeNode<T>* parent;
			TreeNode<T>* left;
			TreeNode<T>* right;
			byte height;
			T content;
		public:
			template<class ... Args>
			TreeNode(Args&& ... args);
			~TreeNode();

			T& get();
			const T& get() const;
			
			TreeNode<T>* getLeft();
			TreeNode<T>* getRight();

			const TreeNode<T>* getLeft() const;
			const TreeNode<T>* getRight() const;

			TreeNode<T>* unlinkLeft();
			TreeNode<T>* unlinkRight();

			TreeNode<T>* getParent();
			const TreeNode<T>* getParent() const;

			bool hasLeft() const;
			bool hasRight() const;

			void linkLeft(TreeNode<T>* node);
			void linkRight(TreeNode<T>* node);
		};

		template<class T, class Alloc = std::allocator<TreeNode<T>>>
		class SimpleTree {
			TreeNode<T>* m_root;
			Alloc m_alloc;


			static byte height(TreeNode<T>* node);
			static short bfactor(TreeNode<T>* node);
			static void fixheight(TreeNode<T>* node);

			static TreeNode<T>* rotateRight(TreeNode<T>* node);
			static TreeNode<T>* rotateLeft(TreeNode<T>* node);
			static TreeNode<T>* balance(TreeNode<T>* p);

			TreeNode<T>* insert(TreeNode<T>* root, TreeNode<T>* node);
			void destruct(TreeNode<T>* node);
		public:
			SimpleTree(const Alloc& alloc = Alloc());
			~SimpleTree();

			void clear();
			
			template<class ... Args>
			void emplace(Args&& ... args);

			bool isEmpty() const;
			
			auto* getRoot();
			const auto* getRoot() const;

			auto& getAlloc();
			const auto& getAlloc() const;

			template<class ... Args>
			TreeNode<T>* alloc(Args&& ... args);

			void free(TreeNode<T>* node);
		};


		template<class T>
		template<class ...Args>
		inline TreeNode<T>::TreeNode(Args && ...args):
			content(std::forward<Args>(args)...),
			left(nullptr), right(nullptr), parent(nullptr), height(1) {
		}

		template<class T>
		inline TreeNode<T>::~TreeNode() {
			assert(left == nullptr);
			assert(right == nullptr);
		}
		template<class T>
		inline T& TreeNode<T>::get() {
			return content;
		}

		template <class T>
		const T& TreeNode<T>::get() const {
			return content;
		}

		template<class T>
		inline TreeNode<T>* TreeNode<T>::getLeft() {
			return left;
		}
		template<class T>
		inline TreeNode<T>* TreeNode<T>::getRight() {
			return right;
		}

		template <class T>
		const TreeNode<T>* TreeNode<T>::getLeft() const {
			return left;
		}

		template <class T>
		const TreeNode<T>* TreeNode<T>::getRight() const {
			return right;
		}

		template<class T>
		inline TreeNode<T>* TreeNode<T>::unlinkLeft() {
			auto* v = left;
			if (v)
				v->parent = nullptr;

			left = nullptr;
			return v;
		}
		template<class T>
		TreeNode<T>* TreeNode<T>::getParent() {
			return parent;
		}
		template<class T>
		const TreeNode<T>* TreeNode<T>::getParent() const {
			return parent;
		}

		template<class T>
		inline TreeNode<T>* TreeNode<T>::unlinkRight() {
			auto* v = right;
			if (v)
				v->parent = nullptr;

			right = nullptr;
			return v;
		}
		template<class T>
		inline bool TreeNode<T>::hasLeft() const {
			return left != nullptr;
		}
		template<class T>
		inline bool TreeNode<T>::hasRight() const {
			return right != nullptr;
		}
		template<class T>
		inline void TreeNode<T>::linkLeft(TreeNode<T>* node) {
			assert(left == nullptr);
			left = node;
			if (node)
				node->parent = this;
		}
		template<class T>
		inline void TreeNode<T>::linkRight(TreeNode<T>* node) {
			assert(right == nullptr);
			right = node;
			if (node)
				node->parent = this;
		}
		template<class T, class Alloc>
		inline byte SimpleTree<T, Alloc>::height(TreeNode<T>* node) {
			return node?node->height: 0;
		}
		template<class T, class Alloc>
		inline short SimpleTree<T, Alloc>::bfactor(TreeNode<T>* node) {
			return height(node->getLeft()) - height(node->getRight());;
		}
		template<class T, class Alloc>
		inline void SimpleTree<T, Alloc>::fixheight(TreeNode<T>* node) {
			byte hl = height(node->getLeft());
			byte hr = height(node->getRight());
			node->height = (hl > hr ? hl : hr) + 1;

		}
		template<class T, class Alloc>
		inline TreeNode<T>* SimpleTree<T, Alloc>::rotateRight(TreeNode<T>* node) {
			TreeNode<T>* q = node->left;
			node->linkLeft(q->unlinkRight());
			q->linkRight(node);
			fixheight(node);
			fixheight(q);
			return q;
		}
		template<class T, class Alloc>
		inline TreeNode<T>* SimpleTree<T, Alloc>::rotateLeft(TreeNode<T>* node) {
			TreeNode<T>* p = node->right;
			node->linkRight(p->unlinkLeft());
			p->linkLeft(node);
			fixheight(node);
			fixheight(p);
			return p;
		}
		template<class T, class Alloc>
		inline TreeNode<T>* SimpleTree<T, Alloc>::balance(TreeNode<T>* p) {
			fixheight(p);
			if (bfactor(p) == 2) {
				if (bfactor(p->right) < 0)
					p->linkRight(rotateRight(p->right));
				return rotateLeft(p);
			}
			if (bfactor(p) == -2) {
				if (bfactor(p->left) > 0)
					p->linkLeft(rotateLeft(p->left));
				return rotateRight(p);
			}
			return p; // балансировка не нужна
		}

		template <class T, class Alloc>
		TreeNode<T>* SimpleTree<T, Alloc>::insert(TreeNode<T>* root, TreeNode<T>* node) {
			if (!root) return node;

			if (node->content < root->content)
				root->linkLeft(insert(root->left, node));
			else
				root->linkRight(insert(root->right, node));

			return balance(root);
		}

		template<class T, class Alloc>
		inline void SimpleTree<T, Alloc>::destruct(TreeNode<T>* node) {
			if (node->hasLeft())
				destruct(node->left);
			if (node->hasRight())
				destruct(node->right);
			node->left = nullptr;
			node->right = nullptr;
			
			
			free(node);
		}
		template<class T, class Alloc>
		inline SimpleTree<T, Alloc>::SimpleTree(const Alloc& alloc):
			m_root(nullptr), m_alloc(alloc){}

		template<class T, class Alloc>
		inline SimpleTree<T, Alloc>::~SimpleTree() {
			clear();
		}

		template <class T, class Alloc>
		void SimpleTree<T, Alloc>::clear() {
			if (m_root != nullptr) {
				destruct(m_root);
				m_root = nullptr;
			}
		}

		template<class T, class Alloc>
		inline bool SimpleTree<T, Alloc>::isEmpty() const {
			return m_root == nullptr;
		}
		template<class T, class Alloc>
		inline auto* SimpleTree<T, Alloc>::getRoot() {
			return m_root;
		}
		template<class T, class Alloc>
		inline const auto* SimpleTree<T, Alloc>::getRoot() const {
			return m_root;
		}
		template<class T, class Alloc>
		inline auto& SimpleTree<T, Alloc>::getAlloc() {
			return m_alloc;
		}
		template<class T, class Alloc>
		inline const auto& SimpleTree<T, Alloc>::getAlloc() const {
			return m_alloc;
		}
		template<class T, class Alloc>
		inline void SimpleTree<T, Alloc>::free(TreeNode<T>* node) {
			node->~TreeNode();
			std::allocator_traits<Alloc>::deallocate(m_alloc, node, 1);
		}
		template<class T, class Alloc>
		template<class ...Args>
		inline void SimpleTree<T, Alloc>::emplace(Args&& ...args) {
			if (m_root)
				m_root = insert(m_root, alloc(std::forward<Args>(args)...));
			else
				m_root = alloc(std::forward<Args>(args)...);
		}
		template<class T, class Alloc>
		template<class ...Args>
		inline TreeNode<T>* SimpleTree<T, Alloc>::alloc(Args && ...args) {
			return new (std::allocator_traits<Alloc>::allocate(m_alloc, 1)) TreeNode<T>(std::forward<Args>(args) ...);
		}
	}
}
