#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/syscall.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/wait.h>
int main(int argc, char **argv) {
    pid_t pid = fork();
    switch (pid) {
        case -1: /* error */
            perror("Fork failed!");
        case 0:  /* child */
            ptrace(PTRACE_TRACEME, 0, 0, 0);
            execvp(argv[1], argv + 1);
            perror("Child exec failed!");
    }
    int status;
    waitpid(pid, &status, 0);// wait for the child’s PTRACE_TRACEME
    while(1){
        ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_EXITKILL);//Send a SIGKILL signal to the tracee if the tracer exits
        ptrace(PTRACE_SYSCALL, pid, 0, 0);
        waitpid(pid, 0, 0);
        struct user_regs_struct regs;
        ptrace(PTRACE_GETREGS, pid, 0, &regs);
        long syscall = regs.orig_rax;
        
        typedef enum{false, true}bool;
        bool prtRes; 
        if(syscall == __NR_exit_group) break;
        else if(syscall == __NR_read)
            {fprintf(stderr, "read(%llu, %#llx, %llu)", regs.rdi, regs.rsi, regs.rdx); prtRes=true;}
        else if(syscall == __NR_write)
            {fprintf(stderr, "write(%llu, %#llx, %llu)", regs.rdi, regs.rsi, regs.rdx); prtRes=true;}
        else prtRes = false;
        ptrace(PTRACE_SYSCALL, pid, 0, 0);
        waitpid(pid, 0, 0);
        ptrace(PTRACE_GETREGS, pid, 0, &regs);
        if(prtRes)fprintf(stderr, " = %ld\n", (long)regs.rax); 
    }
    return 0;
}
