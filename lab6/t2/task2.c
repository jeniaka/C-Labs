#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
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
 
 typedef struct Pair{
    char* name;
    char* valule;
    struct Pair *next;
}Pair;
 
typedef struct process{
    cmdLine* cmd;                         /* the parsed command line*/
    pid_t pid; 		                  /* the process id that is running the command*/
    int status;                           /* status of the process: RUNNING/SUSPENDED/TERMINATED */
    struct process *next;	                  /* next process in chain */
} process;



process* process_list;

void addProcess(process** process_list, cmdLine* cmd, pid_t pid){
	process *newProc;
	newProc = malloc(sizeof(process));
 	newProc->cmd = cmd;
 	newProc->pid = pid;
 	newProc->status = RUNNING;
 	newProc->next = *process_list;
 	*process_list = newProc;
 	freeCmdLines(cmd);
 	free(newProc);

	}

Pair* addPair(cmdLine * cmd, Pair *next)
{
    Pair *newPair = (Pair*)malloc(sizeof(Pair));

	newPair->name = strdup(cmd->arguments[1]);
	newPair->valule = strdup(cmd->arguments[2]);
	newPair->next = next;
	freeCmdLines(cmd);
	return newPair;
}

void setPair(cmdLine * cmd, Pair **pairlist)
{
	Pair *newpair;
	if(pairlist !=NULL){
		Pair *thispair = *pairlist;
		while(thispair !=NULL){
			if(strcmp(thispair->name, cmd->arguments[1])==0)
			{
				free(thispair->valule);
				thispair->valule = strdup(cmd->arguments[2]);
				freeCmdLines(cmd);
				return;
			}
			thispair = thispair->next;
		}
	}
	Pair *firstpair = *pairlist;
	newpair = addPair(cmd,firstpair);

	*pairlist = newpair;

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
	
	if(process_list != NULL){
		freeCmdLines(process_list->cmd);
		freeProcessList(process_list->next);
		free(process_list);
	}	


	

}

void freepairlist(Pair * pairlist)
{
	// Pair * pair;
	// while(pairlist != NULL)
	// {
	// 	pair = pairlist;
	// 	pairlist = pairlist->next;
	// 	free(pair->name);
	// 	free(pair->valule);
	// }

	if(pairlist !=  NULL)
	{
		free(pairlist->name);
		free(pairlist->valule);
		freepairlist(pairlist->next);
		free(pairlist);
	}
}



int execute(cmdLine *pCmdLine, process * process_list , Pair * pairlist){

	 int PID = fork();

 	 if (strcmp(pCmdLine->arguments[0], "quit") == 0)
	{
		freepairlist(pairlist);
		freeProcessList(process_list);

		freeCmdLines(pCmdLine);
		exit(0);
		
	}
	if( PID ==0){
		 if(pCmdLine->inputRedirect !=NULL ){
		 	int  in = open(pCmdLine->inputRedirect,O_RDONLY);// open for reading.
		 	dup2(in, 0);
		 	close(in);
		 }
		 if(pCmdLine->outputRedirect!=NULL)	{
            
            int  out = open(pCmdLine->outputRedirect,O_WRONLY | O_TRUNC | O_CREAT , S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR );
			
            dup2(out, 1);
            
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
	if(strcmp(cmd->arguments[1],"~")==0)
		replaceCmdArg(cmd,1,getenv("HOME"));
	int cdwork = chdir(cmd->arguments[1]);  
		if(cdwork<0)
	 	{
	 	 	perror("error");
	 	 	_exit(1);
		 }
	freeCmdLines(cmd);
}
int checkdolllar(cmdLine * cmd,Pair *pairlist)
{
	
	char *temp;
	int i=0;
	for (i = 0; i < cmd->argCount; i++)
	{
		if(strncmp(cmd->arguments[i],"$",1)==0){
			temp = &cmd->arguments[i][1];
			while(pairlist!=NULL)
			{
				if(strcmp(temp,pairlist->name)==0){
					replaceCmdArg(cmd, i, pairlist->valule);
					return 1;
				}
				pairlist = pairlist->next;
			}
			fprintf(stderr,"ERROR varuables are not found\n");

		
		}
	}
	return 0;
}
void printvars(Pair *pairlist)
{
	Pair* tmp = pairlist;	
	while(tmp != NULL){
		printf("\nname: %s\nvalue: %s\n",tmp->name,tmp->valule);
		tmp= tmp->next;
	}
	
	
}

 int main(int argc, char **argv){
    Pair * pairlist = NULL;
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
	checkdolllar(pCmdLine,pairlist);

	if (strcmp(pCmdLine->arguments[0],"cd")==0)
	{
		changeDirectory(pCmdLine);
		continue;
	}
	else if (strcmp(pCmdLine->arguments[0], "procs") == 0)
			printProcessList(&process_list);
    else if(strncmp(pCmdLine->arguments[0],"set",3)==0)
    {
		setPair(pCmdLine, &pairlist);
		continue;
    }
	else if(strncmp(pCmdLine->arguments[0],"vars",4)==0)
    {
		printvars(pairlist);
		freeCmdLines(pCmdLine);
    }
 	else{
			execute(pCmdLine, process_list,pairlist);

		}
}
	return 0;
}


