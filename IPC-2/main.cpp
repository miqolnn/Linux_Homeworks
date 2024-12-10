#include <iostream>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <string>
#include <vector>
#include <pthread.h>
using namespace std;


int calculateMthPrime(int m) {
    if (m <= 0) return -1;
    vector<int> primes;
    int num = 3;
    primes.push_back(2);
    while (primes.size() < m) {
        bool isPrime = true;
        for (int p : primes) {
            if (p * p > num) break;
            if (num % p == 0) {
                isPrime = false;
                break;
            }
        }
        if (isPrime) primes.push_back(num);
        num += 2;
    }
    return primes.back();
}

int main() {
    int pipefd1[2], pipefd2[2]; // Two pipes: parent->child and child->parent

    if (pipe(pipefd1) == -1 || pipe(pipefd2) == -1) {
        cerr << "Pipe creation failed." << endl;
        return 1;
    }

    pid_t pid = fork();

    if (pid == 0) {
        close(pipefd1[1]);
        close(pipefd2[0]);

        while (true) {
            int m;
            if (read(pipefd1[0], &m, sizeof(m)) <= 0) break;

            cout << "[Child] Calculating " << m << "-th prime number..." << endl;
            int result = calculateMthPrime(m);
            write(pipefd2[1], &result, sizeof(result));
            cout << "[Child] Sending calculation result of prime(" << m << ")..." << endl;
        }

        close(pipefd1[0]);
        close(pipefd2[1]);
    } else {
        close(pipefd1[0]);
        close(pipefd2[1]); 

        while (true) {
            string input;
            cout << "[Parent] Please enter the number: ";
            cin >> input;

            if (input == "exit") {
                cout << "[Parent] Exiting..." << endl;
                break;
            }

            int m;
            try {
                m = stoi(input);
            } catch (exception& e) {
                cout << "[Parent] Invalid input. Please enter an integer or 'exit'." << endl;
                continue;
            }

            cout << "[Parent] Sending " << m << " to the child process..." << endl;
            write(pipefd1[1], &m, sizeof(m));

            cout << "[Parent] Waiting for the response from the child process..." << endl;
            int result;
            if (read(pipefd2[0], &result, sizeof(result)) > 0) {
                cout << "[Parent] Received calculation result of prime(" << m << ") = " << result << "..." << endl;
            }
        }

        close(pipefd1[1]);
        close(pipefd2[0]);

        // Wait for the child process to terminate
        wait(&pid);
    }

    return 0;
}
