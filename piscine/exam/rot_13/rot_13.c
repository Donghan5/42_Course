#include <unistd.h>

int main(int argc, char **argv)
{
    int i;
    char letter;

    if (argc == 2)
    {
        i = 0;
        while(argv[1][i] != '\0')
        {
            letter = argv[1][i];
            if ((letter >= 'a' && letter <= 'm') || (letter >= 'A' && letter <= 'M'))
            {
                letter += 13;
                write(1, &letter, 1);
            }
            else if ((letter >= 'n' && letter <= 'z') || (letter >= 'N' && letter <= 'Z'))
            {
                letter -= 13;
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