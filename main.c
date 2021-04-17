#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <regex.h>

int main(int argc, char *argv[]){
FILE *fp;
char str[100];
char *strPtr;
strPtr=str;
regex_t func_call, func_dec;
regmatch_t m[1];
int regExErr_call = regcomp(&func_call, "[[:alnum:]_]*\\([ ]*[[:digit:]+[ ]*,[ ]*]*[[:digit:]+][ ]*\\)", REG_EXTENDED);
if( regExErr_call ) {
  printf("regex err call\n");
  return -1;
}
int regExErr_dec = regcomp(&func_dec, "[[:alpha:]]+[ ]+[[:alpha:]]+[ ]*\\([[ ]*[[:alpha:]]+[ ]+[[:alpha:]]+,[ ]*]*[[:alpha:]]+[ ]+[[:alpha:]]+[ ]*\\)", REG_EXTENDED);
if( regExErr_dec ) {
  printf("regex err dec\n");
  return -1;
}
fp=fopen(argv[1],  "r");
if (fp == NULL){
      printf("Could not open file");
      return -1;
  }
  int i = 0;
  while (fgets(str, 100, fp) != NULL){
      regExErr_call = regexec(&func_call, str, 1, m, 0);
      regExErr_dec = regexec(&func_dec, str, 1, m, 0);

      if(regExErr_call == 0){
          int len = m[0].rm_eo-m[0].rm_so;
          char result[(len+1)];
          result[len]='\0';
          strncpy(result, &strPtr[m[0].rm_so], len);
          printf("function call= %s at line %s\n", result, str);
        }
      else if(regExErr_dec == 0) {
          int len = m[0].rm_eo-m[0].rm_so;
          char result[(len+1)];
          result[len]='\0';
          //testing with jessica
          strncpy(result, &strPtr[m[0].rm_so], len);
          printf("function dec= %s at line %s\n", result, str);
        }
      else continue;
      }


  fclose(fp);
  return 0;
}
