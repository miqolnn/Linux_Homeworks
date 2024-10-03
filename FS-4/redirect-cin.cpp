#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

void initialize(int argc, char** argv)
{
  int f = open(argv[1], O_RDONLY);
  if(f<0)
  {
	  std::cerr<<"Couldnt open the file"<<std::endl;
	  exit(errno);
  }

  // put your implementation here 
  // ............................
  

  dup2(f,0);
  close(f);

}
 
int main(int argc, char** argv)
{
  if(argc!=2)
  {
	  std::cerr<<"Not correct input info"<<std::endl;
	  return 1;
  }
  // initialize the program based on the requirements
  initialize(argc, argv);
 
  // read the string 
  std::string input;
  std::cin >> input;
  
  // logic to get the reversed string
  std::string reversed;
  
  // put your reversing logic here
  // .............................
 
  for(int i=input.length()-1;i>=0;i--)
	  reversed+=input[i];
  // print to the console
  std::cout << reversed<<std::endl;
 
  return 0;
}
