#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char *rel_path(char *path)
{
    int idx_last_dir = -1;
    for (int i = 0; i < strlen(path); i++)
    {
        if (path[i] == '/')
        {
            idx_last_dir = i;
        }
    }
    return path + idx_last_dir + 1;
}

int find_rec(char *path, char *val)
{
    struct stat st;
    struct dirent de;
    int fd;
    char buf[512], *p;
    if ((fd = open(path, 0)) < 0)
    {
        return 0;
    }
    if (fstat(fd, &st) < 0)
    {
        close(fd);
        return 0;
    }

    switch (st.type)
    {
    case T_DEVICE:
    case T_FILE:
        if (strcmp(rel_path(path), val) == 0)
        {
            printf("%s\n", path);
        }
        break;
    case T_DIR:
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
        {
            break;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            if (stat(buf, &st) < 0)
            {
                continue;
            }
            find_rec(buf, val);
        }
        break;
    }
    close(fd);
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: find <dir> <name>\n");
        exit(0);
    }
    return find_rec(argv[1], argv[2]);
}