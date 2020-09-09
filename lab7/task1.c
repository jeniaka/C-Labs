
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
FILE *fp=NULL;
char file_name[100];
char inlocation[100];
int location;
int length;
char inlengt[100];
char input[100];
short debug_mode = 0;
char unitsize [32];
short display_mode =0;
int unitsizeint;

struct fun_desc {
    char *name;
    char (*fun)();
};

typedef struct {
    char debug_mode;
    char file_name[128];
    int unit_size;
    unsigned char mem_buf[10000];
    size_t mem_count;
}state;


void Toggle_mode(state s) {
    if (!debug_mode) {
        printf("\033[1;35m");
        printf("Debug flag now on\n\n");
        debug_mode = 1;
    } else {
        printf("Debug flag now off\n\n"); debug_mode = 0;
    }
    printf("\033[0m");
}
void toggle_display()
{
 if (!display_mode) {
        printf("\033[1;35m");
        printf("Display flag now on, hexadecimal representation\n\n");
        display_mode = 1;
    } else {
        printf("\033[1;35m");
        printf("Display flag now off, decimal representation\n\n"); display_mode = 0;
    }
    printf("\033[0m");
}
void removejmp(state *s)
{
    int i=0;
    while(s->file_name[i] != 0){
        if(s->file_name[i] == '\n'){
            s->file_name[i] = '\0';
            break;
        }
        i++;
    }
}

void set_file_name(state *s)
{
    printf("please select file to open: \n");
    fgets(s->file_name,100,stdin);
    removejmp(s);
    if(debug_mode) {
        printf("\033[1;35m");
        printf("Debug: file name set to %s\n", s->file_name);
        printf("\033[0m");
    }

}

void set_unit_size(state *s)
{
    printf("please choose unit size: 1 , 2 or 4\n");
    fgets(unitsize,32,stdin);
    sscanf(unitsize,"%d",&unitsizeint);
    if(unitsizeint==1||unitsizeint==2||unitsizeint==4) {
        s->unit_size = unitsizeint;
        if (debug_mode) {
            printf("\033[1;35m");
            printf("Debug: set size to %d\n", unitsizeint);
            printf("\033[0m");
        }
    }
    else
    {
        printf("wrong unit size\n\n");
    }
}
void load_into_memory(state *s)
{
    if(s->file_name==NULL)
    {
        fprintf(stderr , "there is no file to load\n");
    }
    else {
        fp = fopen(s->file_name, "r");
        if (fp == NULL) {
            printf("an error accord.\n");
            return;
        }
        printf("Please enter <location> <length>\n");

        fgets(input, 100, stdin);
        sscanf(input, "%x %d",&location,&length);
        if (debug_mode) {
            printf("\033[1;35m");
            printf("file name: %s location: %x length: %d\n", file_name, location, length);
            printf("\033[0m");
        }
        // printf("loc: %x len: %d\n", location, length);
        fseek(fp,location,SEEK_SET);
        if(fread(s->mem_buf,1,length,fp)==0)
        {
            printf("\033[1;31m");
            printf("failed to load\n") ;
            printf("\033[0m");
            return;
        }
        printf("Loaded %d units into memory\n",length);
        fclose(fp);

    }

}
void memory_display(state *s)
{
    char bf[32];
    int u;
    int addr;
    printf("Enter address and length\n");
    fgets(bf,32,stdin);
    sscanf(bf,"%x %d",&addr ,&u);
    if(addr==0) {
        if(!display_mode)
        {   printf("\033[1;31m");
            printf("Decimal\n=======\n");
            printf("\033[0m");
            for (int i = 0; i < u; ++i) {
            switch (s->unit_size) {
                case 1:
                    printf("%u\n", *(s->mem_buf + i));
                    break;
                case 2:
                    printf("%hu\n", *((short *) s->mem_buf + i));
                    break;
                case 4:
                    printf("%hhu\n", *((int *) s->mem_buf + i));
                    break;
                default:
                    printf("\n");
                    break;

            }
        }
        }
        else
        {
            printf("\033[1;31m");
         printf("Hexadecimal\n===========\n");
         printf("\033[0m");
        for (int i = 0; i < u; ++i) {
            switch (s->unit_size) {
                case 1:
                    printf("%x\n", *(s->mem_buf + i));
                    break;
                case 2:
                    printf("%hx\n", *((short *) s->mem_buf + i));
                    break;
                case 4:
                    printf("%hhx\n", *((int *) s->mem_buf + i));
                    break;
                default:
                    printf("\n");
                    break;

            }
        }
        }
    }
    else
    {
        if(!display_mode)
        {   printf("\033[1;31m");
            printf("Decimal\n ========\n");
            printf("\033[0m");
            for (int i = 0; i < u; ++i) {
            switch (s->unit_size) {
                case 1:
                    printf("%u\n", addr);
                    break;
                case 2:
                    printf("%hu\n", addr);
                    break;
                case 4:
                    printf("%hhu\n",addr);
                    break;
                default:
                    printf("\n");
                    break;

            }
        }
        }
        else
        {
            printf("\033[1;31m");
         printf("Hexadecimal\n ========\n");
         printf("\033[0m");
        for (int i = 0; i < u; ++i) {
            switch (s->unit_size) {
                case 1:
                    printf("%x\n",addr);
                    break;
                case 2:
                    printf("%hx\n", addr);
                    break;
                case 4:
                    printf("%hhx\n",addr);
                    break;
                default:
                    printf("\n");
                    break;

            }
        }
        }
    }
}
void   save_into_file(state *s)
{
    FILE* file = fopen(s->file_name, "r+");
    if(file == NULL){
        printf("\033[1;31m");
        fprintf(stderr, "An error occured while attempting to open %s!\n", s->file_name);
        printf("\033[0m");
        return;
    }
    int source_address_int;
    unsigned char* source_address;
    int target_location;
    int length;
    char buf[9+9+9];
    printf("Plaese enter <source-address> <target-location> <length>\n");

    fgets(buf, 9+9+9 , stdin);
    sscanf(buf, "%x %x %d",&source_address_int, &target_location, &length);
    source_address = (unsigned char*) source_address_int;
    if(source_address == 0){
        source_address = s->mem_buf;
    }
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    if(debug_mode)
    {
        printf("\033[1;35m");
        printf("Debug: file name: %s , file size: %d \n Location: %x , Length: %d \n source: %p\n", s->file_name, (int)s->unit_size, target_location, length,
               (void *) source_address);
        printf("\033[0m");

    }
    if(fileSize < target_location){
        fprintf(stderr,"targert location greater than %s",s->file_name);
        return;
    }
    fseek(file, target_location, SEEK_SET);
    fwrite(source_address, 1, length, file);
    fclose(file);

}
void memory_modify(state *s)
{
    int location;
    int val;
    char buf[9+9];
    FILE* file = fopen(s->file_name, "r+");
    if(file == NULL){

        fprintf(stderr, "An error occured while attempting to open %s!\n", s->file_name);
        return;
    }
    printf("Plaese enter <location> <val>\n");
    fgets(buf, 9+9, stdin);
    sscanf(buf, "%d %x", &location, &val);
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    if(fileSize < location){
        fprintf(stderr,"targert location greater than %s",s->file_name);
        return;
    }
    if (debug_mode)
    {
        printf("\033[1;35m");
        printf("Debug: Location: %d.  val: %x\n",location,val);
        printf("\033[0m");
    }
    fseek(file, location, SEEK_SET);
    fwrite(&val, s->unit_size, 1, file);
    fclose(file);
}

char quit(state *s)
{
    free(s);
    exit(0);
}

int main(int argc, char **argv) {
    state *st = (state *) calloc(1, sizeof(state));
    st->unit_size = 1;
    struct fun_desc menu[] = {  {"Toggle Debug Mode", (char (*)()) Toggle_mode},
                                {"Set File Name", (char (*)()) set_file_name},
                                {"Set Unit Size",     (char (*)()) set_unit_size},
                                { "Load Into Memory", (char (*)()) load_into_memory } ,
                                {"Toggle Display Mode",(char (*)()) toggle_display},
                                {"Memory Display",(char (*)()) memory_display},
                                {"Save Into File",(char (*)()) save_into_file},
                                {"Memory Modify",(char (*)())memory_modify},
                                {"Quit", quit }, {NULL, NULL } };
    while(1)
    {
        printf("\033[1;31m");
        printf("Choose action:\n");
        printf("\033[0m");
        for(int i=0;i<sizeof(menu)/(sizeof(struct fun_desc))-1;i++)
        {   
            if(i%2==0)
                 printf("\033[1;33m");
            else
                 printf("\033[1;36m");
            printf("%d-%s\n",i,menu[i].name);
             printf("\033[0m");
        }
        fflush(stdout);
        char  num[64];
        fgets(num, 64, stdin);

        if (num[0] >= '0' && num[0]<= '9') {
            menu[num[0]-48].fun(st);
        }
    }

}