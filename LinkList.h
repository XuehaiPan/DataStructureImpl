//
// Created by 潘学海 on 16/9/25.
//

#ifndef DATASTRUCTURE_LINKLIST_H
#define DATASTRUCTURE_LINKLIST_H

template <class T>
class LinkList //单链表类
{
protected:
	class node //节点类
	{
		friend class LinkList<T>;

		friend class LinkList<T>::iterator;

	protected:
		T key;
		node *next;

	public:
		node() = default; //默认构造函数

		node(node *const pNext) //构造函数
			: next(pNext)
		{
		}

		node(const T &tkey, node *const pNext = nullptr) //构造函数
			: key(tkey), next(pNext)
		{
		}

		virtual ~node() = default; //析构函数
	};

	node *head;
	unsigned int msize;

public:
	class iterator //迭代器类
	{
		friend class LinkList<T>;

	protected:
		node *ptrf;

		iterator(node *const pf) //构造函数
			: ptrf(pf)
		{
		}

	public:
		iterator() = default; //默认构造函数

		iterator(const iterator &) = default; //拷贝构造函数

		virtual ~iterator() = default; //析构函数

		iterator &operator=(const iterator &) = default; //拷贝

		T &operator*() const
		{
			return ptrf->next->key;
		}

		T &operator[](const unsigned int n) const
		{
			return *(*this + n);
		}

		inline bool isNull() const
		{
			if (ptrf == nullptr)
				return true;
			else
				return (ptrf->next == nullptr);
		}

		const iterator &operator++()
		{
			ptrf = ptrf->next;
			return *this;
		}

		iterator operator++(int)
		{
			iterator temp(ptrf);
			ptrf = ptrf->next;
			return temp;
		}

		iterator operator+(const unsigned int n) const
		{
			iterator temp(ptrf);
			for (unsigned int i = 0; i < n; ++i)
				temp.ptrf = temp.ptrf->next;
			return temp;
		}

		const iterator &operator+=(const unsigned int n)
		{
			*this = *this + n;
			return *this;
		}

		int operator-(const iterator &p) const
		{
			iterator temp(p);
			unsigned int n = 0;
			while (temp.ptrf != ptrf && temp.ptrf->next != nullptr)
				temp.ptrf = temp.ptrf->next, ++n;
			if (temp.ptrf->next != nullptr)
				return n;
			else
				return -1;
		}

		inline bool operator==(const iterator &p) const
		{
			return (ptrf == p.ptrf);
		}

		inline bool operator!=(const iterator &p) const
		{
			return (ptrf != p.ptrf);
		}

		inline bool operator<(const iterator &p) const
		{
			return (p - *this > 0);
		}

		inline bool operator>(const iterator &p) const
		{
			return (*this - p > 0);
		}

		inline bool operator<=(const iterator &p) const
		{
			return (p - *this >= 0);
		}

		inline bool operator>=(const iterator &p) const
		{
			return (*this - p >= 0);
		}
	};

	LinkList() //构造函数
		: msize(0)
	{
		head = new node;
		head->next = nullptr;
	}

	LinkList(const LinkList &list) //拷贝构造函数
	{
		head = new node;
		*this = list;
	}

	LinkList(const T array[], const unsigned int tsize) //构造函数
		: msize(tsize)
	{
		head = new node;
		node *i = head, *j = head;
		for (unsigned int t = 0; t < msize; ++t)
		{
			j = new node(array[t]);
			i->next = j;
			i = j;
		}
	}

	virtual ~LinkList() //析构函数
	{
		clear();
		head->~node();
	}

	LinkList &operator=(const LinkList &list) //拷贝
	{
		clear();
		msize = list.size();
		node *i = head, *j = head;
		for (iterator t = list.begin(), iEnd = list.end(); t != iEnd; ++t)
		{
			j = new node(*t);
			i->next = j;
			i = j;
		}
		return *this;
	}

	void clear() //清空链表内容
	{
		if (!isEmpty())
		{
			node *i = head->next, *j;
			while (i->next != nullptr)
			{
				j = i, i = i->next;
				j->~node();
			}
			i->~node();
			head->next = nullptr, msize = 0;
		}
	}

	inline unsigned int size() const //获取链表大小
	{
		return msize;
	}

	iterator begin() const //获取头迭代器
	{
		return iterator(head);
	}

	iterator end() const //获取超尾迭代器
	{
		node *pEnd = head;
		while (pEnd->next != nullptr)
			pEnd = pEnd->next;
		return iterator(pEnd);
	}

	bool insert(unsigned int loc, const T &key) //插入元素
	{
		node *i = head;
		while (loc > 0 && i->next != nullptr)
		{
			i = i->next;
			--loc;
		}
		node *j = new node(key, i->next);
		i->next = j;
		++msize;
		return true;
	}

	bool insert(const iterator &i, const T &key) //插入元素
	{
		if (i.ptrf != nullptr)
		{
			node *j = new node(key, i.ptrf->next);
			i.ptrf->next = j;
			++msize;
			return true;
		}
		else
			return false;
	}

	bool del(unsigned int loc) //删除元素
	{
		node *i = head;
		while (loc > 0 && i->next != nullptr)
		{
			i = i->next;
			--loc;
		}
		if (i->next != nullptr)
		{
			node *j = i->next;
			i->next = j->next;
			j->~node();
			--msize;
			return true;
		}
		else
			return false;
	}

	bool del(const iterator &i) //删除元素
	{
		if (i.ptrf == nullptr)
			return false;
		else if (i.ptrf->next != nullptr)
		{
			node *j = i.ptrf->next;
			i.ptrf->next = j->next;
			j->~node();
			--msize;
			return true;
		}
		else
			return false;
	}

	bool pushFront(const T &key) //在链表头插入元素
	{
		node *i = new node(key, head->next);
		head->next = i;
		++msize;
		return true;
	}

	bool popFront() //在链表头删除元素
	{
		if (!isEmpty())
		{
			node *i = head->next;
			head->next = i->next;
			i->~node();
			--msize;
			return true;
		}
		else
			return false;
	}

	bool pushBack(const T &key) //在链表尾插入元素
	{
		node *i = head;
		while (i->next != nullptr)
			i = i->next;
		node *j = new node(key);
		i->next = j;
		++msize;
		return true;
	}

	bool popBack() //在链表尾删除元素
	{
		if (!isEmpty())
		{
			node *i = head, *j;
			while (i->next != nullptr)
				j = i, i = i->next;
			j->next = nullptr;
			i->~node();
			--msize;
			return true;
		}
		else
			return false;
	}

	bool search(iterator &p, const T &key) //查找元素
	{
		for (iterator i = begin(), iEnd = end(); i != iEnd; ++i)
		{
			if (*i == key)
			{
				p = i;
				return true;
			}
		}
		return false;
	}

	inline bool isEmpty() const //判断是否为空表
	{
		return (head->next == nullptr);
	}

	T &operator[](unsigned int loc) const //随机访问
	{
		if (!isEmpty())
		{
			node *i = head->next;
			while ((loc--) > 0 && i->next != nullptr)
				i = i->next;
			return i->key;
		}
		else
			return head->key;
	}

	void forEach(void(*func)(T &)) const //遍历
	{
		node *i = head;
		while (i->next != nullptr)
		{
			i = i->next;
			func(i->key);
		}
	}

	void forEach(const iterator &iBegin, const iterator &iEnd, void(*func)(T &)) const //遍历
	{
		for (iterator i = iBegin; i != iEnd; ++i)
			func(*i);
	}

	void forEach(void(*func)(T *const)) const //遍历
	{
		node *i = head;
		while (i->next != nullptr)
		{
			i = i->next;
			func(&(i->key));
		}
	}

	void forEach(const iterator &iBegin, const iterator &iEnd, void(*func)(T *const)) const //遍历
	{
		for (iterator i = iBegin; i != iEnd; ++i)
			func(&(*i));
	}
};

template <class T>
class CLinkList //循环单链表类
{
protected:
	class node //节点类
	{
		friend class CLinkList<T>;

		friend class CLinkList<T>::iterator;

	protected:
		T key;
		node *next;

	public:
		node() = default; //默认构造函数

		node(node *const pNext) //构造函数
			: next(pNext)
		{
		}

		node(const T &tkey, node *const pNext = nullptr) //构造函数
			: key(tkey), next(pNext)
		{
		}

		virtual ~node() = default; //析构函数
	};

	node *head;
	unsigned int msize;

public:
	class iterator //迭代器类
	{
		friend class CLinkList<T>;

	protected:
		node *ptrf;

		iterator(node *const pf) //构造函数
			: ptrf(pf)
		{
		}

	public:
		iterator() = default; //默认构造函数

		iterator(const iterator &) = default; //拷贝构造函数

		virtual ~iterator() = default; //析构函数

		iterator &operator=(const iterator &) = default; //拷贝

		T &operator*() const
		{
			return ptrf->next->key;
		}

		T &operator[](const unsigned int n) const
		{
			return *(*this + n);
		}

		inline bool isNull() const
		{
			if (ptrf == nullptr)
				return true;
			else
				return (ptrf->next == nullptr);
		}

		const iterator &operator++()
		{
			ptrf = ptrf->next;
			return *this;
		}

		iterator operator++(int)
		{
			iterator temp(ptrf);
			ptrf = ptrf->next;
			return temp;
		}

		iterator operator+(const unsigned int n) const
		{
			iterator temp(ptrf);
			for (unsigned int i = 0; i < n; ++i)
				temp.ptrf = temp.ptrf->next;
			return temp;
		}

		const iterator &operator+=(const unsigned int n)
		{
			*this = *this + n;
			return *this;
		}

		int operator-(const iterator &p) const
		{
			iterator temp(p);
			unsigned int n = 0;
			while (temp.ptrf != ptrf && temp.ptrf->next != nullptr)
				temp.ptrf = temp.ptrf->next, ++n;
			if (temp.ptrf->next != nullptr)
				return n;
			else
				return -1;
		}

		inline bool operator==(const iterator &p) const
		{
			return (ptrf == p.ptrf);
		}

		inline bool operator!=(const iterator &p) const
		{
			return (ptrf != p.ptrf);
		}
	};

	CLinkList() //构造函数
		: msize(0)
	{
		head = new node;
		head->next = nullptr;
	}

	CLinkList(const CLinkList &list) //拷贝构造函数
	{
		head = new node;
		*this = list;
	}

	CLinkList(const T array[], const unsigned int tsize) //构造函数
		: msize(tsize)
	{
		head = new node;
		node *i = head, *j;
		for (unsigned int t = 0; t < msize; ++t)
		{
			j = new node(array[t]);
			i->next = j;
			i = j;
		}
		i->next = head->next;
	}

	virtual ~CLinkList() //析构函数
	{
		clear();
		head->~node();
	}

	CLinkList &operator=(const CLinkList &list) //拷贝
	{
		clear();
		msize = list.size();
		node *i = head, *j = head;
		for (iterator t = list.begin(), iEnd = list.end(); t != iEnd; ++t)
		{
			j = new node(*t);
			i->next = j;
			i = j;
		}
		i->next = head->next;
		return *this;
	}

	void clear() //清空链表内容
	{
		if (!isEmpty())
		{
			node *i = head->next, *j;
			while (i->next != head->next)
			{
				j = i, i = i->next;
				j->~node();
			}
			i->~node();
			head->next = nullptr, msize = 0;
		}
	}

	inline unsigned int size() const //获取链表大小
	{
		return msize;
	}

	iterator begin() const //获取头迭代器
	{
		return iterator(head);
	}

	iterator end() const //获取超尾迭代器
	{
		node *pEnd = head;
		for (unsigned int t = 0; t < msize; ++t)
			pEnd = pEnd->next;
		return iterator(pEnd);
	}

	bool insert(int loc, const T &key) //插入元素
	{
		if (!isEmpty())
		{
			while (loc < 0)
				loc += msize;
			loc %= msize;
			node *i = head;
			while (loc > 0)
			{
				i = i->next;
				--loc;
			}
			node *j = new node(key, i->next);
			i->next = j;
			++msize;
			if (i == head)
			{
				for (loc = 0; loc < msize; ++loc)
					i = i->next;
				i->next = j;
			}
		}
		else
		{
			node *i = new node(key);
			head->next = i, i->next = i;
			msize = 1;
		}
		return true;
	}

	bool insert(const iterator &i, const T &key) //插入元素
	{
		if (i.ptrf != nullptr)
		{
			node *j = new node(key, i.ptrf->next);
			i.ptrf->next = j;
			++msize;
			if (i.ptrf == head)
				end().ptrf->next = j;
			return true;
		}
		else
			return false;
	}

	bool del(int loc) //删除元素
	{
		if (!isEmpty())
		{
			while (loc < 0)
				loc += msize;
			loc %= msize;
			node *i = head;
			while (loc > 0)
			{
				i = i->next;
				--loc;
			}
			node *j = i->next;
			i->next = j->next;
			j->~node();
			--msize;
			if (msize == 0)
				head->next = nullptr;
			else if (i == head)
				end().ptrf->next = head->next;
			return true;
		}
		else
			return false;
	}

	bool del(const iterator &i) //删除元素
	{
		if (i.ptrf == nullptr)
			return false;
		else if (i.ptrf->next != nullptr)
		{
			node *j = i.ptrf->next;
			i.ptrf->next = j->next;
			--msize;
			if (msize == 0)
				head->next = nullptr;
			else if (j == head->next)
				head->next = j->next, end().ptrf->next = j->next;
			j->~node();
			return true;
		}
		else
			return false;
	}

	bool pushFront(const T &key) //在链表头插入元素
	{
		node *i = new node(key, head->next);
		head->next = i;
		end().ptrf->next = i;
		++msize;
		return true;
	}

	bool popFront() //在链表头删除元素
	{
		if (!isEmpty())
		{
			node *i = head->next;
			head->next = i->next;
			i->~node();
			--msize;
			if (msize == 0)
				head->next = nullptr;
			else
				end().ptrf->next = head->next;
			return true;
		}
		else
			return false;
	}

	bool pushBack(const T &key) //在链表尾插入元素
	{
		node *i = head;
		for (unsigned int t = 0; t < msize; ++t)
			i = i->next;
		node *j = new node(key);
		i->next = j, j->next = head->next;
		++msize;
		return true;
	}

	bool popBack() //在链表尾删除元素
	{
		if (!isEmpty())
		{
			node *i = head, *j;
			for (unsigned int t = 0; t < msize; ++t)
				j = i, i = i->next;
			j->next = head->next;
			i->~node();
			--msize;
			if (msize == 0)
				head->next = nullptr;
			return true;
		}
		else
			return false;
	}

	bool search(iterator &p, const T &key) //查找元素
	{
		for (iterator i = begin(), iEnd = end(); i != iEnd; ++i)
		{
			if (*i == key)
			{
				p = i;
				return true;
			}
		}
		return false;
	}

	inline bool isEmpty() const //判断是否为空表
	{
		return (head->next == nullptr);
	}

	T &operator[](int loc) const //随机访问
	{
		if (!isEmpty())
		{
			while (loc < 0)
				loc += msize;
			loc %= msize;
			node *i = head->next;
			while ((loc--) > 0)
				i = i->next;
			return i->key;
		}
		else
			return head->key;
	}

	void forEach(void(*func)(T &)) const //遍历
	{
		node *i = head;
		for (unsigned int t = 0; t < msize; ++t)
		{
			i = i->next;
			func(i->key);
		}
	}

	void forEach(const iterator &iBegin, const iterator &iEnd, void(*func)(T &)) const //遍历
	{
		for (iterator i = iBegin; i != iEnd; ++i)
			func(*i);
	}

	void forEach(void(*func)(T *const)) const //遍历
	{
		node *i = head;
		for (unsigned int t = 0; t < msize; ++t)
		{
			i = i->next;
			func(&(i->key));
		}
	}

	void forEach(const iterator &iBegin, const iterator &iEnd, void(*func)(T *const)) const //遍历
	{
		for (iterator i = iBegin; i != iEnd; ++i)
			func(&(*i));
	}
};

#endif //DATASTRUCTURE_LINKLIST_H