#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>
#include "LineParser.h"
#include <sys/types.h>
#include <zconf.h>
#include <sys/stat.h>
#include <fcntl.h>



#define TERMINATED  -1
#define RUNNING 1
#define SUSPENDED 0
	
	int PID;
 int debug=0;
 char prompt[PATH_MAX];
 char input[2048];
 int status;
 
typedef struct process{
    cmdLine* cmd;                         /* the parsed command line*/
    pid_t pid; 		                  /* the process id that is running the command*/
    int status;                           /* status of the process: RUNNING/SUSPENDED/TERMINATED */
    struct process *next;	                  /* next process in chain */
} process;

typedef struct Pair{
    char* name;
    char* valule;
    struct Pair *next;
}Pair;

process* process_list;

void addProcess(process** process_list, cmdLine* cmd, pid_t pid){
	process *newProc;
	newProc = malloc(sizeof(process));
 	newProc->cmd = cmd;
 	newProc->pid = pid;
 	newProc->status = RUNNING;
 	newProc->next = *process_list;
 	*process_list = newProc;
	}


void updateProcessStatus(process* process_list, int pid, int status){
    if(process_list !=NULL) {
        process *lastProcess = process_list;
        if (lastProcess != NULL) 
		{
            while (lastProcess->next != NULL) {
                if(lastProcess->pid == pid){
                    lastProcess->status = status;
                    return;
                }
                lastProcess = lastProcess->next;
            }
            if(lastProcess->pid == pid){
                lastProcess->status = status;
                return;
            }      
	    }
    }
}

void updateProcessList(process **process_list){
	int status = 0, waitId = 0,newStatus = 0;
if(process_list !=NULL) {
while ( (waitId = waitpid(-1, &status, WNOHANG)) > 0) {
			if(waitId == 0)
					newStatus = RUNNING;
   			else if(WIFSTOPPED(status))
            		newStatus = SUSPENDED;
			else if(WIFEXITED(status))
        			newStatus = TERMINATED;
    		}
            updateProcessStatus(*process_list, waitId, newStatus);
        }
	}



void printProcessList(process** process_list){
	
	process *lastProc = *process_list;
	if(lastProc !=NULL)
	{
		
		updateProcessList(process_list);
		printf("PID\t | Command\t | STATUS\n");
		printf("------------------------------------\n");
		while (lastProc->next !=NULL)
		{
			
			printf("%d\t  %s\t\t    ", lastProc->pid,lastProc->cmd->arguments[0]);
			
			if(lastProc->status == RUNNING)
 			printf("%s\n", "Running");
 		else if(lastProc->status == TERMINATED)
 			printf("%s\n", "Terminated");
 		else
 			printf("%s\n", "Suspended");
 		lastProc = lastProc->next;

		}
	}
}

void freeProcessList(process* process_list){
	process *tempProc;
	while(process_list != NULL){
		tempProc = process_list;
		process_list = process_list->next;
		freeCmdLines(tempProc->cmd);
		free(tempProc); 
	}
}



int execute(cmdLine *pCmdLine){

	 int PID = fork();

 	 if (strcmp(pCmdLine->arguments[0], "quit") == 0)
	{
		freeCmdLines(pCmdLine);
		exit(0);
		
	}
	if(strncmp(pCmdLine->arguments[0],"cat",3) ==0 && PID ==0){
		 if(pCmdLine->inputRedirect !=NULL && pCmdLine->outputRedirect!=NULL){
            int  in = open(pCmdLine->inputRedirect,O_RDONLY);// open for reading.
            int  out = open(pCmdLine->outputRedirect,O_WRONLY | O_TRUNC | O_CREAT , S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR );
			dup2(in, 0);
            dup2(out, 1);
            close(in);
            close(out);
	}
    }
 	 if(PID <= 0 && (strcmp(pCmdLine->arguments[0], "cd") == 0)){
	 	 	int cdwork = chdir(pCmdLine->arguments[1]);
			  
	 	 	if(cdwork<0)
	 	 	{
	 	 		perror("error");
				
	 	 		_exit(1);
	 	 	}
	 	 
	 	}
	
 	 else if (PID == 0 && (execvp(pCmdLine->arguments[0], pCmdLine->arguments) == -1) )
 	 	{
			  
 	 	freeCmdLines(pCmdLine);
        perror("ERROR");    
 	 	_exit(1);
 	 }
 	 if(PID>0)
 	 {
 	 	addProcess(&process_list,pCmdLine,PID);
 	 	if(pCmdLine->blocking)
 	 	{
 	 	
 	 	int wait = waitpid(PID,&status,0);
 	 	if(wait == -1)
 	 	{
 	 		perror("error");
 	 		_exit(1);
 	 	}
 	 	if(debug == 1){	
 	 		fprintf(stderr, "PID: %d\n", PID);
 		fprintf(stderr, "Executing command: %s\n", "waitpid");
 	}
 }

 	 }


 	 if(debug == 1){
 	 	fprintf(stderr, "PID: %d\n", PID);
 		fprintf(stderr, "Executing command: %s\n", "fork");
 		fprintf(stderr, "PID: %d\n", PID);
 		fprintf(stderr, "Executing command: %s\n", "execvp");
 	 }
 	 return 1;
 }


 void changeDirectory(cmdLine *cmd)
 {
	int cdwork = chdir(cmd->arguments[1]);  
		if(cdwork<0)
	 	{
	 	 	perror("error");
	 	 	_exit(1);
		 }
}

 int main(int argc, char **argv){
     Pair *pairlist = NULL;
 	process_list = NULL;
	
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
	if (strcmp(pCmdLine->arguments[0],"cd")==0)
		changeDirectory(pCmdLine);
	else if (strcmp(pCmdLine->arguments[0], "procs") == 0)
			printProcessList(&process_list);
    else if(strncmp(pCmdLine->arguments[0],"set",3)==0)
    {

    }
 	else{
			execute(pCmdLine);
		}
}
	return 0;
}


