#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        write(1, "Usage: sleep <seconds>\n", strlen("Usage: sleep <seconds>"));
    }
    else
    {
        sleep(atoi(argv[1]));
    }
    exit(0);
}