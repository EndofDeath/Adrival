#ifndef ADRIVAL_SINGLETONT_H
#define ADRIVAL_SINGLETONT_H

namespace adrival{

template<class T>
class singletonT
{
public:
	static T* GetInstance();
	operator bool();
	bool operator!();
protected:
	singletonT() = default;
	virtual ~singletonT() = default;
private:
	singletonT(const singletonT<T>&) = delete;
	singletonT& operator=(const singletonT<T>&) = delete;
};

template<class T>
T* singletonT<T>::GetInstance()
{
	static T theone;
	return &theone;
}

template<class T>
singletonT<T>::operator bool()
{
	return nullptr != GetInstance();
}

template<class T>
bool singletonT<T>::operator!()
{
	return nullptr == GetInstance();
}

}

#define DECLARATION_SINGLETON(classname)\
	friend class adrival::singletonT<classname>;\
	private:\
	classname() = default;\
	~classname() = default;\
	classname(const classname&) = delete;\
	classname& operator=(const classname&) = delete;\

#endif //!ADRIVAL_SINGLETONT_H