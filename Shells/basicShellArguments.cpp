// Run with ./basicShellArguments
// Compile with g++ basicShellArguments.cpp -o basicShellArguments



#include <iostream>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <bits/stdc++.h>
#include <sys/wait.h>
using namespace std;

int main()
{
	string input;

	for(;;){
		cout << getenv("USER") << "-$ ";
		getline(cin, input);

		if(input.compare("exit") == 0){
			break;
		}

		char cmd[input.length() + 1];
		strcpy(cmd, input.c_str());

		char *argv[input.length()];

		// Argument Implementation
		char* arg = strtok(cmd, " ");
		int counter = 0;
		while(arg){
			argv[counter] = arg;
			counter++;
			arg = strtok(NULL, " ");
		}
		argv[counter] = NULL;

		// End Argument Implementation

		pid_t pid;
		pid = fork();

		if(pid < 0){
			printf("Fork Failed");
		} else if(pid == 0) {
			execvp(cmd, argv);
		} else {
			wait(NULL);
		}
	}
}
