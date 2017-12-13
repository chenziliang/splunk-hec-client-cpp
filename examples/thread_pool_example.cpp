//
// Created by kchen on 12/10/17.
//

#include "concurrent/thread_pool.h"

#include <iostream>

using namespace concurrentcpp;
using namespace std;


int main(int argc, const char** argv) {
    // create thread pool with 4 worker threads
    ThreadPool pool(1);

    for (int i = 0; i < 10000; i++) {
        pool.submit([](int answer) { cout << answer<< endl; return answer; }, 42);
    }

    cout << "get future" << endl;
    auto result = pool.submit([](int answer) { cout << answer<< endl; return answer; }, 42);
    // get result from future
    cout << result.get() << endl;
}

