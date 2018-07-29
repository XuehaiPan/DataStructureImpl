//
// Created by 潘学海.
//

#ifndef DATASTRUCTURE_SEQSTACK_H
#define DATASTRUCTURE_SEQSTACK_H

#ifdef MAXSIZE
#undef MAXSIZE
#endif // MAXSIZE

#define MAXSIZE 100

template <class T>
class SeqStack //顺序栈类
{
protected:
	T *key;
	unsigned int mtop, maxsize;

public:
	SeqStack(const unsigned int tsize = MAXSIZE) //构造函数
		: mtop(0), maxsize(tsize)
	{
		key = new T[maxsize];
	}

	SeqStack(const SeqStack &stack) //拷贝构造函数
		: key(nullptr)
	{
		*this = stack;
	}

	SeqStack(const T array[], const unsigned int tsize) //构造函数
		: mtop(0), maxsize(tsize + MAXSIZE)
	{
		key = new T[maxsize];
		for (unsigned int i = 0; i < mtop; ++i)
			key[i] = array[i];
	}

	virtual ~SeqStack() //析构函数
	{
		delete[] key;
		key = nullptr;
	}

	SeqStack &operator=(const SeqStack &stack) //拷贝
	{
		resize(stack.maxsize);
		mtop = stack.size();
		for (unsigned int i = 0; i < mtop; ++i)
			key[i] = stack.key[i];
		return *this;
	}

	void clear() //清空栈内容
	{
		mtop = 0;
		delete[] key;
		key = new T[maxsize];
	}

	void resize(const unsigned int tsize = MAXSIZE) //重设栈大小
	{
		mtop = 0, maxsize = tsize;
		delete[] key;
		key = new T[maxsize];
	}

	inline unsigned int size() const //获取栈大小
	{
		return mtop;
	}

	bool pushBack(const T &tkey) //在栈顶插入元素
	{
		if (!isFull())
		{
			key[mtop] = tkey;
			++mtop;
			return true;
		}
		else
			return false;
	}

	bool popBack() //在栈顶删除元素
	{
		if (!isEmpty())
		{
			--mtop;
			return true;
		}
		else
			return false;
	}

	bool popBack(T &tkey) //在栈顶删除元素
	{
		if (!isEmpty())
		{
			--mtop;
			tkey = key[mtop];
			return true;
		}
		else
			return false;
	}

	const T &top() const //获取栈顶元素
	{
		if (!isEmpty())
			return key[mtop - 1];
	}

	bool top(T &tkey) const //获取栈顶元素
	{
		if (!isEmpty())
		{
			tkey = key[mtop - 1];
			return true;
		}
		else
			return false;
	}

	inline bool isEmpty() const //判断是否为空栈
	{
		return (mtop == 0);
	}

	inline bool isFull() const //判断栈是否已满
	{
		return (mtop == maxsize);
	}

	void forEach(void(*func)(const T &)) const //遍历
	{
		for (unsigned int i = 0; i < mtop; ++i)
			func(key[i]);
	}

	void forEach(void(*func)(T const *const)) const //遍历
	{
		for (unsigned int i = 0; i < mtop; ++i)
			func(&key[i]);
	}
};

#endif //DATASTRUCTURE_SEQSTACK_H