// Run with ./basicShell
// Compile with g++ basicShell.cpp -o basicShell

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
		cin >> input;

		if(input.compare("exit") == 0){
			break;
		}

		char cmd[input.length() + 1];
		strcpy(cmd, input.c_str());
		char *argv[3];
		argv[0] = cmd;
		argv[1] = NULL;
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
