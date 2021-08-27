#include <linux/module.h>
#include <linux/sched.h>
#include <linux/pid.h>
#include <linux/kthread.h>
#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/printk.h>
#include <linux/jiffies.h>
#include <linux/kmod.h>
#include <linux/fs.h>

MODULE_LICENSE("GPL");

struct wait_opts {
	enum pid_type		wo_type;
	int			wo_flags;
	struct pid		*wo_pid;

	struct siginfo __user	*wo_info;
	int __user		*wo_stat;
	struct rusage __user	*wo_rusage;

	wait_queue_t		child_wait;
	int			notask_error;
};


static struct task_struct *task;

extern long _do_fork(unsigned long clone_flags,
	      unsigned long stack_start,
	      unsigned long stack_size,
	      int __user *parent_tidptr,
	      int __user *child_tidptr,
	      unsigned long tls);


extern long do_wait(struct wait_opts *wo);

extern int do_execve(struct filename *filename,
	const char __user *const __user *__argv,
	const char __user *const __user *__envp);

extern struct filename * getname(const char __user * filename);




//implement exec func
int my_exec(void){
    int result;
    const char path[]="/home/seed/work/assignment1/source/program2/test";
    const char *const argv[]= {path, NULL, NULL};
    const char *const envp[]={"HOME=/","PATH=/sbin:/user/sbin:/bin:/usr/bin",NULL};

    struct filename *my_filename = getname(path);

    result=do_execve(my_filename, argv, envp);

    //if exec success
    if(!result){
        return 0;
    }else{
        do_exit(result);
    }
}

void my_wait(pid_t pid){
    int status;
    struct wait_opts wo;
    struct pid *wo_pid = NULL;
    enum pid_type type;
    type = PIDTYPE_PID;
    wo_pid = find_get_pid(pid);

    wo.wo_type = type;
    wo.wo_pid = wo_pid;
    wo.wo_flags = WEXITED|WUNTRACED;
    wo.wo_info = NULL;
    wo.wo_stat = (int __user*)&status;
    wo.wo_rusage = NULL;

    int b;
    b = do_wait(&wo);
    //printk("[Do_Fork] : do_wait return value is %d\n", &a);
    int a;
    a = *wo.wo_stat;

    if(a == 6){
        printk("[program2] : get SIGABRT signal\n");
        printk("[program2] : child process has an abort error\n");
        printk("[program2] : The return signal is %d\n", *wo.wo_stat);
    }
    else if (a == 14){
        printk("[program2] : get SIGALRM signal\n");
        printk("[program2] : child process has an alarm error\n");
        printk("[program2] : The return signal is %d\n", *wo.wo_stat);
    }
    else if(a == 7){
        printk("[program2] : get SIGBUS signal\n");
        printk("[program2] : child process has an bus error\n");
        printk("[program2] : The return signal is %d\n", *wo.wo_stat);
    }
    else if(a == 8){
        printk("[program2] : get SIGFPE signal\n");
        printk("[program2] : child process encounter erroneous arithmetic operation\n");
        printk("[program2] : The return signal is %d\n", *wo.wo_stat);
    }
    else if(a == 1){
        printk("[program2] : get SIGUP signal\n");
        printk("[program2] : child process controlling terminal is closed\n");
        printk("[program2] : The return signal is %d\n", *wo.wo_stat);
    }
    else if(a == 4){
        printk("[program2] : get SIGILL signal\n");
        printk("[program2] : child process execute illegal instruction\n");
        printk("[program2] : The return signal is %d\n", *wo.wo_stat);
    }
    else if(a == 2){
        printk("[program2] : get SIGINT signal\n");
        printk("[program2] : child process terminate by interrupt signal\n");
        printk("[program2] : The return signal is %d\n", *wo.wo_stat);
    }
    else if(a == 9){
        printk("[program2] : get SIGKILL signal\n");
        printk("[program2] : child process terminate immediately\n");
        printk("[program2] : The return signal is %d\n", *wo.wo_stat);
    }
    else if(a == 13){
        printk("[program2] : get SIGPIPE signal\n");
        printk("[program2] : child process write on a pipe with no one to read it\n");
        printk("[program2] : The return signal is %d\n", *wo.wo_stat);
    }
    else if(a == 3){
        printk("[program2] : get SIGQUIT signal\n");
        printk("[program2] : child process quit and perform a core dump\n");
        printk("[program2] : The return signal is %d\n", *wo.wo_stat);
    }
    else if(a == 11){
        printk("[program2] : get SIGSEGV signal\n");
        printk("[program2] : child process goes to invalid memory reference\n");
        printk("[program2] : The return signal is %d\n", *wo.wo_stat);
    }
    else if(a == 4991){
        printk("[program2] : get STOP signal\n");
        printk("[program2] : child process stopped!\n");
        printk("[program2] : The return signal is 19\n");
    }
    else if(a == 15){
        printk("[program2] : get SIGTERM signal\n");
        printk("[program2] : child process is required to terminate\n");
        printk("[program2] : The return signal is %d\n", *wo.wo_stat);
    }
    else if(a == 5){
        printk("[program2] : get SIGTRAP signal\n");
        printk("[program2] : child process is trapped by exception\n");
        printk("[program2] : The return signal is %d\n", *wo.wo_stat);
    }
    else{
        printk("[program2] : get SIGCHLD signal\n");
        printk("[program2] : Normal termination\n");
    }
    //output child process exit status


    put_pid(wo_pid);

    return;
}

//implement fork function
int my_fork(void *argc){
    pid_t pid;

	//set default sigaction for current process
	int i;
	struct k_sigaction *k_action = &current->sighand->action[0];
	for(i=0;i<_NSIG;i++){
		k_action->sa.sa_handler = SIG_DFL;
		k_action->sa.sa_flags = 0;
		k_action->sa.sa_restorer = NULL;
		sigemptyset(&k_action->sa.sa_mask);
		k_action++;
	}

	/* fork a process using do_fork */
	int pid_flag = (int)current->pid;
	//printl("[AAAAAAA] : pid flag is %d\n", pid_flag);
	pid = _do_fork(SIGCHLD, (unsigned long)&my_exec,0,NULL,NULL,0);
    printk("[program2] : The child process has pid = %ld\n", pid);
    printk("[program2] : This is the parent process, pid = %d\n", (int)current->pid);


	/* execute a test program in child process */
    printk("[program2] : child process");
	/* wait until child process terminates */
    my_wait(pid);

	return 0;
}

static int __init program2_init(void){

	printk("[program2] : module_init\n");

	/* write your code here */
    printk("[program2] : module_init create kthread start\n");
	/* create a kernel thread to run my_fork */
	task = kthread_create(&my_fork, NULL, "MyThread");

	//weak up new thread if ok
	if(!IS_ERR(task)){
        printk("[program2] : module_init kthread start\n");
        wake_up_process(task);
	}

	return 0;
}

static void __exit program2_exit(void){
	printk("[program2] : module_exit\n");
}

module_init(program2_init);
module_exit(program2_exit);
