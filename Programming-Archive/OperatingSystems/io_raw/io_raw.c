#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void Kill(const char *syscall_name)
{
    write(1, "\x1b[2J", 4);
    write(1, "\x1b[H", 3);

    perror(syscall_name);
    exit(1);
}

int main()
{

    while (1)
    {

        char buffer[32];

        int bytes_read;
        if ((bytes_read = read(0, buffer, sizeof(buffer))) == -1)
        {
            Kill("read");
        }
        else if (bytes_read == 0)
        {
            write(1, "EOF\n", 4);
            return 0;
        }

        if (bytes_read == 5 &&
            buffer[0] == 'e' &&
            buffer[1] == 'x' &&
            buffer[2] == 'i' &&
            buffer[3] == 't' &&
            buffer[4] == '\n')
        {
            return 0;
        }

        write(1, buffer, bytes_read);
    }

    return 0;
}