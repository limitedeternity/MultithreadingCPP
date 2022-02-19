# Common Advises

## Minimize waiting

Waiting has at least two drawbacks. First, when a thread waits it cannot make any progress; therefore, your performance goes down. Second, the more waiting you 
have in your program in order to synchronize threads, the more likely it will become that a bad interleaving of waiting periods causes a deadlock.

## Prefer immutable data

A requirement for getting a data race is mutable data. If you have immutable data, no data race can happen. You only have to guarantee that the immutable data will be initialized in a thread-safe way.

Functional programming languages such as Haskell, having no mutable data, are very suitable for concurrent programming.

## Look for the right abstraction

Reinventing the wheel is a big challenge in various aspects: you have to implement it, maintain it, and explain it to your co-workers. 

For instance, instead of implementing a parallel loop for summing up a container, use `std::reduce`. The more you go for the right abstraction, the less likely it will become that you shoot yourself in the foot.

## Don’t use volatile for synchronization

In C++, in contrast to C# or Java, `volatile` has no multithreading semantic. In C# or Java, `volatile` declares an atomic as does `std::atomic` in C++. 

In C++ `volatile` keyword is used to declare objects which can change independently of the regular program flow.

## If you program Lock-Free, use well-established patterns

If you have identified a bottleneck that could benefit from a lock-free solution, apply established patterns.

1. Sharing an atomic boolean or an atomic counter is straightforward.

2. Use a thread-safe or even lock-free container to support consumer/producer workflow. If your container is thread-safe, you can put and get values from the container without worrying about synchronization; you simply shift the application challenges to the infrastructure.

## If possible, use tasks instead of threads

Based on the program, there are a lot of reasons for preferring tasks over threads. These are the main reasons:

* You can use a safe communication channel for returning the result of the communication. If you use a shared variable, you have to synchronize the access to it;
* You can quite easily return values, notifications, and exceptions to the caller.

## Consider using an auto-joining thread

A thread `t` with a callable unit is called joinable if neither a `t.join()` nor a `t.detach()` call happened. The destructor of a joinable thread invokes `std::terminate`.

In order not to forget the `t.join()`, you can create your own wrapper around `std::thread`. This wrapper checks in the constructor, if the given thread is still joinable, and joins it in the destructor.

## If possible, use Promises and Futures instead of Condition Variables

For one-time notifications, promises and futures are a better choice. If your workflow requires that you use a condition variable many times,​ then a promise and a future pair is no alternative.
