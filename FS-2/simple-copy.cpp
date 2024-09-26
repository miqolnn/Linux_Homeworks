#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
int main(int argc, const char** argv)
{
        char buffer[100];
        if(argc < 3)
        {
                printf("Write the source and destination file's names\n");
                return 1;
        }
        int fd = open(argv[1], O_RDONLY);
        if(fd == -1)
        {
                printf("Cant open the source file\n");
                return 2;
        }

        ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);

        if(bytes_read == -1)
        {
                printf("Cant read the source file\n");
                return 3;
        }
	int fd2 = open(argv[2], O_TRUNC | O_WRONLY | O_CREAT, 0664);
	if(fd2 == -1)
	{
		printf("Cant open the destination file");
		return 4;
	}
        while(bytes_read){
                buffer[bytes_read] = '\0';
                write(fd2, buffer, bytes_read);
                bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        }
        close(fd);
        close(fd2);

	return 0;

}


