#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
	std::cout<< "Give normal info"<<std::endl;
        return 1;
    }

    int f1 = open(argv[1], O_WRONLY | O_CREAT, 0644);
    if (f1 < 0) {
	std::cerr<<"Cant open the file"<<std::endl;
        exit(errno);
    }

    int f2 = dup(f1);
    write(f1, "first line\n", 11);
    write(f2, "second line\n", 12);

    close(f1);
    close(f2);

    return 0;
}

