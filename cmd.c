#include <stdio.h>
#include <string.h>

// this is not threadsafe, I use strtok

enum{CMDLEN=100};

// string containing function name
// optionally read an ascii number q
// followed by \n
// then come q bytes of raw data

int
main()
{
  char s[CMDLEN],*line;
  do{
    line=fgets(s,sizeof(s),stdin);
    char *search=" ",*tok=0;
    if(line){
      tok=strtok(line,search);
      while(tok!=0){
	printf("+%s+\n",tok);
	tok=strtok(0,search);
      }
      printf("\n");
    }    
  }while(line);
  return 0;
}
