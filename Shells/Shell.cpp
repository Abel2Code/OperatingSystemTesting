// This final itteration of the Shell allows pipes

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include <iostream>
#include <bits/stdc++.h>
#include <sys/wait.h>
using namespace std;


void executePipe(char* cmd1, char* arg1[], char* cmd2, char* arg2[]){
  int pipefds[2];
  pid_t pid;
  if(pipe(pipefds)==-1){
    perror("Pipe Failed");
    exit(EXIT_FAILURE);
  }

  pid = fork();
  if(pid < 0) {
    perror("Fork Failed");
    exit(EXIT_FAILURE);
  }

  if(pid == 0){
    // Write to pipe instead of STDOUT
    dup2(pipefds[1], STDOUT_FILENO);

    // Close read pipe
    close(pipefds[0]);


    execvp(cmd1, arg1);
    exit(EXIT_SUCCESS);
  } else {
    // Replace stdin with the read end of the pipe
    dup2(pipefds[0], STDIN_FILENO);
    // Close write to the pipe, in parent
    close(pipefds[1]);

    execvp(cmd2, arg2);
    exit(EXIT_SUCCESS);
  }
}


int main(int argc, char* argv[])
{
  string input;

	for(;;){
		cout << getenv("USER") << "-$ ";
		getline(cin, input);

		if(input.compare("exit") == 0){
			break;
		}

		char cmd1[input.length() + 1];
		strcpy(cmd1, input.c_str());

    // Splitting between |
    char* firstCmd = strtok(cmd1, "|");
    char* secondCmd = strtok(NULL, "|");

    // Finding Arg1

		char *arg1[input.length()+ 1];

		// Argument Implementation
		char* arg = strtok(firstCmd, " ");
		int counter = 0;
		while(arg){
			arg1[counter] = arg;
			counter++;
			arg = strtok(NULL, " ");
		}
		arg1[counter] = NULL;

    // Run in seperate process

    pid_t pid;
    pid = fork();

    if(pid < 0){
      printf("Fork Failed");
    } else if(pid == 0) {
      if(secondCmd == NULL){
        executePipe(NULL, NULL, arg1[0], arg1);
      } else{
        // Finding Arg2

        char *arg2[input.length() + 1];

        // Argument Implementation
        arg = strtok(secondCmd, " ");
        counter = 0;
        while(arg){
          arg2[counter] = arg;
          counter++;
          arg = strtok(NULL, " ");
        }
        arg2[counter] = NULL;
        executePipe(arg1[0], arg1, arg2[0], arg2);
      }
    } else {
      wait(NULL);
    }
	}
}
