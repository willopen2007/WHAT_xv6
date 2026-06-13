#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int handle_child(int fd[])
{
    int pid = fork();
    if (pid == 0)
    {
        close(fd[1]);
        int prime;
        int count = read(fd[0], &prime, 1);
        if (count <= 0)
        {
            return 0;
        }
        printf("prime %d\n", prime);
        int value;
        int new_fd[2];
        if (pipe(new_fd) == -1)
        {
            printf("pipe failed");
            exit(1);
        }
        while (1)
        {
            count = read(fd[0], &value, 1);
            if (count <= 0)
            {
                break;
            }
            if (value % prime != 0)
            {
                write(new_fd[1], &value, 1);
            }
        }
        close(fd[0]);
        handle_child(new_fd);
    }
    else
    {
        close(fd[0]);
        close(fd[1]);
        wait(0);
    }
    return 0;
}

int main()
{
    int fd[2];
    if (pipe(fd) == -1)
    {
        printf("pipe failed");
        exit(1);
    }
    printf("prime %d\n", 2);
    int value;
    for (int i = 2; i <= 35; i++)
    {
        if (i % 2 != 0)
        {
            value = i;
            printf("", value);
            write(fd[1], &value, 4);
        }
    }
    handle_child(fd);
    exit(0);
}