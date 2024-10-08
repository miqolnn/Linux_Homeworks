#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char* argv[]){
    if (argc != 3) {
        std::cout << "Give correct arguments" << std::endl;
        return 1;
    }

    int f = open(argv[1], O_RDONLY);
    if (f < 0) {
        std::cerr << "Cant open first file" << std::endl;
        exit(errno);
    }

    int ff = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (ff < 0) {
        std::cerr << "Cant open second file" << std::endl;
        exit(errno);
    }

    char buffer[100];
    int total_bytes=0,total_data=0,total_holes=0;
    int tmp_bytes;
    off_t offset=0, hoffset, doffset;    

    while(1){
	doffset = lseek(f,offset,SEEK_DATA);
	std::cout<<doffset<<std::endl;
	if(doffset == -1){
		if(errno == ENXIO)
			break;
		std::cerr<<"Cant lseek data"<<std::endl;
		close(f);
		close(ff);
		exit(errno);
	}
	if(doffset>offset)
		total_holes+=doffset-offset;

	hoffset = lseek(f,doffset, SEEK_HOLE);
	std::cout<<hoffset<<std::endl;
	if(hoffset==-1){
		std::cerr << "Cant lseek hole"<<std::endl;

		close(f);
		close(ff);
		exit(errno);
	}
	while(doffset < hoffset){
		off_t to_read = hoffset - doffset;
        	if (to_read > 100) 
                	to_read = 100;
            	tmp_bytes = pread(f, buffer, to_read, doffset);
                if(tmp_bytes==-1){
			std::cerr<<"Cant read data"<<std::endl;
        	        close(f);
              		close(ff);
                	exit(errno);
		}
		int w = write(ff,buffer,tmp_bytes);
		if(w==-1){
			std::cerr<<"Cant write data"<<std::endl;
                        close(f);
                        close(ff);
                        exit(errno);
		}
		doffset+=w;
		total_data+=tmp_bytes;
	
	}
	offset=hoffset;
    }
    std::cout<<"Total: "<<total_data+total_holes<<" Data: "<<total_data<<" Holes: "<<total_holes<<std::endl;
    return 0;
}
