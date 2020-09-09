#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct virus {
    unsigned short SigSize;
    char virusName[16];
    char sig[];
} virus;
typedef struct link link;
link *list_virus; 
struct link {
    link *nextVirus;
    virus *vir;
};
struct fun_desc {
    char *name;
    void (*fun)();
};
unsigned short vsize[1];
int ofset=0;
char** argvplus1;
char buffer[10000];

char * jumpLine(char * buffer){
    char * head=buffer;
    while (*buffer!='\n'){
        buffer++;
    }
    *buffer='\0';
    return head;
}

void list_print(link *virus_list,FILE* output)
{
    link *list = virus_list;
    while(list!=NULL)
    {
        printf("Virus name: %s\n",list->vir->virusName);
        printf("Virus size: %d\n",list->vir->SigSize);
        printf("signature:\n");
        for(int i=0;i<list->vir->SigSize;i++){
            printf("%02hhX ",list->vir->sig[i]);
        }
        printf("\n\n");
        list= list->nextVirus;
    }
}


void detect_virus(char *buffer, unsigned int size, link *virus_list)
{
   link *currentList= list_virus;
    while(currentList!=NULL){
        virus * curVirus=currentList->vir;
        short sizeOfSig=curVirus->SigSize;
        if (sizeOfSig>size){
            currentList=currentList->nextVirus;
            continue;
        }
        char* array=curVirus->sig;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j <sizeOfSig; ++j) {
                char * arr=&array[j];
                char * buf=&buffer[i+j];
                if(memcmp(arr,buf,1)==0){
                    if(j==sizeOfSig-1){
                        printf("starting byte location in the suspected file : %d\n",i);
                        printf("Virus name:                                    %s\n",curVirus->virusName);
                        printf("size of the virus signature :                  %d\n",sizeOfSig);
                        printf("\n");
                        i=size;
                        break;
                    }
                }
                else{
                    break;
                }
            }
        }
        currentList=currentList->nextVirus;
    }
}

link* list_append(link* virus_list, virus* data)
{
 link * newLink = (link *) malloc(sizeof(link)+ sizeof(*data));
    newLink->vir =data;
    newLink->nextVirus=NULL;
    if(virus_list==NULL){
        return  newLink;
    }
    link * last = virus_list;
    while((last->nextVirus)!=NULL)
        last = last->nextVirus;
    last->nextVirus=newLink;
    return virus_list;
}

void list_free(link *virus_list)
{
     if(virus_list==NULL)
        return;
    if(virus_list->nextVirus==NULL){
        free(virus_list->vir);
        free(virus_list);
        return;
    }
    list_free(virus_list->nextVirus);
    free(virus_list->vir);
    free(virus_list);
}
void readVirus(FILE* fp)
{

    fseek(fp ,0, SEEK_END);
    long fsize = ftell(fp);
    fseek(fp, ofset, SEEK_SET);
    char *buffer = (char*)malloc((size_t) (fsize + 1));
    char *pointer = buffer;
    fread(buffer, fsize, 1, fp);
        if(fsize<0)
            exit(0);
        vsize[0]= (unsigned short) *buffer;
        buffer++;
        vsize[1]= (unsigned short) *buffer;
        buffer++;
        short number =( vsize[0] |vsize[1] << 8);
        fsize=fsize-18-number;
        ofset=ofset+18+number;
        virus *v = (virus *)(malloc (sizeof(virus) + number));
        v->SigSize= (unsigned short) number;
        for(int i=0;i<16;i++){
            v->virusName[i]=*buffer;
            buffer++;
        }
        for(int i=0;i<number;i++){
            v->sig[i]=*buffer;
            buffer++;
        }
        free(pointer);
        list_virus = list_append(list_virus,v);

}
void loadSignatures()
{
    FILE *fp=NULL;
    printf("input file:\n");
    fflush(stdout);
    char fileName[32];
    fgets(fileName,32,stdin);
   char *f=fileName;
    f=jumpLine(f);
    fp=fopen(f, "r");
    if(fp==NULL)
    {
        printf("The file %s does not exsit \n",f);
        return;
    }
    fseek(fp ,0, SEEK_END);
    long fsize = ftell(fp);
    while(ofset<fsize)
        readVirus(fp);
    fseek(fp, 0, SEEK_SET);
    fclose(fp);
}

void print_list()
{
    FILE* out =stdout;
    list_print(list_virus,out);
}
int minbuff(int fsize)
{
    if(fsize<10000)
        return fsize;
    return 10000;
}
void detect()
{
    FILE *fp=NULL;
    fp=fopen(*argvplus1, "r");
    if (fp==NULL){
        printf("Don't exist File on name %s \n",*argvplus1);
        return;
    }
    fseek(fp ,0, SEEK_END);
    int fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    fread(buffer, fsize, 1, fp);
        fclose(fp);
        unsigned int size=minbuff(fsize);
        detect_virus(buffer, size,list_virus);
}
void quit()
{
    list_free(list_virus);
    exit(0);
}




int main(int argc, char** argv){
    argvplus1 = (argv+1);
    struct fun_desc menu[] = {{"Load signatures",  loadSignatures},
                              {"Print signatures", print_list},
                              {"Detect viruses", detect},
                              {"Quit",             quit},
                              {NULL, NULL}};
    while(1)
    {
        for(int i=0;i<sizeof(menu)/(sizeof(struct fun_desc))-1;i++)
            printf("%d) %s\n",i+1,menu[i].name);
        fflush(stdout);
        char  num[32];
        fgets(num, 32, stdin);
        if (num[0] > '0' && num[0]<= '4')
            menu[num[0]-49].fun();  
    }
    return 0;
}


