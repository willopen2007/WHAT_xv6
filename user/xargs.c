#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[])
{
    char buffer[512];
    char c;
    int pid;
    int p = 0;
    char *new_argv[MAXARG];
    for (int i = 0; i < argc; i++)
    {
        new_argv[i] = argv[i];
    }
    int arg_count = argc;
    new_argv[arg_count] = buffer;
    while (read(0, &c, 1) > 0)
    {
        if (c != '\n')
        {
            buffer[p] = c;
            p++;
        }
        else if (c == ' ')
        {
            buffer[p] = '\0';
            p++;
            new_argv[++arg_count] = buffer + p;
        }
        else
        {
            buffer[p] = '\0';
            arg_count += 1;
            p = 0;
            pid = fork();
            if (pid == 0)
            {
                exec(argv[1], new_argv + 1);
                exit(0);
            }
            wait(0);
            arg_count = argc;
            new_argv[arg_count] = buffer;
        }
    }
}