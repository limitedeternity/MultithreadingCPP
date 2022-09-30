/*
 * CppMem equivalent:
 
int main() {
  atomic_int ptr = 0;
  atomic_int atoData = 0;

  {{{ {
      int p = 0x1337;
      atoData.store(2014, mo_relaxed);
      atomic_thread_fence(mo_release);
      ptr.store(p, mo_relaxed);
      }
  |||
      {
      int p2;
      p2 = ptr.load(mo_relaxed).readsvalue(0x1337);
      atomic_thread_fence(mo_acquire);
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
    std::atomic_thread_fence(std::memory_order_release);

    ptr.store(p, std::memory_order_relaxed);
}

void consumer() {
    std::string* p2;

    while (!(p2 = ptr.load(std::memory_order_relaxed))) {
        __asm volatile ("pause" ::: "memory");
    }

    // 3. ...this.
    std::atomic_thread_fence(std::memory_order_acquire);

    std::cout << "*p2: " << *p2 << std::endl;
    free(p2);

    std::cout << "atoData: " << atoData.load(std::memory_order_relaxed) << std::endl;
}

int main(void) {
    scoped_thread t1(producer);
    scoped_thread t2(consumer);

    return 0;
}
