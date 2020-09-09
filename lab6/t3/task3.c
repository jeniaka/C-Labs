 #include <errno.h>
#include <signal.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
 #include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>


       
     int  main(int argc, char *argv[])
       {
          char* commands[3];// this line took from niv daniel
           commands[0]="ls";// this line took from niv daniel
        commands[1]="-l";// this line took from niv daniel
        commands[2] = NULL;// this line took from niv daniel
           int pipefd[2];
           pid_t cpid;
          // char hello[] = {'H', 'E', 'L','L', 'O', '\0'};
           char buf[20];
           char* commands2[4];
           commands2[0] = "tail";
           commands2[1] = "-n";
           commands2 [2] = "2";
           commands2[3] = NULL;
           int flagD = 0;
          // printf("sadf");

          
           if(argc>1)
           {
            if(strcmp(argv[1], "-D") == 0)
           {
               flagD = 1;
           }
           }
           if (flagD == 1)
           {
              fprintf(stderr,"parent_process>forking…\n");
           }
            if (pipe(pipefd) == -1) {
               perror("pipe");
               exit(EXIT_FAILURE);
           }
           
           cpid = fork();
           if (cpid == -1) {
               perror("fork");
               exit(EXIT_FAILURE);
           }
           if(flagD)
           {
              fprintf(stderr,"parent_process>created process with id: 0\n");   
           }

           if (cpid == 0)
            { 
                if(flagD)
                 {
                   fprintf(stderr,"child1>redirecting stdout to the write end of the pipe…\n");   
                 }
               close(STDOUT_FILENO);
              // int  write = open(pipefd[1],O_RDONLY);// open for reading.
               dup(pipefd[1]);
               close(pipefd[1]);
               if(flagD)
                 {
                   fprintf(stderr,"child1>going to execute cmd: …\n");   
                 }
               execvp(commands[0], commands);// i took this line from niv daniel            

           } 
           else {   
                 
               if(flagD)
           {
              fprintf(stderr,"parent_process>closing the write end of the pipe…\n");   
           }
               close(pipefd[1]);
               int id2 = fork();
               if (id2 == 0)
               {
                   if(flagD)
                 {
                   fprintf(stderr,"child2>redirecting stdout to the write end of the pipe…\n");   
                 }
                  close(STDIN_FILENO);
                  dup(pipefd[0]);
                  close(pipefd[0]);
                  if(flagD)
                 {
                   fprintf(stderr,"child2>going to execute cmd: …\n");   
                 }
                  execvp(commands2[0], commands2);

               }
               else
               {
                   if(flagD)
                    {
                     fprintf(stderr,"parent_process>closing the read end of the pipe…\n");   
                    }
                   close(pipefd[0]);
               }
               
                wait(NULL);                
                exit(EXIT_SUCCESS);
           }
       }
