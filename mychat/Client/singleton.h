#ifndef SINGLETON_H
#define SINGLETON_H
#include <global.h>
#include <iostream>
template <typename T>
class Singleton{
protected:
    Singleton() = default;
    Singleton(const Singleton<T>&) = delete;
    Singleton& operator = (const Singleton<T>& st) = delete;
    static std::shared_ptr<T> _instance;
public:
    static std::shared_ptr<T> GetInstance(){
        static std::once_flag s_flag;   //只生成一次
        std::call_once(s_flag, [&](){   //第一次默认false， 第二次运行
            _instance = std::shared_ptr<T>(new T);    //不能用makeshare是因为构造函数是protected，new可以直接用

        });
        return _instance;
    }
    void PrintAddress(){
        std::cout << _instance.get() << std::endl;
    }
    ~Singleton(){
        std::cout << "this is singleton destruct." << std::endl;
    }
};

template <typename T>
std::shared_ptr<T> Singleton<T>::_instance = nullptr;

#endif // SINGLETON_H
