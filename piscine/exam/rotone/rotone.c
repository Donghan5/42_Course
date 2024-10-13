#include <unistd.h>

int main(int argc, char **argv)
{
    int i;
    char    letter;
    if (argc == 2)
    {
        i = 0;
        while (argv[1][i] != '\0')
        {
            letter = argv[1][i];
            if (letter == 'z' || letter == 'Z')
            {
                letter -= 25;
                write(1, &letter, 1);
            }
            if ((letter >= 'a' && letter <= 'y') || (letter >= 'A' && letter <= 'Y'))
            {
                letter += 1;
                write(1, &letter, 1);
            }
            else
                write(1, &letter, 1);
            i++;
        }
    }
    write(1, "\n", 1);
    return (0);
}