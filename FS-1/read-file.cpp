#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
int main(int argc, const char** argv)
{
	char buffer[100];
	if(argc < 2)
	{
		printf("Write the file's name\n");
		return 1;
	}
	int fd = open(argv[1], O_RDONLY);
	if(fd == -1)
	{
		printf("Cant open the file\n");
		return 2;
	}

	ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);

	if(bytes_read == -1)
	{
		printf("Cant read the file\n");
		return 3;
	}

	while(bytes_read){
		buffer[bytes_read] = '\0';
		printf("%s", buffer);
		bytes_read = read(fd, buffer, sizeof(buffer) - 1);
	}
	close(fd);
	return 0;

}

