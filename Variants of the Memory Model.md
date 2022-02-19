# Variants of the Memory Model

We already know C++ has six variants of the memory models. The default for atomic operations is `std::memory_order_seq_cst`; this expression stands for sequential consistency.

Sequential consistency has two key characteristics:

1. The instructions of a program are executed in source code order
2. There is a global order of all operations on all threads

In addition, you can explicitly specify one of the other five. So what does C++ have to offer?

```cpp
enum memory_order {
  memory_order_relaxed,
  memory_order_consume,
  memory_order_acquire,
  memory_order_release,
  memory_order_acq_rel,
  memory_order_seq_cst
}
```

To classify these memory models, it helps to answer two questions:

1. Which kind of atomic operations should use which memory model?
2. Which synchronization and ordering constraints are defined by these models?

## Kinds of Atomic Operations

There are three different kinds of operations:

* Read operation: `memory_order_acquire` and `memory_order_consume`
* Write operation: `memory_order_release`
* Read-write operation: `memory_order_acq_rel` and `memory_order_seq_cst`

`memory_order_relaxed` defines no synchronization and ordering constraints; therefore, it does not fit in this taxonomy. The following table orders the atomic operations based on their reading and/or writing characteristics.

| **Operation**                                         | **read** | **write** | **read-write** |
|:-----------------------------------------------------:|:--------:|:---------:|:--------------:|
| `test_and_set`                                        |          |           | +              |
| `clear`                                               |          | +         |                |
| `is_lock_free`                                        | +        |           |                |
| `load`                                                | +        |           |                |
| `store`                                               |          | +         |                |
| `exchange`                                            |          |           | +              |
| `compare_exchange_strong` `compare_exchange_weak`     |          |           | +              |
| `fetch_add`, `+=` `fetch_sub`, `-=`                   |          |           | +              |
| `fetch_or`, `\|=` `fetch_and`, `&=` `fetch_xor`, `^=` |          |           | +              |
| `++`, `--`                                            |          |           | +              |

If you use an atomic operation `atomVar.load()` with a memory model that is designed for a write or read-write operation, the write part has no effect. 

The result is that:

* Operation `atomVar.load(std::memory_order_seq_cst)` is equivalent to `atomVar.load(std::memory_order_acquire)`;

* Operation `atomVar.load(std::memory_order_release)` is equivalent to `atomVar.load(std::memory_order_relaxed)`.

## Types of Synchronization & Ordering Constraints

There are, roughly speaking, three different types of synchronization and ordering constraints in C++:

* Sequential consistency: `memory_order_seq_cst`
* Acquire-release: `memory_order_consume`, `memory_order_acquire`, `memory_order_release` and `memory_order_acq_rel`
* Relaxed: `memory_order_relaxed`

### Modification Order Consistency

While the sequential consistency establishes a global order between threads, the acquire-release semantic establishes an ordering between **read** and **write** operations on the same atomic variable with different threads. The relaxed semantic only guarantees that operations on one specific data type in the same thread cannot be reordered. This guarantee is called modification order consistency, but other threads can see this operation in a different order.


