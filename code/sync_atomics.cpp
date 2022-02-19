#include <atomic>
#include <thread>
#include <iostream>
#include <string>

std::atomic<std::string*> ptr;
std::atomic<int> atoData;

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
    std::thread t1(producer);
    std::thread t2(consumer);
    t1.join();
    t2.join();

    return 0;
}
