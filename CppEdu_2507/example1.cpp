#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <shared_mutex>
#include <string_view>
#include <condition_variable>
using namespace std::literals;

int share_data = 0;
bool data_ready = false;

std::shared_mutex m;
std::condition_variable_any cv;

void Writer()
{
    while (1)
    {
        {
            std::lock_guard<std::shared_mutex> g(m);

            share_data = share_data + 1;
            std::cout << "Writer : " << share_data << std::endl;
            std::this_thread::sleep_for(1s);
            
            data_ready = true;
        }
        cv.notify_all();
    }
}

void Reader(std::string_view name)
{
    while (1)
    {
        {
            std::shared_lock<std::shared_mutex> ul(m);   

            cv.wait(ul, []() { return data_ready; } );

            std::cout << "Reader(" << name << ") : " << share_data << std::endl;
            std::this_thread::sleep_for(500ms);
        }
        std::this_thread::sleep_for(10ms);
    }
}

int main()
{
    std::thread t1(Writer);
    std::thread t2(Reader, "A");
    std::thread t3(Reader, "B");
    std::thread t4(Reader, "C");
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}



