#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// this is not threadsafe, I use strtok

#define len(x) (sizeof(x)/sizeof(x[0]))

enum{CMDLEN=100};

// string containing function name
// optionally read an ascii number q
// followed by \n
// then come q bytes of raw data

int
fun(int a)
{
  return a+a;
}

int
fub(int a)
{
  return 3*a;
}


struct{
  char name[CMDLEN];
  int (*fptr)();
}cmd[]={{"fun",fun},
	{"fub",fub}};

int
lookup(char*s)
{
  unsigned int i;
  for(i=0;i<len(cmd);i++)
    if(0==strncmp(s,cmd[i].name,CMDLEN))
      return i;
  return -1;
}

int
main()
{
  char s[CMDLEN],*line;
  do{
    line=fgets(s,sizeof(s),stdin);
    char *search=" ",*tok;
    if(line){
      tok=strtok(line,search);
      while(tok!=0){
	if(isalpha(tok[0]))
	  printf("+%s=%d+\n",tok,lookup(tok));
	else
	  printf("+%s=%g+\n",tok,atof(tok));
	tok=strtok(0,search);
      }
      printf("\n");
    }    
  }while(line);
  return 0;
}
