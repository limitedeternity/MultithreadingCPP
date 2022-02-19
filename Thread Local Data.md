# Thread Local Data

Thread-local data, also known as thread-local storage, will be created for each thread separately. It behaves like static data because it’s bound for the lifetime of the thread and it will be created at its first usage. Also, thread-local data belongs exclusively to the thread.

```cpp
thread_local std::string hello("hello from ");
std::mutex cout_mutex;

void thread_func(const std::string_view thread_name) {
    hello.append(thread_name);

    std::lock_guard<std::mutex> guard(cout_mutex);
    std::cout << hello << std::endl;
    std::cout << "&hello: " << &hello << std::endl;
}
```

> **From a Single-Threaded to Multithreaded Program**
> 
> Thread-local data helps to port a single-threaded program to a multithreaded environment. If global variables are thread-local, there is a guarantee that each thread will get its own copy of the data. Due to this fact, there is no shared mutable state which may cause a data race resulting in undefined behavior.


