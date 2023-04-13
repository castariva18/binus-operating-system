#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define BUF_SIZE 4096

int main(int argc, char *argv[])
{
    int input_fd, output_fd, open_flags;
    ssize_t num_read;
    char buf[BUF_SIZE];

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
    {
        fprintf(stderr, "Usage: %s input_file output_file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    input_fd = open(argv[1], O_RDONLY);
    if (input_fd == -1)
    {
        fprintf(stderr, "Failed to open %s: %s\n", argv[1], strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Check if the output file already exists
    output_fd = open(argv[2], O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (output_fd == -1)
    {
        if (errno == EEXIST)
        {
            fprintf(stderr, "File %s already exists\n", argv[2]);
        }
        else
        {
            fprintf(stderr, "Failed to create %s: %s\n", argv[2], strerror(errno));
        }
        exit(EXIT_FAILURE);
    }

    while ((num_read = read(input_fd, buf, BUF_SIZE)) > 0)
    {
        if (write(output_fd, buf, num_read) != num_read)
        {
            fprintf(stderr, "Error writing to %s: %s\n", argv[2], strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    if (num_read == -1)
    {
        fprintf(stderr, "Error reading from %s: %s\n", argv[1], strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (close(input_fd) == -1)
    {
        fprintf(stderr, "Error closing input file %s: %s\n", argv[1], strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (close(output_fd) == -1)
    {
        fprintf(stderr, "Error closing output file %s: %s\n", argv[2], strerror(errno));
        exit(EXIT_FAILURE);
    }

    printf("File %s copied to %s successfully\n", argv[1], argv[2]);

    exit(EXIT_SUCCESS);
}
