# Condition Variables

Condition variables enable threads to be synchronized via messages. They need the `<condition_variable>` header, one thread to act as the sender, and the other as the receiver of the message; the receiver waits for the notification from the sender. Typical use cases for condition variables are sender-receiver or producer-consumer workflows.

## The Wait Workflow

If it's the first time `wait` is invoked, the following steps will happen:

* The call to `wait` locks the mutex and checks if the predicate `[] { return is_ready; }` evaluates to true.
  
  * If it does, the condition variable unlocks the mutex and continues.
  
  * Else, the condition variable unlocks the mutex and puts itself back in the wait state.

Subsequent `wait` calls behave differently:

* The waiting thread gets a notification. It locks the mutex and checks if the predicate `[] { return is_ready; }` evaluates to true.
  
  * If it does, the condition variable unlocks the mutex and continues.
  
  * Else, the condition variable unlocks the mutex and puts itself back in the wait state.

Maybe you are wondering why you need a predicate for the `wait` call when you can invoke `wait` without a predicate? The predicate is a kind of memory for the stateless condition variable; therefore, the `wait` call always checks the predicate at first.

Condition variables are victim to two known phenomena: lost wakeup and spurious wakeup.

## Lost Wakeup

The phenomenon of the lost wakeup is that the sender sends its notification before the receiver gets to a wait state. The consequence is that the notification is lost. The C++ standard describes condition variables as a simultaneous synchronization mechanism: “The condition_variable class is a synchronization primitive that can be used to block a thread, or multiple threads *at the same time*, …”. So, the notification gets lost and the receiver is waiting, and waiting, and…

## Spurious Wakeup

It can happen that the receiver wakes up, although no notification happened. At a minimum, [POSIX Threads](https://en.wikipedia.org/wiki/POSIX_Threads) and the [Windows API](https://en.wikipedia.org/wiki/Windows_API) can be victims of these phenomena.

In most of the use-cases, tasks are the less error-prone way to synchronize threads.
