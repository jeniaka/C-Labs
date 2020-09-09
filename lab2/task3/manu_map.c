#include <stdlib.h>
#include <stdio.h>
#include <string.h>
 
char censor(char c) {
  if(c == '!')
    return '.';
  else
    return c;
}
 
char* map(char *array, int array_length, char (*f) (char)){
  char* mapped_array = (char*)(malloc(array_length*sizeof(char)));
  for(int i = 0;i<array_length;i++)
  	mapped_array[i] = f(array[i]);
  return mapped_array;
}
char encrypt(char c)
{
  if(c>=0x20 && c<=0x7E)
      c = c+3;
    return c;
} /* Gets a char c and returns its encrypted form by adding 3 to its value. 
          If c is not between 0x20 and 0x7E it is returned unchanged */
char decrypt(char c)
{
  if(c>=0x20 && c<=0x7E)
      c = c-3;
    return c;
} /* Gets a char c and returns its decrypted form by reducing 3 to its value. 
            If c is not between 0x20 and 0x7E it is returned unchanged */
char dprt(char c)
{
  printf("%d\n",c);
  return c;
}/* dprt prints the value of c in a decimal representation followed by a 
           new line, and returns c unchanged. */
char cprt(char c)
{
  if(c>=0x20 && c<=0x7E)
    printf("%c\n",c);
  else if(c!='\n')
    printf(".\n");
  return c;
} /* If c is a number between 0x20 and 0x7E, cprt prints the character of ASCII value c followed 
                    by a new line. Otherwise, cprt prints the dot ('.') character. After printing, cprt returns 
                    the value of c unchanged. */
char my_get(char c)
{
  FILE *fp;
    fp = stdin;
    char ch;
        ch =fgetc(fp);

        return ch;
} /* Ignores c, reads and returns a character from stdin using fgetc. */
 char quit(char c)
 {
  
  if(c=='q')
    exit(0);
  else return c;
 }
 struct fun_desc {
  char *name;
  char (*fun)(char);
};
int main(int argc, char **argv){
int base_len=5;
char *tmp;
char carray[5]="";

struct fun_desc menu[] = { { "Censor", censor },{ "Encrypt", encrypt },
  { "Decrypt", decrypt }, { "Print dec", dprt },{ "Print String", cprt },
   { "Get String", my_get }, { "Quit", quit },{ NULL, NULL } };
   int choose=0;
   while(1)
   {
    printf("Please choose a function:\n");
     for(int i=0;i<sizeof(menu)/(sizeof(struct fun_desc))-1;i++)
    {
      printf("%d) %s\n",i,menu[i].name);
   }

    
    printf("Option: "); 
     scanf("%d",&choose);
     if(0<=choose&& choose <= sizeof(menu)/(sizeof(struct fun_desc))-1)
    {
      printf("Within bounds\n");
      getchar();
      tmp = map(carray,base_len,menu[choose].fun);
      strcpy(carray,tmp);
      free(tmp);
    }
    else
    {
      printf("Not within bounds\n");
    }
    printf("DONE.\n");
}
}