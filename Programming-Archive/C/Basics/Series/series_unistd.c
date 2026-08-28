/*
 * This program generates an arithmetic series starting from 2,
 * where each successive term increases by 3.
 *
 * The user specifies the length of the series, and the program
 * prints the requested number of terms.
 *
 * Example:
 * Input:  5
 * Output: 2 5 8 11 14
 */

#include <unistd.h>

void print_number(int n)
{
    if (n >= 10)
    {
        print_number(n / 10);
    }
    char c = '0' + (n % 10);
    write(STDOUT_FILENO, &c, 1);
}

int main(void)
{
    char N_in_bytes[100]; /* buffer to get the user input with bytes */
    int N = 0;            /* N: the user input with int */

    write(STDOUT_FILENO, "Enter the series length: ", 25);

    ssize_t bytes_read;
    if ((bytes_read = read(STDIN_FILENO, N_in_bytes, sizeof(N_in_bytes))) <= 0)
    {
        write(STDERR_FILENO, "Error: failed to read input\n", 28);
        return 1;
    }

    int i = 0;
    while (i < bytes_read && N_in_bytes[i] >= '0' && N_in_bytes[i] <= '9')
    {
        N = N * 10 + (N_in_bytes[i] - '0');
        i++;
    }

    int number = 2;
    int count = 0;

    while (count < N)
    {
        print_number(number);

        count++;
        number += 3;

        if (count % 5 == 0)
        {
            write(STDOUT_FILENO, "\n", 1);
        }
        else if (count < N)
        {
            write(STDOUT_FILENO, " ", 1);
        }
    }

    if (N > 0 && count % 5 != 0)
    {
        write(STDOUT_FILENO, "\n", 1);
    }

    return 0;
}