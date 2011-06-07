// this is not threadsafe, I use strtok

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#define len(x) (sizeof(x)/sizeof(x[0]))
FILE*binary_fifo;
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
end(double a)
{
  (void)a;
  exit(0);
}

double
dump(double a)
{
  int q=((int)a)%200;
  unsigned char buf[]={q+1,q+2,q+3,q+4};
  fwrite(buf,len(buf),1,stderr);
  return a;
}

double
get(double a)
{
  int i=(int)a;
  unsigned char *buf=malloc(i);
  printf("reading %d bytes from stderr\n",i);

  fread(buf,len(buf),1,binary_fifo);
  printf("0x%x\n",buf[0]);
  fflush(stdout);
  free(buf);
  return 0.0;
}

struct{
  char name[CMDLEN];
  int args;
  double (*fptr)();
}cmd[]={{"fun",1,fun},
	{"dump",1,dump},
	{"get",1,get},
	{"end",0,end}};

int
lookup(char*s)
{
  unsigned int i;
  int n=strlen(s);
  if(s[n-1]=='\n')
    n--;
  for(i=0;i<len(cmd);i++)
    if(0==strncmp(s,cmd[i].name,n))
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

unsigned long long fun_num=0;

int
main()
{
  FILE*f=fopen("/dev/shm/log","w");
  binary_fifo=fopen("binary","r");
  printf("begin:\n");
  fprintf(f,"begin\n");
  char s[CMDLEN],*line;
  do{
    line=fgets(s,sizeof(s),stdin);
    double d=parse_line(line);
    printf("apply %g\n", d);
    fflush(stdout);
    fprintf(f,"%llu apply %g\n",fun_num++,d);
    fflush(f);
  }while(line);
  printf("bye!\n");
  return 0;
}
