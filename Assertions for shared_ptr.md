# Assertions for std::shared_ptr

`std::shared_ptr` is the only non-atomic data type on which you can apply atomic operations.

Atomic operations on `std::shared_ptr` will increase and decrease the reference-counter in a thread-safe way because the control block of `std::shared_ptr` is thread-safe. You also have the guarantee that the resource will be destroyed exactly once.

The assertions that a `std::shared_ptr` provides are described by [Boost](http://www.boost.org/doc/libs/1_57_0/libs/smart_ptr/shared_ptr.htm#ThreadSafety):

1. **Each** shared_ptr instance can be “read” (accessed using only const operations) simultaneously by multiple threads.

2. **Different** shared_ptr instances can be “written to” (accessed using mutable operations such as `operator=` or `reset`) simultaneously by multiple threads (even when these instances are copies, and share the same reference count underneath).

## Example

```cpp
int main(void) {
  std::shared_ptr<int> ptr = std::make_shared<int>(2011);

  for (auto i = 0; i < 10; ++i) {
     std::thread([ptr] {                         
       std::shared_ptr<int> localPtr(ptr);       
       localPtr = std::make_shared<int>(2014);
       std::cout << "localPtr: " << (*localPtr) << std::endl;
     }).detach(); 
  }
}
```

When you capture `std::shared_ptr ptr` by value into thread's closure, all is fine: copy construction of `localPtr` is thread-safe (because only the control block is used) and the modification of `localPtr` takes place on a copy.

However, if you'll capture `std::shared_ptr ptr` by reference, the assignment would be a race condition on the resource.
