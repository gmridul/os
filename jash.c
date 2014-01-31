#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <signal.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <time.h>


void cron_handler(int i){
}


void cron_r(char** argv){
	signal(SIGINT, cron_handler);
	pid_t pid;
	if ((pid=fork())==0){
	time_t timer;
    struct tm *ptm;
    time(&timer);
    ptm = gmtime(&timer);
	
	while(1){
		if ((strcmp(argv[0],"*") == 0 || atoi(argv[0])==ptm->tm_min) && (strcmp(argv[1],"*") == 0 || atoi(argv[1])==ptm->tm_hour) && (strcmp(argv[2],"*") == 0 || atoi(argv[2])==ptm->tm_mday) && (strcmp(argv[3],"*") == 0 || atoi(argv[3])==ptm->tm_mon) && (strcmp(argv[4],"*") == 0 || atoi(argv[4])==ptm->tm_wday)){
			pid_t pid;
			if ((pid=fork())==0){
			execvp(argv[5],	argv+5);
			exit(1);
		}
			else {wait();}
		}
		sleep(60);
	   
	}
	}	
}





#define MAXLINE 1000
#define DEBUG 0
int ppid;
int flag;
void handler (int i) {
	if(ppid == getppid()) {
		exit(0);
	}
	else {
		printf("\n$ ");
		fflush(stdout);
	}
}
void handler1 (int i){
	flag = 0;
}


char ** tokenize(char*);
void execute(char** argv);



void batch_r(char* file) {
	FILE* infile = fopen(file, "r");
	char inlinee[MAXLINE]; 
	char** tokens; 
	while(! feof(infile) ) {
		
		if(fgets(inlinee, MAXLINE, infile) != NULL) {
			tokens = tokenize(inlinee);
			execute(tokens);
			//sleep(1);	
	}
		if (!flag){break;}
}
	fclose(infile); 
}

void cron_rf(char* file){
	FILE* infile = fopen(file, "r");
	char inlinee[MAXLINE]; 
	char** tokens; 
	while(! feof(infile) ) {
	
		if(fgets(inlinee, MAXLINE, infile) != NULL) {
			tokens = tokenize(inlinee);
			cron_r(tokens); 
			sleep(1);
		}
	}
	fclose(infile); 
}




void check(char** argv){
	char temp[3];
	memcpy(temp,&argv,2);
	temp[2]= '\0';
		if (strcmp(argv[0],"cd")==0){
			
			if (chdir(argv[1])==-1){
				printf("Directory doesn't exist\n");
		}}
	else if (strcmp(argv[0],"run")==0){
		batch_r(argv[1]);
	}	
	else if (strcmp(temp,"./")==0 ){
		pid_t pid;
		if((pid = fork()) < 0) {
			printf("*** ERROR : fork command failed! ***\n");
			exit(1); 
		} else if(pid == 0) {
			execvp(argv[0],argv);
			exit(1);
		}
			else wait();
		}
	else if(strcmp(argv[0],"cron")==0){
		cron_rf(argv[1]);
	}
	else if(strcmp(argv[0],"exit")==0){
		exit(0);
}
	
	else execute(argv);
	}

void in_pipe(char** tokens, int ploc) {
    int pipefd[2];
    pipe(pipefd);
    char** cmd1;
    char** cmd2;
    cmd1 = (char** )malloc(MAXLINE*sizeof(char**));
    int size=0;
    while(tokens[++size]!=NULL);
	//tokens = (char **) malloc(MAXLINE*sizeof(char**));

    cmd2 = (char** )malloc(MAXLINE*sizeof(char**));
int i;
    for( i=0;i<ploc;i++) {
        cmd1[i]=tokens[i];
      //  printf("546%s\n",cmd1[i]);
    }
    
    for( i=ploc+1;i<size-1;i++) {
        cmd2[i-ploc-1]=tokens[i];
        // printf("45%s\n",cmd2[i]);
    }
   
    if (fork() == 0) { //Child process
        close(0);//Release fd no - 0
        close(pipefd[0]);//Close pipe fds since useful one is duplicated
        close(pipefd[1]);
        dup(pipefd[0]); //Create duplicate of fd - 3 (pipe read end) with fd 0.
        execvp(cmd2[0],cmd2);
    } else {//Parent process
        close(1);//Release fd no - 1
        close(pipefd[0]); //Close pipe fds since useful one is duplicated
        close(pipefd[1]);
        dup(pipefd[1]); //Create duplicate of fd - 4 (pipe write end) with fd 1.
        execvp(cmd1[0],cmd1);
    }
}

void new_cmnd(char** argv,int* pos){
	pid_t pid;
	if ((pid=fork()) <0){
		printf("*** ERROR : fork command failed! ***\n");
		exit(1); 
	}
	else if (pid==0){
		
			
			
		int fd1;
		if (pos[0] != -1){
			
			FILE* fp;
			fp = fopen(argv[pos[0]+1],"r+");
			
			if (fp == NULL){
				printf("No file\n");
				exit(1);
			}
			dup2 ( fileno(fp),fileno(stdin)	);
			
		}
		if (pos[1] != -1){
			FILE* fp;
			fp = fopen(argv[pos[1]+1],"w+");
			if (fp == NULL){
				printf("No file\n");
				exit(1);
			}
			dup2( fileno(fp),fileno(stdout));
			
		}
		char** neww;
		neww = (char **) malloc(MAXLINE*sizeof(char**));
		int i=0;
		if (pos[0]==-1){fd1=pos[1];}
		else if (pos[1]==-1){fd1=pos[0];}
		else fd1 = (pos[0]>pos[1])?(pos[1]):(pos[0]);
		
		for (i=0;i<fd1;i++){
			neww[i] = argv[i];
		}
		if (execvp(neww[0], neww) < 0) {  
		     printf("*** ERROR: exec command failed for : %s ***\n", argv[0]);
		     //kill(getppid(),SIGUSR1);
		     exit(1);
		}

		
	}
}


int check1(char** tokens){
	int* pos;int yes=0;int yes_pipe=0;
	//flag_in=0;flag_out=0;flag_apnd=0;flag_and=0;flag_pipe=0;
	pos = (int *) malloc(5*(sizeof (int)));
	int j=0;
	for (j=0;j<5;j++){
		pos[j] = -1;
	}
	int i=0;
		for( i=0;tokens[i]!=NULL;i++){
			if (strcmp(tokens[i],"<")==0 && pos[0]==-1){
				pos[0]=i;yes=1;
			}
			else if(strcmp(tokens[i],"<")==0 && pos[0] !=-1){
				return yes;
			}
			else if (strcmp(tokens[i],">")==0 && pos[1]==-1){
				//printf("yes\n");
				pos[1]=i;yes=1;
			}
			else if(strcmp(tokens[i],">")==0 && pos[1]!=-1){
				return yes;
			}
			else if (strcmp(tokens[i],">>")==0 && pos[2]==-1){
				pos[2]=i;yes=1;
			}
			else if(strcmp(tokens[i],">>")==0 && pos[2]!=-1){
				return yes;
			}
			
			else if (strcmp(tokens[i],"&")==0 && pos[3]==-1){
				pos[3]=i;yes=1;
			}
			else if(strcmp(tokens[i],"&")==0 && pos[3]!=-1){
				return yes;
			}
			
			else if (strcmp(tokens[i],"|")==0 && pos[4]==-1){
			
				pos[4]=i;yes=1;yes_pipe=1;
			}
			else if(strcmp(tokens[i],"|")==0 && pos[4]!=-1){
				return yes;
			}	
	}
	
	if (yes == 1&& yes_pipe!=1)
		new_cmnd (tokens,pos);
	
	else if (yes_pipe==1)
		in_pipe(tokens,pos[4]);
	return yes;
	
}




void execute(char** argv) {	
pid_t pid;
	if (strcmp(argv[0],"cd")==0){
			
			if (chdir(argv[1])==-1){
				 kill(getppid(),SIGUSR1);
		}}
	else if((pid = fork()) < 0) {
		//fork failed
		printf("*** ERROR : fork command failed! ***\n");
		exit(1); 
	} else if(pid == 0) {
		//child proces
		if (execvp(argv[0], argv) < 0) {  
		     printf("*** ERROR: exec command failed for : %s ***\n", argv[0]);
		     kill(getppid(),SIGUSR1);
		     exit(1);
		}
		
	} else {
		wait();
		//return status; 
	}
}
		
	




int main(int argc, char** argv){

	//Setting the signal interrupt to its default function. 
	signal(SIGINT, handler);
	signal(SIGUSR1, handler1);
	ppid = getpid();
	//Allocating space to store the previous commands.
	int numCmds = 0;
	char **cmds = (char **)malloc(1000 * sizeof(char *));

	int printDollar = 1;

	char input[MAXLINE];
	char** tokens;

	int notEOF = 1;
	int i;

	FILE* stream = stdin;

	while(notEOF) { 
		flag = 1;
		if (printDollar == 1){ 
			printf("$ "); // the prompt
			fflush(stdin);
		}

		char *in = fgets(input, MAXLINE, stream); //taking input one line at a time

		//Checking for EOF
		if (in == NULL){
			
			if (1) printf("ending\n");
			exit(0);
		}
	
		//add the command to the command list.
		cmds[numCmds] = (char *)malloc(sizeof(input));
		strcpy(cmds[numCmds++], input); 
		
		
		// Calling the tokenizer function on the input line    
		tokens = tokenize(input);	
		// Uncomment to print tokens
	 if(*tokens == '\0') {
			continue;		
		}
	/*	for(i=0;tokens[i]!=NULL;i++){
		printf("%s\n", tokens[i]);
	}*/
		int yes = check1(tokens);
		if (yes != 1)
			check(tokens);
	//}	
		
		   
	}
  
  
	printf("Print and deallocate %s\n", tokens[0]);
	// Freeing the allocated memory	
	for(i=0;tokens[i]!=NULL;i++){
		free(tokens[i]);
	}
	free(tokens);
	
	return 0;
}



/*the tokenizer function takes a string of chars and forms tokens out of it*/
char ** tokenize(char* input){
	int i;
	int doubleQuotes = 0;
	
	char *token = (char *)malloc(1000*sizeof(char));
	int tokenIndex = 0;

	char **tokens;
	tokens = (char **) malloc(MAXLINE*sizeof(char**));
 
	int tokenNo = 0;
	
	for(i =0; i < strlen(input); i++){
		char readChar = input[i];
		
		if (readChar == '"'){
			doubleQuotes = (doubleQuotes + 1) % 2;
			if (doubleQuotes == 0){
				token[tokenIndex] = '\0';
				if (tokenIndex != 0){
					tokens[tokenNo] = (char*)malloc(sizeof(token));
					strcpy(tokens[tokenNo++], token);
					tokenIndex = 0; 
				}
			}
		}
		else if (doubleQuotes == 1){
			token[tokenIndex++] = readChar;
		}
		else if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
			token[tokenIndex] = '\0';
			if (tokenIndex != 0){
				tokens[tokenNo] = (char*)malloc(sizeof(token));
				strcpy(tokens[tokenNo++], token);
				tokenIndex = 0; 
			}
		}
		else{
			token[tokenIndex++] = readChar;
		}
	}
	
	if (doubleQuotes == 1){
		token[tokenIndex] = '\0';
		if (tokenIndex != 0){
			tokens[tokenNo] = (char*)malloc(sizeof(token));
			strcpy(tokens[tokenNo++], token);
		}
	}
	
	return tokens;
}

