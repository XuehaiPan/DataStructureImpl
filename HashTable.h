//
// Created by 潘学海.
//

#ifndef DATASTRUCTURE_HASHTABLE_H
#define DATASTRUCTURE_HASHTABLE_H

template <class KeyType, class DataType>
class HashTable //散列表类
{
protected:
	class element //元素类
	{
		friend class HashTable<KeyType, DataType>;

	protected:
		KeyType mkey;
		DataType mdata;
		bool misDeleted;

	public:
		element() //默认函数
			: misDeleted(false)
		{
		}

		element(KeyType tkey, DataType tdata) //构造函数
			: mkey(tkey), mdata(tdata), misDeleted(false)
		{
		}

		virtual ~element() = default; //析构函数

		element &operator=(const element &tele) //拷贝
		{
			mkey = tele.mkey;
			mdata = tele.mdata;
			misDeleted = tele.misDeleted;
			return *this;
		}

		const KeyType &key() const //获取关键码
		{
			return mkey;
		}

		const DataType &data() const //获取数据
		{
			return mdata;
		}

		bool del() //删除记录
		{
			if (!misDeleted)
			{
				misDeleted = true;
				return true;
			}
			else
				return false;
		}

		inline bool isDeleted() const //判断是否为已删记录
		{
			return misDeleted;
		}
	};

	element **mtable;

	unsigned int (*hash)(const KeyType &);

	unsigned int msize, maxsize, mcollisions;

	static unsigned int defaultHashFunction(const KeyType &tkey) //默认散列函数
	{
		return 0;
	}

	static bool isValid(const element *const ptr) const
	{
		if (ptr != nullptr)
		{
			if (!(ptr->isDeleted()))
				return true;
		}
		return false;
	}

	unsigned int search(const KeyType &tkey) const //查找元素，若找到返回元素位置，否则返回可插入位置
	{
		unsigned int loc = hash(tkey) % maxsize;
		for (unsigned int i = 0; i < maxsize; ++i)
		{
			if (isValid(mtable[(loc + i) % maxsize]))
			{
				if (((mtable[(loc + i) % maxsize])->key()) == tkey)
					return ((loc + i) % maxsize);
			}
			else
				return ((loc + i) % maxsize);
		}
		return loc;
	}

	bool insert(const element &tele) //插入元素
	{
		return insert(tele.key(), tele.data());
	}

public:
	HashTable(const unsigned int tsize = MAXSIZE) //构造函数
		: msize(0), maxsize(tsize), mcollisions(0), hash(defaultHashFunction)
	{
		mtable = new element *[maxsize];
		for (unsigned int i = 0; i < maxsize; ++i)
			mtable[i] = nullptr;
	}

	HashTable(const unsigned int(*hashFunc)(const KeyType &), const unsigned int tsize = MAXSIZE) //构造函数
		: msize(0), maxsize(tsize), mcollisions(0), hash(hashFunc)
	{
		mtable = new element *[maxsize];
		for (unsigned int i = 0; i < maxsize; ++i)
			mtable[i] = nullptr;
	}

	HashTable(const HashTable &ttable) //拷贝构造函数
		: mtable(nullptr)
	{
		*this = ttable;
	}

	virtual ~HashTable() //析构函数
	{
		clear();
		delete[] mtable;
	}

	HashTable &operator=(const HashTable &ttable) //拷贝
	{
		resize(ttable.maxsize);
		hash = ttable.hash;
		for (unsigned i = 0; i < maxsize; ++i)
		{
			if (isValid(ttable[i]))
				insert(ttable[i]->mkey, ttable[i]->mdata);
		}
		return *this;
	}

	void clear() //清空散列表内容
	{
		for (unsigned int i = 0; i < maxsize; ++i)
		{
			if (mtable[i] != nullptr)
				mtable[i]->~element();
		}
		msize = 0, mcollisions = 0;
	}

	void resize(const unsigned int tsize = MAXSIZE) //重设散列表大小
	{
		clear();
		delete[] mtable;
		maxsize = tsize;
		mtable = new element *[maxsize];
		for (unsigned int i = 0; i < maxsize; ++i)
			mtable[i] = nullptr;
	}

	inline unsigned int size() const //获取散列表大小
	{
		return msize;
	}

	double loadFactor() const //获取散列表负载因子
	{
		return double(msize) / maxsize;
	}

	inline unsigned int collisions() const //获取散列表碰撞次数
	{
		return mcollisions;
	}

	bool insert(const KeyType &tkey, const DataType &tdata) //插入元素
	{
		if (!isFull())
		{
			unsigned int loc = search(tkey);
			delete mtable[loc];
			mtable[loc] = new element(tkey, tdata);
			++msize;
			mcollisions += (loc + maxsize - hash(tkey) % maxsize) % maxsize;
			return true;
		}
		else
			return false;
	}

	bool del(const KeyType &tkey) //删除元素
	{
		unsigned int loc = search(tkey);
		if (isValid(mtable[loc]))
		{
			if ((mtable[loc]->key()) == tkey)
			{
				mtable[loc]->del();
				--msize;
				return true;
			}
		}
		return false;
	}

	bool search(const KeyType &tkey, DataType &tdata) const //查找元素
	{
		unsigned int loc = search(tkey);
		if (isValid(mtable[loc]))
		{
			if ((mtable[loc]->key()) == tkey)
			{
				tdata = mtable[loc]->data();
				return true;
			}
		}
		return false;
	}

	inline bool isEmpty() const //判断是否为空表
	{
		return (msize == 0);
	}

	inline bool isFull() const //判断散列表是否已满
	{
		return (msize == maxsize);
	}

	void forEach(void(*func)(const KeyType &)) const //遍历
	{
		for (unsigned int i = 0; i < maxsize; ++i)
		{
			if (isValid(mtable[i]))
				func(mtable[i]->key());
		}
	}

	void forEach(void(*func)(const KeyType *const)) const //遍历
	{
		for (unsigned int i = 0; i < maxsize; ++i)
		{
			if (isValid(mtable[i]))
				func(&(mtable[i]->key()));
		}
	}

	void forEach(void(*func)(const DataType &)) const //遍历
	{
		for (unsigned int i = 0; i < maxsize; ++i)
		{
			if (isValid(mtable[i]))
				func(mtable[i]->data());
		}
	}

	void forEach(void(*func)(const DataType *const)) const //遍历
	{
		for (unsigned int i = 0; i < maxsize; ++i)
		{
			if (isValid(mtable[i]))
				func(&(mtable[i]->data()));
		}
	}

	void forEach(void(*func)(const KeyType &, const DataType &)) const //遍历
	{
		for (unsigned int i = 0; i < maxsize; ++i)
		{
			if (isValid(mtable[i]))
				func(mtable[i]->key(), mtable[i]->data());
		}
	}

	void forEach(void(*func)(const KeyType *const, const DataType &)) const //遍历
	{
		for (unsigned int i = 0; i < maxsize; ++i)
		{
			if (isValid(mtable[i]))
				func(&(mtable[i]->key()), mtable[i]->data());
		}
	}

	void forEach(void(*func)(const KeyType &, const DataType *const)) const //遍历
	{
		for (unsigned int i = 0; i < maxsize; ++i)
		{
			if (isValid(mtable[i]))
				func(mtable[i]->key(), &(mtable[i]->data()));
		}
	}

	void forEach(void(*func)(const KeyType *const, const DataType *const)) const //遍历
	{
		for (unsigned int i = 0; i < maxsize; ++i)
		{
			if (isValid(mtable[i]))
				func(&(mtable[i]->key()), &(mtable[i]->data()));
		}
	}
};

#endif //DATASTRUCTURE_HASHTABLE_H