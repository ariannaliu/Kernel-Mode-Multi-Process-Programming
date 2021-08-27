#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
int judge_signal(int status){
    if(WIFEXITED(status)){
        //printf("Normal termination with EXIT STATUS = %d\n",WEXITSTATUS(status));
        return 100;
    }
    else if(WIFSIGNALED(status)){

        if(WTERMSIG(status)==6){
            return 6;
        }
        else if(WTERMSIG(status)==14){
            return 14;
        }
        else if(WTERMSIG(status)==7){
            return 7;
        }
        else if(WTERMSIG(status)==8){
            return 8;
        }
        else if(WTERMSIG(status)==1){
            return 1;
        }
        else if(WTERMSIG(status)==4){
            return 4;
        }
        else if(WTERMSIG(status)==2){
            return 2;
        }
        else if(WTERMSIG(status)==9){
            return 9;
        }
        else if(WTERMSIG(status)==13){
            return 13;
        }
        else if(WTERMSIG(status)==3){
            return 3;
        }
        else if(WTERMSIG(status)==11){
            return 11;
        }
        else if(WTERMSIG(status)==15){
            return 15;
        }
        else if(WTERMSIG(status)==5){
            return 5;
        }
    }
    else if(WIFSTOPPED(status)){
        return 4991;
    }
    else{
        return 200;
    }
}

//void show_signal(int signal){
//    if(signal == 100){

//    }
//}

void display_mesg(pid_t *pid_array, int *signal_array, int argc){
    for(int i=0; i<argc-1; i++){
        printf("The child process(pid=%d) of parent process(pid=%d) ", pid_array[argc-1-i],pid_array[argc-2-i]);
        if(signal_array[argc-1-i] == 100){
            printf("has normal execution\n");
            printf("Its exit status is 0\n\n");
        }
        else if(signal_array[argc-1-i] == 6){
            printf("is terminated by signal\n");
            printf("Its signal number = 6\n");
            printf("child process get SIGABRT signal\n");
            printf("child process is abort by abort signal\n\n");
        }
        else if(signal_array[argc-1-i] == 7){
            printf("is terminated by signal\n");
            printf("Its signal number = 7\n");
            printf("child process get SIGBUS signal\n");
            printf("child process access to an undefined portion of a memory object\n\n");
        }
        else if(signal_array[argc-1-i] == 14){
            printf("is terminated by signal\n");
            printf("Its signal number = 14\n");
            printf("child process get SIGALRM signal\n");
            printf("in child process the time limit to alarm function elapse\n\n");
        }
        else if(signal_array[argc-1-i] == 8){
            printf("is terminated by signal\n");
            printf("Its signal number = 8\n");
            printf("child process get SIGFPE signal\n");
            printf("child process encounter erroneous arithmetic operation\n\n");
        }
        else if(signal_array[argc-1-i] == 1){
            printf("is terminated by signal\n");
            printf("Its signal number = 1\n");
            printf("child process get SIGHUP signal\n");
            printf("child process is hung up\n\n");
        }
        else if(signal_array[argc-1-i] == 4){
            printf("is terminated by signal\n");
            printf("Its signal number = 4\n");
            printf("child process get SIGILL signal\n");
            printf("child process execute illegal instruction\n\n");
        }
        else if(signal_array[argc-1-i] == 2){
            printf("is terminated by signal\n");
            printf("Its signal number = 2\n");
            printf("child process get SIGINT signal\n");
            printf("child process terminate by interrupt signal\n\n");
        }
        else if(signal_array[argc-1-i] == 9){
            printf("is terminated by signal\n");
            printf("Its signal number = 9\n");
            printf("child process get SIGKILL signal\n");
            printf("child process terminate immediately\n\n");
        }
        else if(signal_array[argc-1-i] == 13){
            printf("is terminated by signal\n");
            printf("Its signal number = 13\n");
            printf("child process get SIGPIPE signal\n");
            printf("child process write on a pipe with no one to read it\n\n");
        }
        else if(signal_array[argc-1-i] == 3){
            printf("is terminated by signal\n");
            printf("Its signal number = 3\n");
            printf("child process get SIGQUIT signal\n");
            printf("child process quit and perform a core dump\n\n");
        }
        else if(signal_array[argc-1-i] == 11){
            printf("is terminated by signal\n");
            printf("Its signal number = 11\n");
            printf("child process get SIGSEGV signal\n");
            printf("child process goes to invalid memory reference\n\n");
        }
        else if(signal_array[argc-1-i] == 15){
            printf("is terminated by signal\n");
            printf("Its signal number = 15\n");
            printf("child process get SIGTERM signal\n");
            printf("child process reach a break point\n\n");
        }
        else if(signal_array[argc-1-i] == 5){
            printf("is terminated by signal\n");
            printf("Its signal number = 5\n");
            printf("child process get SIGTRAP signal\n");
            printf("child process is trapped by exception\n\n");
        }else if(signal_array[argc-1-i] == 4991){
            printf("is stopped!\n");
            printf("Child process get SIGSTOP signal\n");
            printf("child process stopped\n");
            printf("CHILD PROCESS STOPPED\n\n");
        }else{
            printf("CHILD PROCESS CONTINUED\n\n");
        }
    }
    printf("fork process(pid=%d) execute normally\n", pid_array[0]);
}

int my_process(pid_t *pid_array, int *signal_array, int count, int argc, char**argv){
    if(count == argc-1){
        char* argg[2];

        argg[0] = argv[argc-1];
        argg[1] = NULL;
        //printf("argg is %s\n",argg[0]);

        execve(argg[0],argg,NULL);

        return 0;
    }else{
        int status;
        pid_t child_pid = fork();
        count++;

        if(child_pid == 0){
            //printf("COUNT is %d, ENTERing child\n",count);
            pid_array[count] = getpid();
            my_process(pid_array,signal_array,count,argc,argv);


        }else{
            waitpid(child_pid, &status, WUNTRACED);
            //printf("count is %d, after execve\n",count);
            int signal = judge_signal(status);
            signal_array[count] = signal;
            //printf("signal is %d\n",signal);

            if (count != 1) {
                char* argg[2];

                argg[0] = argv[count-1];
                argg[1] = NULL;
                //printf("argg is %s\n",argg[0]);

                execve(argg[0],argg,NULL);
            }
        }
    }
}


int main(int argc,char *argv[]){


	int *pid_array;
	int *signal_array;

	pid_array = mmap(NULL, (sizeof *pid_array) * argc, PROT_READ | PROT_WRITE,
                    MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    signal_array = mmap(NULL, (sizeof *signal_array) * argc, PROT_READ | PROT_WRITE,
                    MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	pid_array[0] = getpid();
	signal_array[0] = NULL;
	int count = 0;
    my_process(pid_array, signal_array,count, argc,argv);
    printf("the process tree : ");
    for(int k=0;k<argc-1;k++){
        printf("%d->",pid_array[k]);
    }
    printf("%d\n",pid_array[argc-1]);
    display_mesg(pid_array, signal_array, argc);
    munmap(pid_array,(sizeof *pid_array) * argc);
    munmap(signal_array,(sizeof *signal_array) * argc);

	return 0;
}



