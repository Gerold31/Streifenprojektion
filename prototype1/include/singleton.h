#ifndef SINGLETON_H
#define SINGLETON_H

template<typename T>
class Singleton
{
public:
    static T *getSingleton() {if(!mSingleton) mSingleton = new T(); return mSingleton;}

	Singleton(const Singleton &) = delete;
	Singleton<T> &operator=(Singleton<T> &) = delete;

protected:
	Singleton() {}
    virtual ~Singleton() {}

private:
    static T *mSingleton;
};

template <typename T> T *Singleton<T>::mSingleton = 0;

#endif
