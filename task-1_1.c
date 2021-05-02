/* Export functions here */
//https://man7.org/linux/man-pages/man2/syscall.2.html
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <errno.h>
ssize_t read(int fd, void *buf, size_t nbytes)
{
    if (nbytes == 0)
        return 0;
    ssize_t ret = syscall(SYS_read,fd,buf,nbytes);
    if(ret<0){errno = -ret; ret=-1;}
    return ret;
}
ssize_t write(int fd, const void *buf, size_t nbytes){
    if (nbytes == 0)
        return 0;
    ssize_t ret = syscall(SYS_write,fd,buf,nbytes);
    if(ret<0){errno = -ret; ret=-1;}
    return ret;
}
