#include <pthread.h>
#include <queue>
#include <functional>
#include <vector>
#include <atomic>
#include <iostream>

class parallel_scheduler {
public:
    explicit parallel_scheduler(size_t capacity) : capacity_(capacity), stop_(false) {
        pthread_mutex_init(&queue_mutex_, nullptr);
        pthread_cond_init(&queue_cond_, nullptr);

        for (size_t i = 0; i < capacity_; ++i) {
            pthread_t thread;
            pthread_create(&thread, nullptr, thread_worker, this);
            threads_.push_back(thread);
        }
    }

    ~parallel_scheduler() {
        stop_ = true;

        pthread_cond_broadcast(&queue_cond_);

        for (pthread_t& thread : threads_) {
            pthread_join(thread, nullptr);
        }

        pthread_mutex_destroy(&queue_mutex_);
        pthread_cond_destroy(&queue_cond_);
    }

    void run(std::function<void(void*)> func, void* arg) {
        pthread_mutex_lock(&queue_mutex_);
        task_queue_.push({func, arg});
        pthread_mutex_unlock(&queue_mutex_);
        pthread_cond_signal(&queue_cond_);
    }

private:
    struct Task {
        std::function<void(void*)> func;
        void* arg;
    };

    static void* thread_worker(void* arg) {
        auto* pool = static_cast<parallel_scheduler*>(arg);

        while (true) {
            Task task;

            pthread_mutex_lock(&pool->queue_mutex_);

            while (!pool->stop_ && pool->task_queue_.empty()) {
                pthread_cond_wait(&pool->queue_cond_, &pool->queue_mutex_);
            }

            if (pool->stop_ && pool->task_queue_.empty()) {
                pthread_mutex_unlock(&pool->queue_mutex_);
                break;
            }

            task = pool->task_queue_.front();
            pool->task_queue_.pop();

            pthread_mutex_unlock(&pool->queue_mutex_);

            task.func(task.arg);
        }

        return nullptr;
    }

    size_t capacity_;
    std::vector<pthread_t> threads_;
    std::queue<Task> task_queue_;
    pthread_mutex_t queue_mutex_;
    pthread_cond_t queue_cond_;
    std::atomic<bool> stop_;
};

