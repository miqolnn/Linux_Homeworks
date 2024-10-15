#include <iostream>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

char buffer[BUFFER_SIZE];

int main(int argc, char *argv[]) {
    if (argc != 6) {
        std::cerr << "Invalid parameters" << std::endl;
        return -1;
    }

    const char *source = argv[1];
    const char *destination = argv[2];
    off_t src_offset = std::atol(argv[3]);
    off_t dest_offset = std::atol(argv[4]);
    size_t num_bytes = std::atol(argv[5]);

    int src_fd, dest_fd;
    ssize_t bytes_read, bytes_written;
    size_t bytes_to_copy = num_bytes;

    src_fd = open(source, O_RDONLY);
    if (src_fd == -1) {
        std::cerr << "Error opening source file" << std::endl;
        return -1;
    }

    dest_fd = open(destination, O_WRONLY | O_CREAT, 0644);
    if (dest_fd == -1) {
        std::cerr << "Error opening destination file" << std::endl;
        close(src_fd);
        return -1;
    }

    lseek(src_fd, src_offset, SEEK_SET);
    lseek(dest_fd, dest_offset, SEEK_SET);

    while (bytes_to_copy > 0 && (bytes_read = read(src_fd, buffer, sizeof(buffer))) > 0) {
        if ((size_t)bytes_read > bytes_to_copy) {
            bytes_read = bytes_to_copy;
	}

        bytes_written = write(dest_fd, buffer, bytes_read);
        bytes_to_copy -= bytes_written;
    }

    close(src_fd);
    close(dest_fd);

    std::cout << "Successfully copied " << num_bytes << " bytes from the source file to the destination file." << std::endl;
    return 0;
}
