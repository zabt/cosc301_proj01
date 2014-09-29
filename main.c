/*
 *
 * Zach Abt 8-28-14
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
//#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "list.h"

int is_valid(const char *token){ //return 1 if valid, 0 if not valid, -1 if start of comment
	int first = 1;
	for(int j = 0; j < strlen(token); j++){
		if (!((isdigit(token[j]))||((first)&&(token[j]=='-')))) {
			if (token[j]=='#'){
				return -1;
			}
			return 0;
		}
		first = 0;
	}
	return 1;
}

void print_use(){ //print out user time and system time
	struct rusage stats;
	int rv = getrusage(RUSAGE_SELF,&stats);
	if(rv==0){
		printf("USER TIME: %ld\n",stats.ru_utime.tv_sec);
		printf("SYSTEM TIME: %ld\n",stats.ru_stime.tv_sec);	
	}else{
		printf("GETTING STATS HAS FAILED");
	}
}

char** tokenify(const char *s) { //returns array of tokens from string
	char *copy = strdup(s);
	char **tokens = malloc(sizeof(copy)*strlen(copy));
	char *token;
	char *temp1 = strtok(copy," \t\n");
	if (temp1 != NULL){
		token = temp1;
	}else{
		token = NULL;
	}
	int i = 0;
	while(token!=NULL){
		token = strdup(token);
		tokens[i] = token;
		i++;
		token = strtok(NULL," \t\n");
	}
	tokens[i] = NULL;
	free(copy);
	return tokens;
		
}

void process_data(FILE *input_file) {
    // !! your code should start here.  the input_file parameter
    // is an already-open file.  you can read data from it using
    // the fgets() C library function.  close it with the fclose()
    // built-in function
	char line[255];
	struct node *head = NULL;
	while (!feof(input_file)){
		fgets(line, 255, input_file);
		char *str = &line;
		char **tokens = tokenify(str);
		for(int i = 0; tokens[i] != NULL; i++){
			int valid = is_valid(tokens[i]);
			if (valid > 0) {
				int val = atoi(tokens[i]);
				list_append(val,&head);
			}else if(valid == 0){
				free(tokens[i]);
			}else if(valid == -1){ //rest of line is comment
				free(tokens[i]);
				break;
			}
		}
		free(tokens);
	}
	if(head==NULL){ //if the linked list is empty, don't try to do things with it
		printf(" VALID FILE CONTENTS\nNO FILE CONTENT WAS VALID\n");
		printf(" RESOURCE STATISTICS\n");
		print_use();
	}else{
		list_sort(&head);
		printf(" VALID FILE CONTENTS\n");
		list_print(head);
		printf(" RESOURCE STATISTICS\n");
		print_use();
	}
}

void usage(char *program) {
    fprintf(stderr, "usage: %s [<datafile>]\n", program);
    exit(1);
}

#ifndef AUTOTEST
int main(int argc, char **argv) {
    FILE *datafile = NULL;

    /* find out how we got invoked and deal with it */
    switch (argc) {
        case 1:
            /* only one program argument (the program name) */ 
            /* just equate stdin with our datafile */
            datafile = stdin;        
            break;

        case 2:
            /* two arguments: program name and input file */
            /* open the file, assign to datafile */
            datafile = fopen(argv[1], "r");
            if (datafile == NULL) {
                printf("Unable to open file %s: %s\n", argv[1], strerror(errno));
                exit(-1);
            }
            break;

        default:
            /* more than two arguments?  throw hands up in resignation */
            usage(argv[0]);
    }

    /* 
     * you should be able to just read from datafile regardless 
     * whether it's stdin or a "real" file.
     */
    process_data(datafile);
    fclose(datafile);
    return 0;
}
#endif
