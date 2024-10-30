#include <iostream>
#include <sys/wait.h>
#include <unistd.h>       
#include <cstring>    

void do_command(char** argv) {
    auto start = std::chrono::high_resolution_clock::now();  

    pid_t pid = fork();  

    if (pid == 0) {
        execvp(argv[0], argv);
	return;
    } 
    int status;
    wait(&status);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Provide the command" << std::endl;
	return 1;
    }

    char** args = new char*[argc];
    for (int i = 1; i < argc; i++) {
       args[i - 1] = argv[i];
    }
    args[argc - 1] = nullptr;

    do_command(args);

    delete[] args;
    return 0;
}
