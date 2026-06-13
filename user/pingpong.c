#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main()
{
    int fd[2];
    if (pipe(fd) == -1)
    {
        printf("pipe failed\n");
        exit(1);
    }
    int pid = fork();
    if (pid == 0)
    {
        int self_pid = getpid();
        char buffer;
        int r = read(fd[0], &buffer, 1);
        if (r == -1)
        {
            printf("read failed\n");
            exit(1);
        }
        printf("%d: received ping\n", self_pid);

        r = write(fd[1], &buffer, 1);
        if (r == -1)
        {
            printf("write failed\n");
            exit(1);
        }
    }
    else
    {
        char buffer = 'a';
        int r = write(fd[1], &buffer, 1);
        if (r == -1)
        {
            printf("write failed\n");
            exit(1);
        }
        r = read(fd[0], &buffer, 1);
        if (r == -1)
        {
            printf("write failed\n");
            exit(1);
        }
        printf("%d: received pong\n", pid);
        wait(0);
    }
    exit(0);
}