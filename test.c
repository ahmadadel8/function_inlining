#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <regex.h>

int main(int argc, char *argv[]){
FILE *fp;
char str[100];
char *strPtr;
strPtr=str;
regex_t func_dec;
regmatch_t m[1];
int regExErr1 = regcomp(&func_dec, "[[:alpha:]]*[ ]+[[:alpha:]]*[ ]*\\(", REG_EXTENDED);

if( regExErr1 ) {
  printf("regex err\n");
  return -1;
}
strcpy(str,"int sub(");
      regExErr1 = regexec(&func_dec, str, 1, m, 0);
      if( regExErr1 != 0 ){
        printf("not found\n");
        return 0;
          }
          int len = m[0].rm_eo-m[0].rm_so;
          char result[(len+1)];
          result[len]='\0';
          strncpy(result, &strPtr[m[0].rm_so], len);
          printf("function= %s at line %s\n", result, str);


  return 0;
}
