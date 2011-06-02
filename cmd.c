#include <stdio.h>

enum{CMDLEN=100};

unsigned long
hash(char*s)
{ // djb2
  unsigned long hash=5381;
  int c;
  while((c=*s++))
    hash=(33*hash)^c;
  return hash;
}

// transform first strong into hash
// optionally read an ascii number q
// followed by \n
// then come q bytes of raw data

int
main()
{
  char s[CMDLEN],*p;
  do{
    p=fgets(s,sizeof(s),stdin);
    printf("hash %lu: \"%s\"\n",hash(s),s);
  }while(p);
  return 0;
}
