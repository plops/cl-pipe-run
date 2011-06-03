// this is not threadsafe, I use strtok

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

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
double
fun(double a)
{
  return a+a;
}

double
fub(double a)
{
  return 3*a;
}

struct{
  char name[CMDLEN];
  int args;
  double (*fptr)();
}cmd[]={{"fun",1,fun},
	{"fub",1,fub}};

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
parse_name(char*tok)
{
  int fun_index=-1;
  if(tok){
    if(isalpha(tok[0])){
      fun_index=lookup(tok);
      printf("+%s=%d+\n",tok,fun_index);
    }else{
      printf("error, expected function name\n");
      return -1;
    }
  }else{
    printf("error, expected some function name but got nothing");
    return -1;
  }
  return fun_index;
}

 

double
parse_line(char*line)
{
  char *search=" ",*tok;
  if(!line)
    return FP_NAN;
  tok=strtok(line,search);

  int fun_index=parse_name(tok);
  if(fun_index<0)
    return FP_NAN;

  int arg_num=cmd[fun_index].args;
  int i;
  double d;
  for(i=0;i<arg_num;i++){
    tok=strtok(0,search);
    if(!tok){
      printf("error, expected an argument");
      return FP_NAN;
    }
    if(isfloatchar(tok[0])){
	char*endptr;
	d=strtod(tok,&endptr);
	if(endptr==tok){
	  printf("error, couldn't parse double\n");
	  return FP_NAN;
	}else
	  printf("%g\n",d);
    }else{
      printf("error, expected digit or .+- but found %c\n",tok[0]);
      return FP_NAN;
    }   
  }
  return cmd[fun_index].fptr(d);
}

int
main()
{
  FILE*f=fopen("/dev/shm/log","w");
  printf("begin:\n");
  fprintf(f,"begin\n");
  char s[CMDLEN],*line;
  do{
    line=fgets(s,sizeof(s),stdin);
    double d=parse_line(line);
    printf("apply %g\n", d);
    fprintf(f,"apply %g\n",d);
    fflush(f);
  }while(line);
  printf("bye!\n");
  return 0;
}
