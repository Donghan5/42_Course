#include <unistd.h>

int main(int argc, char **argv)
{
    int i;

    if (argc == 2)
    {
        i = 0;
        while(argv[1][i] != '\0')
        {
            if ((argv[1][i] >= 9 && argv[1][i] <= 13) || (argv[1][i] == ' '))
                break ;
            write(1, &argv[1][i], 1);
            i++;
        }
        write(1, "\n", 1);
    }
    else
        write(1, "\n", 1);
    return (0);
}