# Operation atomicity

| **Method**                                            | **`atomic_flag`** | **`atomic<bool>`** | **`atomic<T*>`** | **`atomic<int>`** | **`atomic<T>`** |
|:-----------------------------------------------------:|:-----------------:|:------------------:|:----------------:|:-----------------:|:---------------:|
| `test_and_set`                                        | yes               | no                 | no               | no                | no              |
| `clear`                                               | yes               | no                 | no               | no                | no              |
| `is_lock_free`                                        | yes               | no                 | no               | no                | no              |
| `load`                                                | no                | yes                | yes              | yes               | yes             |
| `store`                                               | no                | yes                | yes              | yes               | yes             |
| `exchange`                                            | no                | yes                | yes              | yes               | yes             |
| `compare_exchange_strong` `compare_exchange_weak`     | no                | yes                | yes              | yes               | yes             |
| `fetch_add`, `+=` `fetch_sub`, `-=`                   | no                | no                 | yes              | yes               | no              |
| `fetch_or`, `\|=` `fetch_and`, `&=` `fetch_xor`, `^=` | no                | no                 | no               | yes               | no              |
| `++`, `--`                                            | no                | no                 | yes              | yes               | no              |
