/*task1b*/
#include "util.h"

#define SYS_CLOSE 6
#define SYS_WRITE 4
#define STDOUT 1
#define STDERR 2
#define SYS_OPEN 5
#define SYS_LSEEK 19
#define SYS_EXIT 1
#define O_RDONLY 0
#define O_WRNLY 1
#define O_RDRW 2
#define O_TRUNC 512
#define O_CREAT 64
#define MODE 0777
#define SEEK_END 2
#define SEEK_SET 0
#define SEEK_CUR 1
#define O_RDRW 2
#define SYS_READ 3
#define STDIN 0
#define EXIT 1
#define lowerMode 0

void encode(char*,char*,int,int,int);

int main (int argc , char* argv[], char* envp[])
{
    int i,j;
    int regularIN=0;
    int regularOUT=0;
    int writeout=0;
    int readin=0;
    int outFile;  
    int outsize=0;
    int startIN;
    int startOUT;
    int input =0;   
    int insize=0;
    int mode=0;
    int output=STDOUT;  
    
    char* in="stdin";
    char* out="stdout";
    
        for (i = 1; i < argc; i++) {
            if (strncmp(argv[i], "-D",2) == 0) {
                system_call(SYS_WRITE,output,"-D\n",3);
                mode=1;                            
            }else if(strncmp(argv[i],"-i",2)==0){
                readin=1;    
                insize= strlen(argv[i])-2;  
                startIN=i;
            }else if(strncmp(argv[i],"-o",2)==0){
                writeout=1;   
                outsize= strlen(argv[i])-2;
                startOUT=i;
                
            }

            else {
                system_call(EXIT,0x55,"\n",1); 
            }
        }
    
    
    if(readin||writeout){
    char strIN[insize];    
    char strOUT[outsize];   
        if(readin){
             regularIN=1;
               for(j=0;j<insize;j++){
                    strIN[j]=(argv[startIN])[j+2];
                }
                strIN[insize]='\0';
                
        }
        if(writeout){
                regularOUT=1;
                for(j=0;j<outsize;j++){
                    strOUT[j]=(argv[startOUT])[j+2];
                }
                strOUT[outsize]='\0';
                
            
        }
        if(regularIN){
                in=strIN;
                input=system_call(SYS_OPEN,strIN,O_RDONLY,MODE);
                
                if(input<0){    
                    system_call(EXIT,0x55,"\n",1); 
                }
        }
        if(regularOUT){
            out=strOUT;
            outFile=system_call(SYS_OPEN,strOUT,O_RDRW|O_CREAT,MODE);
            if(outFile<0){
                    system_call(EXIT,0x55,"\n",1);
                }
                output=outFile; 
        }
        encode(in,out,mode,output,input);   }
    else{
            encode(in,out,mode,output,input);   }
    if(writeout==1){
        int err=system_call(SYS_CLOSE,outFile);
        if(err<0){
            system_call(EXIT,0x55,"\n",1); 
        }
    }

    if(readin==1){
        
        int err=system_call(SYS_CLOSE,input);
        if(err<0){
            system_call(EXIT,0x55,"\n",1); 
        }
    }

    return 0;
}

void encode(char* in,char* out,int mode,int output,int input) {
    char intput;
    char buffer[256];
    int j = 1;
    buffer[0] = '\n';
       if(!mode){
        while(system_call(SYS_READ,input,&intput,1)==1){
            
            if(intput>='a'&&intput<='z'){
                intput-=32;
            }
            buffer[j] = intput;
            j++;
             if(buffer[j-1] == '\n'){
            system_call(SYS_WRITE, output, buffer, j);
            j = 1;
        }
    }
}
     else{
            system_call(SYS_WRITE,STDERR,"\ninput: ",8);
            system_call(SYS_WRITE,STDERR,in,strlen(in));
            system_call(SYS_WRITE,STDERR,"\noutput: ",9);
            system_call(SYS_WRITE,STDERR,out,strlen(out));
            system_call(SYS_WRITE,STDERR,"\n",1);
        char *c;   
           
        while((system_call(SYS_READ,input,&intput,1))==1){
            int i=0;
         i++;
        system_call(SYS_WRITE, STDERR, "\nSYS_CALL ID : ", 15);
         system_call(SYS_WRITE,STDERR,itoa(i),1);  
            c=itoa(SYS_READ); 
            system_call(SYS_WRITE, STDERR, "\nReturn Code: ", 15);
            system_call(SYS_WRITE,STDERR,c,1); 
           if(intput>='a'&&intput<='z'){
                intput-=32;
            }
            buffer[j] = intput;
            j++;
            if(buffer[j-1] == '\n'){
            system_call(SYS_WRITE, output, buffer, j);
            j = 1;
        }

            }
    }
}
