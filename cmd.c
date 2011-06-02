// this is not threadsafe, I use strtok

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define len(x) (sizeof(x)/sizeof(x[0]))

enum{CMDLEN=100};

// string containing function name
// optionally read an ascii number q
// followed by \n
// then come q bytes of raw data
/*
./cmd << EOF
fun 3.1415 2.8
fub -2.34
3 fub a
fub fun b
fub .d3s0
EOF
*/
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

int isfloatchar(int c)
{
  if(c=='+'||c=='-'||c=='.'||isdigit(c))
    return 1;
  return 0;
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
      int i=0;
      while(tok!=0){
	if(i==0)
	  if(isalpha(tok[0]))
	    printf("+%s=%d+\n",tok,lookup(tok));
	  else
	    printf("error, expected function name\n");
	else
	  if(isfloatchar(tok[0])){
	    char*endptr;
	    double d=strtod(tok,&endptr);
	    if(endptr==tok)
	      printf("error, couldn't parse double\n");
	    else
	      printf("+%s=%g+\n",tok,d);
	  }else
	    printf("error, expected digit or .+- but found %c\n",tok[0]);
	tok=strtok(0,search);
	i++;
      }
      printf("\n");
    }    
  }while(line);
  return 0;
}
