#include <iostream>
#include <mutex>
#include <thread>

std::mutex m_ping;
std::mutex m_pong;
const size_t iter = 500000;

void ping()
{
    for(size_t it = 0; it < iter; it++)
    {
        m_ping.lock();
        std::cout << "ping\n";
        m_pong.unlock();
    }
}

void pong()
{
    for(size_t it = 0; it < iter; it++)
    {
        m_pong.lock();
        std::cout << "pong\n";
        m_ping.unlock();
    }
}

int main()
{
    m_pong.lock();
    std::thread t1(ping);
    std::thread t2(pong);
    t1.join();
    t2.join();
    return (0);
}