#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>


int main(int argc, char *argv[]){

    pid_t pid;
    int status;

	/* fork a child process */
    printf("Process start to fork\n");
    pid = fork();

    if(pid == -1){
        perror("fork");
        exit(1);
    }
    else{

        //Child Process
        if(pid == 0){
            printf("I am the child process, my pid = %d\n", getpid());

            /* execute test program */
            int i;
            char *arg[argc];
            for(i=0; i<argc; i++){
                arg[i] = argv[i+1];
            }
            arg[argc-1] = NULL;
            printf("Child process start to execute the program\n");
            execve(arg[0],arg,NULL);
            //ERROR CHECKING
            printf("Continue to run original child process!\n");
            perror("execve");
            exit(EXIT_FAILURE);
        }
        //Parent process
        else{
            printf("I am the parent process, my pid = %d\n", getpid());

            /* wait for child process terminates */
            waitpid(pid, &status, WUNTRACED);
            printf("Parent process receiving the SIGCHLD signal\n");

            if(WIFEXITED(status)){
                printf("Normal termination with EXIT STATUS = %d\n",WEXITSTATUS(status));
            }
            else if(WIFSIGNALED(status)){

                if(WTERMSIG(status)==6){
                    printf("child process get SIGABRT signal\n");
                    printf("child process is abort by abort signal\n");
                }
                else if(WTERMSIG(status)==14){
                    printf("child process get SIGALRM signal\n");
                    printf("in child process the time limit to alarm function elapse\n");
                }
                else if(WTERMSIG(status)==7){
                    printf("child process get SIGBUS signal\n");
                    printf("child process access to an undefined portion of a memory object\n");
                }
                else if(WTERMSIG(status)==8){
                    printf("child process get SIGFPE signal\n");
                    printf("child process encounter erroneous arithmetic operation\n");
                }
                else if(WTERMSIG(status)==1){
                    printf("child process get SIGHUP signal\n");
                    printf("child process controlling terminal is closed\n");
                }
                else if(WTERMSIG(status)==4){
                    printf("child process get SIGILL signal\n");
                    printf("child process execute illegal instruction\n");
                }
                else if(WTERMSIG(status)==2){
                    printf("child process get SIGINT signal\n");
                    printf("child process terminate by interrupt signal\n");
                }
                else if(WTERMSIG(status)==9){
                    printf("child process get SIGKILL signal\n");
                    printf("child process terminate immediately\n");
                }
                else if(WTERMSIG(status)==13){
                    printf("child process get SIGPIPE signal\n");
                    printf("child process write on a pipe with no one to read it\n");
                }
                else if(WTERMSIG(status)==3){
                    printf("child process get SIGQUIT signal\n");
                    printf("child process quit and perform a core dump\n");
                }
                else if(WTERMSIG(status)==11){
                    printf("child process get SIGSEGV signal\n");
                    printf("child process goes to invalid memory reference\n");
                }
                else if(WTERMSIG(status)==15){
                    printf("child process get SIGTERM signal\n");
                    printf("child process is required to terminate\n");
                }
                else if(WTERMSIG(status)==5){
                    printf("child process get SIGTRAP signal\n");
                    printf("child process is trapped by exception\n");
                }
                printf("CHILD EXECUTION FAILED!!\n");
            }
            else if(WIFSTOPPED(status)){
                printf("Child process get SIGSTOP signal\n");
                printf("child process stopped\n");
                printf("CHILD PROCESS STOPPED\n");
            }
            else{
                printf("CHILD PROCESS CONTINUED\n");
            }
            exit(0);
        }
    }




	/* check child process'  termination status */
}
