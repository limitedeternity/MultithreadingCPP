#pragma once
#include <cassert>
#include <thread>


class scoped_thread {
private:
    std::thread m_thread;

public:
    explicit scoped_thread(std::thread&& t) :
        m_thread(std::move(t))
    {
        assert(m_thread.joinable());
    }

    ~scoped_thread() {
        m_thread.join();
    }

    scoped_thread(scoped_thread&) = delete;
    scoped_thread& operator=(scoped_thread const&) = delete;
};
