#include <iostream>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <string>
#include <fcntl.h>
#include <sys/wait.h>
using namespace std;

int yev = 0, kam = 0;

int run(std::string s, std::string fl = "", bool append = false){
	std::vector<char*> args;
	int l = s.length();
	bool silent = false;
	if(s.substr(0,6)=="silent"){
		silent = true;
		s = s.substr(7,s.length()-6);
	}

	std::vector<std::string> tmp;
	args.clear();
	tmp.clear();
	tmp.push_back("");
	int v = 0;
	for(int i=0; i<s.length();i++)
	{
		if(s[i]!=' '){
			tmp[v]+=s[i];
		}
		else{
			args.push_back("");
			tmp.push_back("");
			v++;
		}
	}
	if(s[s.length()-1]!=' ')
	args.push_back("");
	for(int i=0;i<tmp.size();i++)
	{
		args[i]=&tmp[i][0];
	}
	args.push_back(nullptr);
	pid_t fr = fork();
	if(fr==0){
		if(silent || fl !=""){
			std::string lFile = std::to_string(getpid()) + ".log";
			if(fl!="")
				lFile = fl;
			int fs;
			cout<<lFile<<endl;
			if(!append)
				fs = open(lFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
			else
				fs = open(lFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC | O_APPEND, 0644);	
			dup2(fs, STDOUT_FILENO);
			close(fs);
		}	
		execvp(args[0],args.data());	
		return EXIT_FAILURE;
	}
	int sd;
	wait(&sd);
	return WIFEXITED(sd) ? WEXITSTATUS(sd) : -1;
}
int main(int argc, char* argv[]){
	while(true){
		std::string s;
		std::getline(std::cin, s);
		if(s.empty() || s=="exit")
			break;
		std::string tmp = "";
		for(int i=0;i<s.length();i++)
		{
			if(i<s.length()-1){
				if(s[i]=='&' && s[i+1]==s[i])
				{
					if(run(tmp)==-1)
						break;
					tmp = "";
				}
				else
				if(s[i]=='|' && s[i+1]=='|')
				{
					if(run(tmp)==0)
						break;
					tmp = "";
				}
				else
				if(s[i]=='>')
				{
					bool gri = false;
					if(s[i+1]==s[i])
					{
						gri=true;i++;
					}
					i++;
					if(s[i]==' ')
						i++;
					std::string fl = "";
					while(i<s.length() && s[i]!=' ')
						fl+=s[i++];
					if(run(tmp, fl, gri)==-1)
						return 0;
					tmp = "";
				}
				else
					if(s[i]==';'){
						run(tmp);
						tmp = "";
					}
				else
					tmp+=s[i];
			}
			else{
				tmp+=s[i];
			}
		}
		run(tmp);

	}
	return 0;
}
