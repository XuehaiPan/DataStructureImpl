//
// Created by 潘学海.
//

#ifndef DATASTRUCTURE_BINTREE_H
#define DATASTRUCTURE_BINTREE_H

template <class T>
class BinTree //二叉树类
{
protected:
	class node //节点类
	{
		friend class BinTree<T>;

		friend class BinTree<T>::iterator;

	protected:
		T key;
		node *left, *right;

	public:
		node(node *const pLeft = nullptr, node *const pRight = nullptr)
			: left(pLeft), right(pRight)
		{
		}

		node(const T &tkey, node *const pLeft = nullptr, node *const pRight = nullptr)
			: key(tkey), left(pLeft), right(pRight)
		{
		}

		virtual ~node() = default;
	};

	node *mroot;

	void copyTree(node *&ptr1, const node *const ptr2)
	{
		if (ptr2 != nullptr)
		{
			ptr1 = new node(ptr2->key);
			copyTree(ptr1->left, ptr2->left);
			copyTree(ptr1->right, ptr2->right);
		}
		else
			ptr1 = nullptr;
	}

	void createTree(node *&ptr, const unsigned int i, const T array[], const unsigned int tsize)
	{
		if (i < tsize)
		{
			ptr = new node(array[i]);
			createTree(ptr->left, 2 * i + 1, array, tsize);
			createTree(ptr->right, 2 * i + 2, array, tsize);
		}
		else
			ptr = nullptr;
	}

	void clearTree(node *const ptr) const
	{
		if (ptr != nullptr)
		{
			clearTree(ptr->left);
			clearTree(ptr->right);
			ptr->~node();
		}
	}

public:
	class iterator
	{
		friend class BinTree<T>;

	protected:
		node *ptr;

		iterator(node *const p)
			: ptr(p)
		{
		}

	public:
		iterator() = default;

		iterator(const iterator &) = default;

		virtual ~iterator() = default;

		iterator &operator=(const iterator &) = default;

		T &operator*() const
		{
			return ptr->key;
		}

		iterator left() const
		{
			return iterator(ptr->left);
		}

		iterator right() const
		{
			return iterator(ptr->right);
		}

		const iterator &toLeft()
		{
			ptr = ptr->left;
			return *this;
		}

		const iterator &toRight()
		{
			ptr = ptr->right;
			return *this;
		}

		inline bool isNull() const
		{
			return (ptr == nullptr);
		}

		inline bool isLeaf() const
		{
			if (!isNull())
				return (ptr->left == nullptr && ptr->right == nullptr);
			else
				return false;
		}

		inline bool operator==(const iterator &p) const
		{
			return (ptr == p.ptr);
		}

		inline bool operator!=(const iterator &p) const
		{
			return (ptr != p.ptr);
		}
	};

	BinTree()
		: mroot(nullptr)
	{
	}

	BinTree(const T &key)
	{
		mroot = new node(key);
	}

	BinTree(const BinTree &tree)
		: mroot(nullptr)
	{
		*this = tree;
	}

	BinTree(const T &key, const BinTree &leftTree, const BinTree &rightTree)
	{
		BinTree *pLeftTree = new BinTree(leftTree), *pRightTree = new BinTree(rightTree);
		mroot = new node(key, pLeftTree->mroot, pRightTree->mroot);
	}

	BinTree(const iterator &i)
		: mroot(nullptr)
	{
		copyTree(mroot, i->ptr);
	}

	BinTree(const T array[], const unsigned int tsize)
	{
		createTree(mroot, 0, array, tsize);
	}

	virtual ~BinTree()
	{
		clear();
	}

	BinTree &operator=(const BinTree &tree)
	{
		clear();
		copyTree(mroot, tree.mroot);
		return *this;
	}

	void clear()
	{
		if (!isEmpty())
		{
			clearTree(mroot);
			mroot = nullptr;
		}
	}

	int height() const
	{
		return height(root());
	}

	int height(const iterator &iroot) const
	{
		if (!iroot.isNull())
		{
			int leftHeight = height(iroot.left()), rightHeight = height(iroot.right());
			return (leftHeight > rightHeight ? leftHeight + 1 : rightHeight + 1);
		}
		else
			return -1;
	}

	unsigned int depth(const iterator &i) const
	{
		if (!i.isNull())
		{
			iterator iparent = parent(i);
			if (!iparent.isNull())
				return (depth(iparent) + 1);
			else
				return 0;
		}
		else
			return 0;
	}

	unsigned int size() const
	{
		return size(root());
	}

	unsigned int size(const iterator &iroot) const
	{
		if (!iroot.isNull())
			return (size(iroot.left()) + size(iroot.right()) + 1);
		else
			return 0;
	}

	unsigned int leafs() const
	{
		return leafs(root());
	}

	unsigned int leafs(const iterator &iroot) const
	{
		if (!iroot.isNull())
		{
			if (!iroot.isLeaf())
				return (leafs(iroot.left()) + leafs(iroot.right()));
			else
				return 1;
		}
		else
			return 0;
	}

	iterator root() const
	{
		return iterator(mroot);
	}

	iterator parent(const iterator &ichild) const
	{
		if (ichild != root())
			return parent(root(), ichild);
		else
			return iterator(nullptr);
	}

	iterator parent(const iterator &iroot, const iterator &ichild) const
	{
		if (!iroot.isNull() && !ichild.isNull())
		{
			if (iroot.left() == ichild || iroot.right() == ichild)
				return iroot;
			else
			{
				iterator ileft = parent(iroot.left(), ichild), iright = parent(iroot.right(), ichild);
				if (!ileft.isNull())
					return ileft;
				else if (!iright.isNull())
					return iright;
			}
		}
		return iterator(nullptr);
	}

	iterator loc(const unsigned int tloc) const
	{
		if (tloc == 0)
			return root();
		else
		{
			iterator iparent = loc((tloc - 1) / 2);
			if (!iparent.isNull())
			{
				if (tloc == 2 * ((tloc - 1) / 2) + 1)
					return iparent.left();
				else
					return iparent.right();
			}
			else
				return iterator(nullptr);
		}
	}

	unsigned int loc(const iterator &i) const
	{
		iterator iparent = parent(i);
		if (!iparent.isNull())
		{
			if (i == parent(i).left())
				return (2 * loc(parent(i)) + 1);
			else
				return (2 * loc(parent(i)) + 2);
		}
		else
			return 0;
	}

	bool search(iterator &i, const T &key)
	{
		return search(root(), i, key);
	}

	bool search(const iterator &iroot, iterator &i, const T &key)
	{
		if (!iroot.isNull())
		{
			if (*iroot == key)
			{
				i = iroot;
				return true;
			}
			else
				return (search(iroot.left(), i, key) || search(iroot.right(), i, key));
		}
		else
			return false;
	}

	bool isEmpty() const
	{
		return (mroot == nullptr);
	}

	T &operator[](const unsigned int tloc) const
	{
		iterator i = loc(tloc);
		if (!i.isNull())
			return *i;
		else
			return (mroot->key);
	}

	void inorderTraversal(void(*func)(T &)) const
	{
		inorderTraversal(root(), func);
	}

	void preorderTraversal(void(*func)(T &)) const
	{
		preorderTraversal(root(), func);
	}

	void postorderTraversal(void(*func)(T &)) const
	{
		postorderTraversal(root(), func);
	}

	void inorderTraversal(void(*func)(T *const)) const
	{
		inorderTraversal(root(), func);
	}

	void preorderTraversal(void(*func)(T *const)) const
	{
		preorderTraversal(root(), func);
	}

	void postorderTraversal(void(*func)(T *const)) const
	{
		postorderTraversal(root(), func);
	}

	void inorderTraversal(void(*func)(const iterator &)) const
	{
		inorderTraversal(root(), func);
	}

	void preorderTraversal(void(*func)(const iterator &)) const
	{
		preorderTraversal(root(), func);
	}

	void postorderTraversal(void(*func)(const iterator &)) const
	{
		postorderTraversal(root(), func);
	}

	void inorderTraversal(void(*func)(const iterator &, const BinTree &)) const
	{
		inorderTraversal(root(), func);
	}

	void preorderTraversal(void(*func)(const iterator &, const BinTree &)) const
	{
		preorderTraversal(root(), func);
	}

	void postorderTraversal(void(*func)(const iterator &, const BinTree &)) const
	{
		postorderTraversal(root(), func);
	}

	void inorderTraversal(const iterator &iroot, void(*func)(T &)) const
	{
		if (!iroot.isNull())
		{
			inorderTraversal(iroot.left(), func);
			func(*iroot);
			inorderTraversal(iroot.right(), func);
		}
	}

	void preorderTraversal(const iterator &iroot, void(*func)(T &)) const
	{
		if (!iroot.isNull())
		{
			func(*iroot);
			preorderTraversal(iroot.left(), func);
			preorderTraversal(iroot.right(), func);
		}
	}

	void postorderTraversal(const iterator &iroot, void(*func)(T &)) const
	{
		if (!iroot.isNull())
		{
			postorderTraversal(iroot.left(), func);
			postorderTraversal(iroot.right(), func);
			func(*iroot);
		}
	}

	void inorderTraversal(const iterator &iroot, void(*func)(T *const)) const
	{
		if (!iroot.isNull())
		{
			inorderTraversal(iroot.left(), func);
			func(&(*iroot));
			inorderTraversal(iroot.right(), func);
		}
	}

	void preorderTraversal(const iterator &iroot, void(*func)(T *const)) const
	{
		if (!iroot.isNull())
		{
			func(&(*iroot));
			preorderTraversal(iroot.left(), func);
			preorderTraversal(iroot.right(), func);
		}
	}

	void postorderTraversal(const iterator &iroot, void(*func)(T *const)) const
	{
		if (!iroot.isNull())
		{
			postorderTraversal(iroot.left(), func);
			postorderTraversal(iroot.right(), func);
			func(&(*iroot));
		}
	}

	void inorderTraversal(const iterator &iroot, void(*func)(const iterator &)) const
	{
		if (!iroot.isNull())
		{
			inorderTraversal(iroot.left(), func);
			func(iroot);
			inorderTraversal(iroot.right(), func);
		}
	}

	void preorderTraversal(const iterator &iroot, void(*func)(const iterator &)) const
	{
		if (!iroot.isNull())
		{
			func(iroot);
			preorderTraversal(iroot.left(), func);
			preorderTraversal(iroot.right(), func);
		}
	}

	void postorderTraversal(const iterator &iroot, void(*func)(const iterator &)) const
	{
		if (!iroot.isNull())
		{
			postorderTraversal(iroot.left(), func);
			postorderTraversal(iroot.right(), func);
			func(iroot);
		}
	}

	void inorderTraversal(const iterator &iroot, void(*func)(const iterator &, const BinTree &)) const
	{
		if (!iroot.isNull())
		{
			inorderTraversal(iroot.left(), func);
			func(iroot, *this);
			inorderTraversal(iroot.right(), func);
		}
	}

	void preorderTraversal(const iterator &iroot, void(*func)(const iterator &, const BinTree &)) const
	{
		if (!iroot.isNull())
		{
			func(iroot, *this);
			preorderTraversal(iroot.left(), func);
			preorderTraversal(iroot.right(), func);
		}
	}

	void postorderTraversal(const iterator &iroot, void(*func)(const iterator &, const BinTree &)) const
	{
		if (!iroot.isNull())
		{
			postorderTraversal(iroot.left(), func);
			postorderTraversal(iroot.right(), func);
			func(iroot, *this);
		}
	}
};

#endif //DATASTRUCTURE_BINTREE_H