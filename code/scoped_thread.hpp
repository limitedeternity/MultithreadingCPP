#pragma once
#include <thread>

struct scoped_thread : std::thread {

    using thread::thread;

    ~scoped_thread() noexcept {
        if (joinable()) {
            join();
        }
    }
};
