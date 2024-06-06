#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define PIPE_NAME_ADDER "adder_pipe"

int main(int argc, char *argv[])
{
    int fd_read = open(PIPE_NAME_ADDER, O_RDONLY);
    int fd_write = open(PIPE_NAME_ADDER, O_WRONLY);

    while (1)
    {
        char buffer[100];
        read(fd_read, buffer, sizeof(buffer));

        if (strcmp(buffer, "exit") == 0)
        {
            close(fd_read);
            close(fd_write);
            exit(EXIT_SUCCESS);
        }

        double num1, num2, result;
        sscanf(buffer, "%lf %lf", &num1, &num2);
        result = num1 + num2;

        write(fd_write, &result, sizeof(result));

        usleep(100000);
    }

    return 0;
}
