#include <iostream>
#include <condition_variable>
#include <mutex>

#include "scoped_thread.hpp"

std::mutex g_mutex;
std::condition_variable g_condvar;

bool is_ready = false;

void waiting_worker() {
    std::cout << "Worker: Waiting for data." << std::endl;

    std::unique_lock<std::mutex> lck(g_mutex);
    g_condvar.wait(lck, []{ return is_ready; });

    std::cout << "Worker: Processing shared data." << std::endl;
    std::cout << "Worker: Work done." << std::endl;
}

void prepare_data() {
    std::cout << "Sender: Preparing data." << std::endl;
    {
        std::lock_guard<std::mutex> lck(g_mutex);
        is_ready = true;
    }

    std::cout << "Sender: Data is ready."  << std::endl;
    g_condvar.notify_all();
}

int main() {
    scoped_thread t1(waiting_worker);
    scoped_thread t2(prepare_data);
}
