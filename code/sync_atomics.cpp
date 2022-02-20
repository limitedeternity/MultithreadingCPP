/*
 * CppMem equivalent:

int main() {
  atomic_int ptr = 0;
  atomic_int atoData = 0;

  {{{ {
      int p = 0x1337;
      atoData.store(2014, mo_relaxed);
      ptr.store(p, mo_release);
      }
  |||
      {
      int p2;
      p2 = ptr.load(mo_acquire).readsvalue(0x1337);
      atoData.load(mo_relaxed);
      }
  }}}

  return 0;
}
*/

#include <atomic>
#include <iostream>
#include <string>

#include "scoped_thread.hpp"

std::atomic<std::string*> ptr = nullptr;
std::atomic<int> atoData = 0;

void producer() {
    auto p = new std::string("Sample text");

    // 1. This happens-before...
    atoData.store(2014, std::memory_order_relaxed);

    // 2. ...this. This synchronizes-with...
    ptr.store(p, std::memory_order_release);
}

void consumer() {
    std::string* p2;

    // 3. ...this. This happens-before...
    while (!(p2 = ptr.load(std::memory_order_acquire))) {
        __asm volatile ("pause" ::: "memory");
    }

    std::cout << "*p2: " << *p2 << std::endl;
    free(p2);

    // 4. ...this.
    std::cout << "atoData: " << atoData.load(std::memory_order_relaxed) << std::endl;
}

int main(void) {
    scoped_thread{std::thread(producer)};
    scoped_thread{std::thread(consumer)};

    return 0;
}
