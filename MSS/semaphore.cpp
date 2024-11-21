#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <chrono>

class Integer {
private:
  int inner_value = 0;
  sem_t sem;
public:
  Integer(int value) : inner_value(value) {
    sem_init(&sem, 0, 1);
  }
  
  ~Integer() {
    sem_destroy(&sem);
  }

  void inc() {
    sem_wait(&sem);
    inner_value++;
    sem_post(&sem);
  }

  int get_inner_value() {
    return inner_value;
  }
};

void* increment(void* arg) {
  Integer* num = (Integer*)arg;
  for (int i = 0; i < 10000; ++i) {
    num->inc();
  }
  return nullptr;
}

int main() {
  Integer* num = new Integer(0);
  const int thread_count = 50;
  pthread_t tids[thread_count];

  auto start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < thread_count; ++i) {
    pthread_create(&tids[i], NULL, increment, num);
  }
  for (int i = 0; i < thread_count; ++i) {
    pthread_join(tids[i], NULL);
  }

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;

  std::cout << "Semaphore: Final value is " << num->get_inner_value() << ", Time: " << elapsed.count() << " seconds\n";

  delete num;
  return 0;
}

