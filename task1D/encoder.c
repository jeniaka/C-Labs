#include<stdio.h>
#include<string.h>
#include <stdlib.h>
//FILE *in = stdin;
FILE *out = NULL;
FILE *in=NULL;
int ch;
int i=0;
int beforeCh;
int debug=0;
char *jumpword;
int changedC;
int encriptsign=0;
int cycle=0;
int writefileMode=0;
int readfileMode=0;
char* file;

int nextJump()
{
int j = cycle;
    changedC =jumpword[j]-48;
    cycle=(cycle+1)%(strlen(jumpword));
    return changedC;
}
int changC()
{
	if(encriptsign!=0)
	{
		ch= ch+nextJump()*encriptsign;
	}
	 if(ch<0)
	 	ch=ch+255;
    
    return ch;
}
void jumpline()
{
    if(encriptsign!=0 && beforeCh == 10){
        fprintf(out, "\n" );
       cycle=0; 
    }

}

void readinput()
{
        beforeCh=ch;
        if(debug==1)
        {fprintf(stderr,"0x%02x\t",ch);}
        if(encriptsign==0){
            if( ch >= 'A' && ch <= 'Z')
                ch+=32;
        }
        if(debug==1)
        {fprintf(stderr,"0x%02x\n",ch);}
        if(encriptsign!=0&& ch != '\n')
            ch=  changC();
        fputc(ch,out);
        jumpline();
    
}
void regularMode()
{
     while((ch = fgetc(stdin))!=EOF)
    {
        readinput();
    }
}

void outputFromFile()
{
    in = fopen(file,"w+");
    if(in ==NULL){
        printf("can't open the file" );
        exit(1);
    }
    while((ch = fgetc(stdin))!=EOF)
    {
        out=in;
        readinput();
    }
    
}
int main(int argc,  char **argv)
{

    for(i=1;i<argc;i++)
    {

        if (strcmp(argv[i], "-D") == 0){
            debug=1;
            printf("%s\n","-D" );
        }
        else if( strncmp(argv[i], "+e",2)==0)
        {
          
            jumpword =argv[i]+2;
            encriptsign=1;
        }
        else if( strncmp(argv[i], "-e",2)==0)
        {
           
            jumpword = argv[i]+2;
            encriptsign=-1;
        }
        else if( strncmp(argv[i],"-o",2)==0)
        {
            writefileMode=1;
            file = argv[i]+2;
        }
       
    }
    out = stdout;
    if(writefileMode==1)
        outputFromFile();
    else{
    regularMode();
}
    printf("^D\n");
    return 0;
}
