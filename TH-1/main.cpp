#include <pthread.h>
#include <iostream>
#include <sys/types.h>
#include <cstdlib>
#include <ctime>
#include <chrono>

using namespace std;

int *arr;
long long sum = 0;
int N, M;

long long sumArray() {
    long long local_sum = 0;
    for (int i = 0; i < N; ++i) {
        local_sum += arr[i];
    }
    return local_sum;
}

void* threadSum(void* arg) {
    pair<int, int>* data = (pair<int, int>*) arg;
    long long local_sum = 0;
    for (int i = data->first; i < data->second; ++i) {
        local_sum += arr[i];
    }
    __sync_add_and_fetch(&sum, local_sum);
    return nullptr;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <N> <M>" << endl;
        return 1;
    }

    N = stoi(argv[1]);
    M = stoi(argv[2]);

    arr = new int[N];

    for (int i = 0; i < N; ++i) {
        arr[i] = rand()%30;
    }

    auto start = chrono::high_resolution_clock::now();
    long long sumSeq = sumArray();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> durationSeq = end - start;
    
    sum = 0;
    pthread_t threads[M];
    pair<int, int> threadData[M];
    int chunkSize = N / M;

    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < M; ++i) {
        threadData[i].first = i * chunkSize;
        threadData[i].second = (i == M - 1) ? N : (i + 1) * chunkSize;
        pthread_create(&threads[i], nullptr, threadSum, &threadData[i]);
    }

    for (int i = 0; i < M; ++i) {
        pthread_join(threads[i], nullptr);
    }
    end = chrono::high_resolution_clock::now();
    chrono::duration<double> durationThreaded = end - start;

    cout << "Sum using regular for loop: " << sumSeq << endl;
    cout << "Time spent using regular for loop: " << durationSeq.count() << " seconds" << endl;

    cout << "Sum using threads: " << sum << endl;
    cout << "Time spent using threads: " << durationThreaded.count() << " seconds" << endl;

    delete[] arr;

    return 0;
}
