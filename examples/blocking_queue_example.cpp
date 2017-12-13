//
// Created by kchen on 12/5/17.
//

#include "concurrent/blocking_queue.h"

#include <iostream>
#include <thread>

using namespace concurrentcpp;
using namespace std;
using namespace std::chrono;

int main(int argc, const char** argv) {
    BlockingQueue<int> queue{100};
    int total = 10000;
    thread producer1([&]{
        for (int i = 0; i < total; i++) {
            bool res = queue.put(i);
            cout << "producer 1: " << i << endl;
            assert(res);
        }
    });

    thread producer2([&]{
        for (int i = 0; i < total; i++) {
            bool res = queue.put(i);
            cout << "producer 2: " << i << endl;
            assert(res);
        }
    });

    thread producer3([&]{
        for (int i = 0; i < total; i++) {
            bool res = queue.put(i);
            cout << "producer 3: " << i << endl;
            assert(res);
        }
    });

    atomic_int64_t count{0};
    thread consumer1([&]{
        while (true) {
            int i = 0;
            bool res = queue.get(i, milliseconds{100});
            cout << "consumer 1: " << i << endl;
            if (res) {
                count.fetch_add(1);
            } else {
                if (count == total * 3) {
                    break;
                }
            }
        }
    });

    thread consumer2([&]{
        while (true) {
            int i = 0;
            bool res = queue.get(i, milliseconds{100});
            cout << "consumer 2: " << i << endl;
            if (res) {
                count.fetch_add(1);
            } else {
                if (count == total * 3) {
                    break;
                }
            }
        }
    });

    producer1.join();
    producer2.join();
    producer3.join();
    consumer1.join();
    consumer2.join();

    assert(queue.empty());
}