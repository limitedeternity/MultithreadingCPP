# Thread-Safe Initialization

If the variable is never modified there is no need for synchronization by using an expensive lock or an atomic. You only have to ensure that it is initialized in a thread-safe way.

There are three ways in C++ to initialize variables in a thread-safe way:

1. Constant expressions;
2. The function `std::call_once` in combination with the flag `std::once_flag`;
3. A static variable with block scope.

> **Thread-safe initialization in the main thread**
> 
> The easiest and the fourth way to initialize a variable in a thread-safe way: initialize the variable in the main thread before you create any child threads.

## Constant Expressions

Constant expressions are expressions that the compiler can evaluate at compile time; they are implicitly thread-safe. Placing the keyword `constexpr` in front of a variable makes the variable a constant expression. The constant expression must be initialized immediately.

```cpp
constexpr double pi = 3.1415;
```

Additionally, user-defined types can also be constant expressions. For those types, there are a few restrictions that must be met in order to initialize it at compile time:

* They must not have virtual methods or a virtual base class.
* Their constructor must be empty and itself be a constant expression.
* Their methods, which should be callable at compile time, must be constant expressions.

## call_once and once_flag

By using the `std::call_once` function you can register a callable. The `std::once_flag` ensures that only one registered function will be invoked, but you can register additional functions via the same `std::once_flag`. That being said, only one function from that group is called.

`std::call_once` obeys the following rules:

* Exactly one execution of exactly one of the functions is performed. 
  It is undefined which function will be selected for execution. The selected function runs in the same thread as the `std::call_once` invocation it was passed to.
* No invocation in the group returns before the above-mentioned execution of the selected function completes successfully.
* If the selected function exits via an exception, it is propagated to the caller. Another function is then selected and executed.

```cpp
#include <iostream>
#include <thread>
#include <mutex>

std::once_flag onceFlag;

void do_once() {
    std::call_once(onceFlag, []() { std::cout << "Only once." << std::endl; });
}

int main(void) {
    std::thread t1(do_once);
    std::thread t2(do_once);
    std::thread t3(do_once);
    std::thread t4(do_once);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
}
```

## Static Variables with Block Scope

Static variables with block scope will be created exactly once and lazily (i.e. created just at the moment of the usage). This characteristic is the basis of the so-called Meyers Singleton, named after [Scott Meyers](https://en.wikipedia.org/wiki/Scott_Meyers). This is by far the most elegant implementation of the singleton pattern. With C++11, static variables with block scope have an additional guarantee – they will be initialized in a thread-safe way.

```cpp
class MySingleton {
public:
    static MySingleton& getInstance() {
        static MySingleton instance;
        return instance;
    }

private:
    MySingleton() = default;
    ~MySingleton() = default;
    MySingleton(const MySingleton&) = delete;
    MySingleton& operator=(const MySingleton&) = delete;
};
```

> **Know your Compiler support for static**
> 
> If you use the Meyers Singleton pattern in a concurrent environment, be sure that your compiler implements static variables with the C++11 thread-safe semantic. It happens quite often that programmers rely on the C++11 semantic of static variables, but their compiler does not support it. The result may be that more than one instance of a singleton is created.
