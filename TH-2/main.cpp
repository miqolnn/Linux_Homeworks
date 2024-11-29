#include "parallel_scheduler.cpp"
#include <iostream>
#include <unistd.h>

void example_task(void* arg) {
    int* num = static_cast<int*>(arg);
    std::cout << "Processing task: " << *num << " on thread " << pthread_self() << std::endl;
}

int main() {
    size_t thread_pool_size = 4;
    parallel_scheduler scheduler(thread_pool_size);

    const int num_tasks = 10;
    int task_ids[num_tasks];

    for (int i = 0; i < num_tasks; ++i) {
        task_ids[i] = i + 1;
        scheduler.run(example_task, &task_ids[i]);
    }

    sleep(5); 
    return 0;
}

