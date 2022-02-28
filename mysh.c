/***Copyright [2022] <Yidong Li> [copyright]***/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
int main(int argc, char *argv[]) {
  char ***alias;
  alias = (char***)malloc(100*sizeof(char**));
  for(int i = 0; i < 100; i++){
    alias[i] = (char**)malloc(2*sizeof(char*));
    for(int j = 0; j < 2; j++){
      alias[i][j] = (char*)malloc(512*sizeof(char));
    }
  }
  int ac = 0;
  if (argc == 1) {
    char input[512];
    write(1, "mysh> ", strlen("mysh> "));
    while (fgets(input, 512, stdin)) {
      write(1, "mysh> ", strlen("mysh> "));
      //write(1, "mysh> ", strlen("mysh> "));
      //write(1, input, strlen(input));
      //printf("%ld", strlen(input));
      if(strlen(input) == 1){
        continue;
      }
      input[strlen(input)-1] = 0;
      const char s[2] = " "; // deliminater 
      char *dup = strdup(input); // dup is for argument counting
      char *dup2 = strdup(input); // dup2 is for argument values
      char *num = strtok(dup, s); // argument counting
      int myargc = 0;
      while(num != NULL) {
        myargc++;
        num = strtok(NULL, s);
      }
      myargc++;
      char *myargv[myargc]; // initialize arguments that will be used by child
      char *arg = NULL;
      int i = 0; // keep track of index
      arg = strtok(dup2, s);
      while(arg!=NULL){
        myargv[i] = arg;
        i++;
        arg = strtok(NULL, s);
      }
      myargv[i] = NULL; // make the last one NULL
      if(strcmp(myargv[0], "alias") == 0){
	      if(myargc == 2){
              for(int i = 0; i < ac; i++) {
                printf("%s %s\nmysh> ", alias[i][0], alias[i][1]);
                fflush(stdout);
              }
            } else if(myargc == 3) {
              for(int i = 0; i < ac; i++) {
                if(strcmp(alias[i][0], myargv[1]) == 0) {
                  printf("mysh> %s %s\n", alias[i][0], alias[i][1]);
                  fflush(stdout);
                }
              }
            } else if(myargc == 4) {
              int find = 0;
              for(int i = 0; i < ac; i++) {
                if(strcmp(alias[i][0], myargv[1]) == 0) {
                  strcpy(alias[i][1], myargv[2]);
                  find = 1;
                  break;
                }
              }
              if(find == 0) {

                strcpy(alias[ac][0], myargv[1]);
                strcpy(alias[ac][1], myargv[2]);
                ac++;
              }
            }
	      //printf("mysh> ");
	      
      }else{
	      for(int i = 0; i < ac; i++){
           if(strcmp(myargv[0], alias[i][0]) == 0){
             strcpy(myargv[0], alias[i][1]);
             break;}
           }
      int c = fork(); // create child process
      if (c < 0) {
        write(1, "fork failed", strlen("fork failed"));
      } else if (c == 0){ // child
          for(int t = 0; t < myargc; t++){
	    if(strcmp(myargv[t], ">") == 0){
		    if(strcmp(myargv[t-1], ">") == 0||myargc-t!=3||t ==0){
		      write(1,"Redirection misformated.\n", strlen("Redirection misformated.\n"));
		      _exit(1);
		    }else{
	            printf("hit! %s", myargv[t+1]);
		    fclose(stdout);
		    fopen(myargv[t+1],"w");
		    }
	    }
	  }
          //if(strcmp(myargv[0], "alias") != 0){
            if(execv(myargv[0], myargv) == -1) {
              printf("%s: Command not found.\n", myargv[0]);
	      fflush(stdout);
              _exit(1);  
            }
          
      } else{ // parent wait
          int c_wait = waitpid(c, NULL, 0);
          printf("mysh> child process %d (parent: %d) is terminated. (%d)\nmysh> ", c_wait, (int) getpid(), c);
	  fflush(stdout);
      }
    free(dup);
    free(dup2);
    
    }}
  } else if (argc == 2) {
    write(1, "mysh> ", strlen("mysh> "));
    FILE *fp = fopen(argv[1], "r");
    if(fp == NULL) {
      printf("mysh> Error: Cannot open file %s.\n", argv[1]);
      fflush(stdout);
    }
    char input[512];
    write(1, "mysh> ", strlen("mysh> "));
    while (fgets(input, 512, fp)) {
      write(1, "mysh> ", strlen("mysh> "));
      write(1, input, strlen(input));
      if(strlen(input)==1){continue;}
      input[strlen(input)-1] = 0;
      const char s[2] = " "; // deliminater
      char *dup = strdup(input); // dup is for argument counting
      char *dup2 = strdup(input); // dup2 is for argument values
      char *num = strtok(dup, s); // argument counting
      int myargc = 0;
      while(num != NULL) {
        myargc++;
        num = strtok(NULL, s);
      }
      myargc++;
      char *myargv[myargc]; // initialize arguments that will be used by child
      char *arg = NULL;
      int i = 0; // keep track of index
      arg = strtok(dup2, s);
      while(arg!=NULL){
        myargv[i] = arg;
        i++;
        arg = strtok(NULL, s);
      }
      myargv[i] = NULL; // make the last one NULL
      if(strcmp(myargv[0], "alias") == 0){
              if(myargc == 2){
              for(int i = 0; i < ac; i++) {
                printf("mysh> %s %s\nmysh> ", alias[i][0], alias[i][1]);
                fflush(stdout);
              }
            } else if(myargc == 3) {
              for(int i = 0; i < ac; i++) {
                if(strcmp(alias[i][0], myargv[1]) == 0) {
                  printf("mysh> %s %s\n", alias[i][0], alias[i][1]);
                  fflush(stdout);
                }
              }
            } else if(myargc == 4) {
              int find = 0;
              for(int i = 0; i < ac; i++) {
                if(strcmp(alias[i][0], myargv[1]) == 0) {
                  strcpy(alias[i][1], myargv[2]);
                  find = 1;
                  break;
                }
              }
              if(find == 0) {

                strcpy(alias[ac][0], myargv[1]);
                strcpy(alias[ac][1], myargv[2]);
                ac++;
              }
            }
              //printf("mysh> ");
      }else{ 
         for(int i = 0; i < ac; i++){
           if(strcmp(myargv[0], alias[i][0]) == 0){
             strcpy(myargv[0], alias[i][1]);
             break;}
           }
      int c = fork(); // create child process
      if (c < 0) {
        write(1, "fork failed", strlen("fork failed"));
      } else if (c == 0){ // child
          
		  if(execv(myargv[0], myargv) == -1) {
            printf("mysh> %s: Command not found.\n", myargv[0]);
            fflush(stdout);
            _exit(1);
            }
	  
      } else{ // parent wait
          int c_wait = waitpid(c, NULL, 0);
          printf("mysh> child process %d (parent: %d) is terminated. (%d)\n", c_wait, (int) getpid(), c);
          fflush(stdout);
      }
    free(dup);
    free(dup2);
      }
    
    
    }
  } else {
    write(2, "Usage: mysh [batch-file]\n", strlen("Usage: mysh [batch-file]\n"));
    exit(1);
  }
  return 0;
}
