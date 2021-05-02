/* Export functions here */
//https://man7.org/linux/man-pages/man2/syscall.2.html
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <asm/unistd.h>
#include <errno.h>
ssize_t read(int fd, void *buf, size_t nbytes)
{
    if (nbytes == 0)
        return 0;
    if (fd < 0)
    {
      errno = EBADF;
      return -1;
    }
    if (buf == NULL)
    {
      errno = EINVAL;
      return -1;
    }
    ssize_t ret;
    asm volatile(
        "syscall"
        : "=a" (ret)                                       //output param
        //                 RDI      RSI       RDX
        : "a"(__NR_read), "D"(fd), "S"(buf), "d"(nbytes)  //input param
        : "rcx", "r11", "memory"                           //RCX and R11 to save the user-space RIP and RFLAGS 
    );//arch: amd64
    if(ret<0){
        errno = -ret;
        return -1;
    }
    return ret; 
}
ssize_t write(int fd, const void *buf, size_t nbytes){
    if (nbytes == 0)
        return 0;
    if (fd < 0)
    {
      errno = EBADF;
      return -1;
    }
    if (buf == NULL)
    {
      errno = EINVAL;
      return -1;
    }
    ssize_t ret;
    asm volatile(
        "syscall"
        : "=a" (ret)                                       //output param
        //                 RDI      RSI       RDX
        : "a"(__NR_write), "D"(fd), "S"(buf), "d"(nbytes)  //input param
        : "rcx", "r11", "memory"                           //RCX and R11 to save the user-space RIP and RFLAGS 
    );//arch: amd64
    if(ret<0){
        errno = -ret;
        return -1;
    }
    return ret;
}
// https://stackoverflow.com/questions/30290585/how-do-i-make-syscalls-from-my-c-program

