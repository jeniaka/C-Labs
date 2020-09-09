#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>
#include "LineParser.h"
#include <errno.h>
	
	int PID;
 int debug=0;
 char prompt[PATH_MAX];
 char input[2048];
 int status;
 

 int execute(cmdLine *pCmdLine){
	 
 	 if (strcmp(pCmdLine->arguments[0], "quit") == 0)
	{
		freeCmdLines(pCmdLine);
		exit(0);
	}

 	 int PID = fork();
 	 if(PID <= 0 && (strcmp(pCmdLine->arguments[0], "cd") == 0)){
	 	 	int cdwork = chdir(pCmdLine->arguments[1]);
	 	 	if(cdwork<0)
	 	 	{
	 	 		perror("error");
				fprintf(stderr, "%d\n", errno);
	 	 		_exit(1);
	 	 	}
	 	 
	 	}

 	 else if (PID == 0 && (execvp(pCmdLine->arguments[0], pCmdLine->arguments) == -1))
 	 	{
 	 	freeCmdLines(pCmdLine);
 	 	perror("Error");
 	 	_exit(1);
 	 }
 	 if(PID>0 && (pCmdLine->blocking))
 	 {
 	 	int wait = waitpid(PID,&status,0);
 	 	if(wait == -1)
 	 	{
 	 		perror("error: ");
 	 		_exit(1);
 	 	}
 	 	if(debug == 1){	
 	 		fprintf(stderr, "PID: %d\n", PID);
 		fprintf(stderr, "Executing command: %s\n", "waitpid");
 	}

 	 }


 	 if(debug == 1){
 	 	fprintf(stderr, "PID: %d\n", PID);
 		fprintf(stderr, "Executing command: %s\n", "fork");
 		
 	 }
 	 return 1;
 }



 int main(int argc, char **argv){
	
	for (int i = 0; i < argc; ++i)
	{
		if(strncmp(argv[i], "-d", 2) == 0){
			debug = 1;
		}
	}

	while(1)
	{
	getcwd(prompt,PATH_MAX);
	printf("%s\n",prompt);
	fgets(input,2048,stdin);
	cmdLine *pCmdLine;
	pCmdLine = parseCmdLines(input);
	execute(pCmdLine);
}

	return 0;
}
