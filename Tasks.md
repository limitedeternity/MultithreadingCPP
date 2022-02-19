# Tasks

In addition to threads, C++ has tasks to perform work asynchronously. These tasks need the `<future>` header, and they will be parameterized with a work package. Additionally, they consist of two associated components: a promise and a future; both are connected via a data channel. The promise executes the work packages and puts the result in the data channel; the associated future picks up the result. Both communication endpoints can run in separate threads. What is special is that the future can pick up the result at a later time; therefore, the calculation of the result by the promise is independent of the query of the result by the associated future.

## Threads vs Tasks

Communication between the creator thread and the created thread requires the use of a shared variable. The task communicates via its data channel which is implicitly protected; therefore, a task is not obliged to use a protection mechanism like a mutex.

The future can request the result of the task only once (by calling `get`). Calling it more than once results in an undefined behavior. This is not true for a `std::shared_future`, which can be queried multiple times.

The creator thread waits for its child with the call to `join`. The future uses the `get` call which blocks until the result is available. If an exception is thrown in the created thread, `std::terminate` will be invoked. In contrast, the promise can send the exception to the future, which can handle the exception.

A promise can serve one or many futures, and it can send a value, an exception, or just a notification. In addition, you can use one as a safe replacement for a condition variable, if you use it for only one synchronization.

`std::async` is the easiest way to create a future.

## std::async

`std::async` behaves like an asynchronous function call. This function call takes a [callable](https://en.cppreference.com/w/cpp/named_req/Callable) together with its arguments. The call to `std::async` returns a future object. That’s like a handle for getting the result via `get` call.

> The C++ runtime decides if `std::async` is executed in a separate thread. The decision of the C++ runtime may depend on the number of CPU cores available, the utilization of your system, or the size of your work package. By using `std::async`, you only specify the task that should run; the C++ runtime automatically manages the creation and the lifetime of the thread.

Optionally, you can specify a start policy for `std::async`.

## std::async: Start Policy

With the start policy you can explicitly specify whether the asynchronous call should be executed in the same thread (`std::launch::deferred`) or in another thread (`std::launch::async`).

> **Eager versus lazy evaluation**
> 
> Eager and lazy evaluations are two orthogonal strategies to calculate the result of an expression. In the case of [eager evaluation](https://en.wikipedia.org/wiki/Eager_evaluation), the expression will be evaluated immediately. In the case of [lazy evaluation](https://en.wikipedia.org/wiki/Lazy_evaluation), the expression will only be evaluated if needed. Eager evaluation is often called greedy evaluation and lazy evaluation is often called call-by-need. With lazy evaluation, you save time and compute power because there is no evaluation on suspicion.

By default, `std::async` executes its work package immediately. What is special about the call `auto fut = std::async(std::launch::deferred, ... )` is that the promise will not be executed immediately. The call `fut.get()` starts the promise lazily; i.e. the promise will only run if the future asks for the result.

You do not have to bind a future to a variable.

## std::async: Fire and Forget

Fire and forget futures are special futures; they execute just in place because their future is not bound to a variable. For a “fire and forget” future, it is necessary that the promise runs in a separate thread so it can immediately start its work. This is done by the `std::launch::async` policy.

Fire and forget futures look very promising, but have a big drawback. A future that is created by `std::async` waits on its destructor, until its promise is done. In this context, it's not very different from blocking.

That being said, `std::async` is a convenient mechanism used to distribute a bigger compute job on more shoulders.

## std::packaged_task

`std::packaged_task` is a wrapper for a [callable](https://en.cppreference.com/w/cpp/named_req/Callable) in order for it to be invoked asynchronously.

1. Wrap a callable:
   
   ```cpp
   std::packaged_task<int(int, int)> sumTask([](int a, int b){ return a + b; });
   ```

2. Create a future:
   
   ```cpp
   std::future<int> sumResult = sumTask.get_future();
   ```

3. Invoke the task:
   
   ```cpp
   sumTask(2000, 11);
   ```

4. Query the result:
   
   ```cpp
   sumResult.get();
   ```

## std::promise

`std::promise` enables you to set a value, a notification, or an exception. In addition, the promise can provide its result in a delayed fashion.

| **Method**                              | **Description**                                               |
|:---------------------------------------:|:-------------------------------------------------------------:|
| `prom.get_future()`                     | Returns the future.                                           |
| `prom.set_value(val)`                   | Sets the value.                                               |
| `prom.set_exception(ex)`                | Sets the exception.                                           |
| `prom.set_value_at_thread_exit(val)`    | Stores the value and makes it ready if the promise exits.     |
| `prom.set_exception_at_thread_exit(ex)` | Stores the exception and makes it ready if the promise exits. |

If a value or an exception is assigned to the promise more than once, `std::future_error` exception is thrown.

## std::future

`std::future` enables you to

* pick up the value from the promise;
* ask the promise if the value is available;
* wait for the notification from the promise;
* create a shared future (`std::shared_future`).

| **Method**                | **Description**                                                                  |
|:-------------------------:|:--------------------------------------------------------------------------------:|
| `fut.share()`             | Returns a `std::shared_future`. Afterwards, the result is not available anymore. |
| `fut.get()`               | Returns the result which can be a value or an exception.                         |
| `fut.valid()`             | Checks if the result is available. After calling `fut.get()` it returns false.   |
| `fut.wait()`              | Waits for the result.                                                            |
| `fut.wait_for(relTime)`   | Waits for the result, but not longer than for a `relTime`.                       |
| `fut.wait_until(absTime)` | Waits for the result, but not longer than until `abstime`.                       |

If the future `fut` asks for the result more than once, `std::future_error` exception is thrown.

There is a one-to-one relationship between the promise and the future. In contrast, `std::shared_future` supports one-to-many relations between the promise and many futures. 

In addition to the `std::future`, a `std::shared_future` enables you to query the promise independently of the other associated futures. Also, unlike `std::future`, `std::shared_future` can be copied.
