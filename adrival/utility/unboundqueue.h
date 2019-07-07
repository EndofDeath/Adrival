#ifndef ADRIVAL_UNBOUND_QUEUE_H
#define ADRIVAL_UNBOUND_QUEUE_H

namespace adrival{

template<class T>
class unboundqueue
{
public:
	unboundqueue()
		:root(new block{})
		, head(root)
		, tail(root)
		, maxsize(size)
		, count(0)
	{
		root->next = root;
	}
	~unboundqueue()
	{
		block* next = root;
		do 
		{
			block* free = next;
			next = free->next;
			delete free;
		} while (next != root);
	}

	void push(const T& value)
	{
		grow();
		*tail = value;
		++tail;
		count++;
	}

	bool pop(T& value)
	{
		if (0 == count) return false;
		value = *head;
		++head;
		count--;
		return true;
	}
private:
	static const size_t size = 32;

	struct block
	{
		T data[size]{};
		block* next{};
		T& operator[](size_t index)
		{
			return data[index&(size - 1)];
		}
	};

	class iterator
	{
	public:
		iterator(block* ptr, size_t i=0)
			:p(ptr), index(i), prev(ptr)
		{}
		iterator& operator++()
		{
			index = (index+1)&(size - 1);
			if (0 == index)
			{
				prev = p;
				p = p->next;
			}
			return *this;
		}
		T& operator*()
		{
			return (*p)[index];
		}
		bool half() const
		{
			return index > (size / 2);
		}
		void set_prev(iterator& other)
		{
			prev = other.p;
		}
		void copy_front(block* pnew) const
		{
			for (size_t i = 0; i < index; i++)
			{
				(*pnew)[i] = (*p)[i];
			}
		}
		void copy_back(block* pnew) const
		{
			for (size_t i = index; i < size; i++)
			{
				(*pnew)[i] = (*p)[i];
			}
		}
		bool operator==(iterator& other) const
		{
			return p == other.p&&index == other.index;
		}
		bool operator!=(iterator& other) const
		{
			return p != other.p || index != other.index;
		}
		iterator& operator+=(block* pnew)
		{
			pnew->next = p->next;
			p->next = pnew;
			p = pnew;
			return *this;
		}
		iterator& operator-=(block* pnew)
		{
			pnew->next = p;
			prev->next = pnew;
			p = pnew;
			return *this;
		}
	private:
		block* p;
		size_t index;
		block* prev;
	};
	void grow()
	{
		if (maxsize == count)
		{
			block* newblock = new block;
			if (!tail.half())
			{
				tail.copy_front(newblock);
				tail -= newblock;
			}
			else
			{
				head.copy_back(newblock);
				head += newblock;
			}
			head.set_prev(tail);
			maxsize += size;
		}
	}
	block* root;
	iterator head;
	iterator tail;
	size_t maxsize;
	size_t count;
};

}
#endif // !ADRIVAL_UNBOUND_QUEUE_H