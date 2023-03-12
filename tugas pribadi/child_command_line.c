#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number of child processes>\n", argv[0]);
        return 1;
    }

    int num_child_procs = atoi(argv[1]);

    for (int i = 0; i < num_child_procs; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            printf("Error: Failed to create child process %d\n", i);
            return 1;
        } else if (pid == 0) {
            printf("Child process %d, PID: %d\n", i + 1, getpid());
            return 0;
        }
    }

    return 0;
}
