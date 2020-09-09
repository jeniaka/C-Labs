

#include <stdio.h>
#include <string.h>
#include <elf.h>
//stakoverflof
#include <sys/types.h>//
#include <sys/stat.h>//
#include <sys/mman.h> /* mmap() is defined in this header */ //stakoverflof
#include <fcntl.h>//
//

Elf32_Ehdr *header;
void *map_start;
int num_of_section_headers;
Elf32_Shdr *sectionDataRow;


int foo(char** argv)
{
  int fd;
   ; /* will point to the start of the memory mapped file */
   struct stat fd_stat; /* this is needed to  the size of the file */
    /* this will point to the header structure */
   

   if( (fd = open(argv[1], O_RDWR)) < 0 ) {
      perror("error in open");
      exit(-1);
   }

   if( fstat(fd, &fd_stat) != 0 ) {
      perror("stat failed");
      exit(-1);
   }

   if ( (map_start = mmap(0, fd_stat.st_size, PROT_READ | PROT_WRITE , MAP_SHARED, fd, 0)) == MAP_FAILED ) {
      perror("mmap failed");
      exit(-4);
   }

   /* now, the file is mapped starting at map_start.
    * all we need to do is tell *header to point at the same address:
    */

   header = (Elf32_Ehdr *) map_start;////////////////////

  
   /* now we can do whatever we want with header!!!!
    * for example, the number of section header can be obtained like this:
    */
   num_of_section_headers = header->e_shnum;

   /* now, we unmap the file */
   //munmap(map_start, fd_stat.st_size);

}

void ddd()
{
     printf("%c, %c, %c\n", header->e_ident[0], header->e_ident[1], header-> e_ident[2]);
   printf("%x\n", header->e_entry);
}

void printAllsection()
{

   void* address = map_start + header->e_shoff ;
         Elf32_Shdr* sh = (Elf32_Shdr*)address;
         Elf32_Shdr* table = &sh[header->e_shstrndx];
         char* pointerToSectionsNamesTable = map_start + table->sh_offset;

    for(int i = 0; i< num_of_section_headers ; i++)
    {
        int index = i;
      //  int offset = header->e_shoff + i*sizeof(Elf32_Shdr);
       // int numOfBytes = sectionDataRow->sh_size;
       // Elf32_Shdr sh =   &sectionDataRow[header->e_shstrndx];
        //char *name = sectionDataRow->sh_name + header->e_shstrndx + map_start;

        printf("index: [%2d], offset: %.6x, addres: %.8x, size: %.6x, type: %.10x, name: %5s\n", index, sh[i].sh_offset,sh[i].sh_addralign, sh[i].sh_size,  sh[i].sh_type, sh[i].sh_name+pointerToSectionsNamesTable);
    }
}

int checksmbol(Elf32_Shdr* sh)
{
  if(sh->sh_type == SHT_DYNSYM || sh->sh_type == SHT_SYMTAB)
     return 1;
     else return 0;
}

void printAllSymbols()
{
  //void* address = map_start + header->e_shoff ;
  Elf32_Shdr* sh = (Elf32_Shdr*)(map_start + header->e_shoff);
  Elf32_Shdr* rowToNames = &sh[header->e_shstrndx];
  const char* const pointerToSectionsNamesTable = map_start + rowToNames->sh_offset;
  //Elf32_Shdr* stringTableRow = sh[header->]
  Elf32_Sym* sym = NULL;
  int sizeOfSym = 0;
  char* pointerToTablesNames = NULL;
    printf("first print\n");

  for(int i = 0; i< num_of_section_headers; i++)
  {
     //printf("for\n");
      if(checksmbol(&sh[i]) == 1)
      {
         sizeOfSym = sh[i].sh_size/sh[i].sh_entsize;
         printf("if\n");
         sym =  (map_start + sh[i].sh_offset); 
         pointerToTablesNames = map_start + sh[i+1].sh_offset;
         
      }
  }
 // printf("fhfjhfjk");
  for (int i = 0; i < sizeOfSym; i++)
  {
    printf("index: [%2d], value: %-.8x, section index: [%2d], section name: %-20s, symbol name: %-20s\n" ,i , sym[i].st_value, sym[i].st_shndx, sh[sym[i].st_shndx].sh_name + pointerToSectionsNamesTable, pointerToTablesNames + sym[i].st_name);
  }
}
void printRelocation()
{
   
   
     Elf32_Shdr* sh = (Elf32_Shdr*)(map_start + header->e_shoff);
     Elf32_Shdr* table = &sh[header->e_shstrndx];
         char* pointerToSectionsNamesTable = map_start + table->sh_offset;
     Elf32_Shdr* sh2 = NULL;
     Elf32_Rel* rl = NULL;
     int relSize = 0;
     printf("offset\t\tinfo\n");
     {
     
     for(int i = 0; i < num_of_section_headers; i++)
     {

        if(sh[i].sh_type == SHT_REL)
        {
           relSize = sh[i].sh_size/ sh[i].sh_entsize;
           rl = map_start + sh[i].sh_offset;

           for(int j = 0; j< relSize; j++)
           {
              printf("%-.8x\t%-.8x\n", rl[j].r_offset, rl[j].r_info);
           }
        }
     }
     }
}







int main(int argc, char** argv)
{
foo(argv);
char inputUser [20];
 printf("0-Toggle Debug Mode\n1-Examine ELF File\n2-Print Section Names\n3-Print Symbols\n4-relocation table row format\n5-Quit\n");
 fgets(inputUser, 20, stdin);
 
 while(inputUser[0] != '5')
 {
     if(inputUser[0] == '1')
     {
        ddd();
     }
     if (inputUser[0] == '2')
     {
        printAllsection();
     }
     if(inputUser[0] == '3' )
     {
        printAllSymbols();
     }
     if(inputUser[0] == '4')
     {
        printRelocation();
     }
    
     
 printf("0-Toggle Debug Mode\n1-Examine ELF File\n2-Print Section Names\n3-Print Symbols\n4-relocation table row format\n5-Quit\n");
 fgets(inputUser, 20, stdin);
 }

  

}