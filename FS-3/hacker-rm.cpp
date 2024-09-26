#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
int main(int argc, const char** argv)
{
        char buffer[100];
        if(argc < 2)
        {
                printf("Write the source file's name\n");
                return 1;
        }
        int fd = open(argv[1], O_RDWR);
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
        while(bytes_read){
                buffer[bytes_read] = '\0';
		lseek(fd, -bytes_read, SEEK_CUR);
                write(fd, "\0", bytes_read);
                bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        }
        close(fd);
	if(unlink(argv[1]) != 0)
	{
		printf("Cant delete the file\n");
		return 4; 
	}
	printf("File succesfully deleted\n");
        return 0;

}

