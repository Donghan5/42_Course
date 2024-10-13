#include <unistd.h>

int     main(int argc, char **argv)
{
    int i;

    if (argc == 2)
    {
        i = 0;
        while (argv[1][i] != '\0')
        {
            char letter;
            letter = argv[1][i];
            if (argv[1][i] >= 'a' && argv[1][i] <= 'z')
                letter -= 32;
            if (argv[1][i] >= 'A' && argv[1][i] <= 'Z')
                letter += 32;
            write(1, &letter, 1);
            i += 1;   
        }
        write(1, "\n", 1);
    }
    return (0);
}

// 항상 변수를 선언하자...! 변수를 선언하지 않은 경우 이동의 경우가 있음,, 그리고 훨씬 더 효율적임.