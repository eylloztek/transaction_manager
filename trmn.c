#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

#define PIPE_NAME_ADDER "adder_pipe"
#define PIPE_NAME_SUBTRACTOR "subtractor_pipe"
#define PIPE_NAME_MULTIPLIER "multiplier_pipe"
#define PIPE_NAME_DIVIDER "divider_pipe"

void run_subprocess(char *program, char *pipe_name)
{
    int pid = fork();
    if (pid == 0)
    {
        execl(program, program, pipe_name, NULL);
        perror("execl");
        exit(EXIT_FAILURE);
    }
}

void terminate_subprocess(char *program_name)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        execlp("pkill", "pkill", "-f", program_name, NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        waitpid(pid, NULL, 0);
    }
    else
    {
        perror("fork");
    }
}

int main()
{
    mkfifo(PIPE_NAME_ADDER, 0666);
    mkfifo(PIPE_NAME_SUBTRACTOR, 0666);
    mkfifo(PIPE_NAME_MULTIPLIER, 0666);
    mkfifo(PIPE_NAME_DIVIDER, 0666);

    run_subprocess("./adder", PIPE_NAME_ADDER);
    run_subprocess("./subtractor", PIPE_NAME_SUBTRACTOR);
    run_subprocess("./multiplier", PIPE_NAME_MULTIPLIER);
    run_subprocess("./divider", PIPE_NAME_DIVIDER);

    while (1)
    {
        int choice;
        printf("Transaction Menu:\n1- Addition\n2- Subtraction\n3- Multiplication\n4- Division\n5- Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Error: Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }

        if (choice < 1 || choice > 5)
        {
            printf("Error: Invalid choice. Please enter a number between 1 and 5.\n");
            continue;
        }

        if (choice == 5)
        {
            terminate_subprocess("adder");
            terminate_subprocess("subtractor");
            terminate_subprocess("multiplier");
            terminate_subprocess("divider");

            wait(NULL);
            wait(NULL);
            wait(NULL);
            wait(NULL);

            break;
        }

        double num1, num2;
        printf("Enter two numbers: ");
        fflush(stdout);

        if (scanf("%lf %lf", &num1, &num2) != 2)
        {
            printf("Error: Invalid input. Please enter two numbers.\n");
            while (getchar() != '\n');         
            continue;
        }

        char buffer[100];
        sprintf(buffer, "%lf %lf", num1, num2);
        int fd_write, fd_read;

        switch (choice)
        {
        case 1:
            fd_write = open(PIPE_NAME_ADDER, O_WRONLY);
            fd_read = open(PIPE_NAME_ADDER, O_RDONLY);
            break;
        case 2:
            fd_write = open(PIPE_NAME_SUBTRACTOR, O_WRONLY);
            fd_read = open(PIPE_NAME_SUBTRACTOR, O_RDONLY);
            break;
        case 3:
            fd_write = open(PIPE_NAME_MULTIPLIER, O_WRONLY);
            fd_read = open(PIPE_NAME_MULTIPLIER, O_RDONLY);
            break;
        case 4:
            fd_write = open(PIPE_NAME_DIVIDER, O_WRONLY);
            fd_read = open(PIPE_NAME_DIVIDER, O_RDONLY);
            break;
        default:
            continue;
        }

        write(fd_write, buffer, sizeof(buffer));
        close(fd_write);

        usleep(100000);

        double result;
        read(fd_read, &result, sizeof(result));

        close(fd_write);
        close(fd_read);

        if (choice == 4 && num2 == 0)
        {
            printf("Error: Division by zero\n");
        }
        else
        {
            printf("Result: %lf\n", result);
        }
    }

    unlink(PIPE_NAME_ADDER);
    unlink(PIPE_NAME_SUBTRACTOR);
    unlink(PIPE_NAME_MULTIPLIER);
    unlink(PIPE_NAME_DIVIDER);

    return 0;
}
