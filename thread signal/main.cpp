#include <chrono>
#include <iostream>
#include <thread>
#include "ThreadSignal.hpp"

int main() {
    ThreadSignal ts;

    std::thread t([&] {
        ts.wait_and_hold([] { std::cout << "Holding lock\n"; });
        std::cout << "The wait is over\n";
    });

    std::cout << "Waiting..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    ts.notify_one();

    std::this_thread::sleep_for(std::chrono::seconds(1));
    ts.reset();
    std::cout << "TS reset" << std::endl;

    t.join();
}
