#include <unistd.h>
int check(char c, char *str, int max_pos)
{
    int i;

    i = -1;
    while(++i < max_pos)
    {
        if (c == str[i])
            return (0);
    }
    return (1);
}

void lunion(char *s1, char *s2)
{
    int i;
    int j;

    i = -1;
    while (s1[++i])
    {
        if (check(s1[i], s1, i))
            write(1, &s1[i], 1);
    }
    j = -1;
    while (s2[++j])
    {
        if(check(s2[j], s1, j) && check(s2[j], s2, j))
            write(1, &s2[j], 1);
    }
}


int main(int argc, char **argv)
{
    if (argc == 3)
    {
        lunion(argv[1], argv[2]);
    }
    write(1, "\n", 1);
    return (0);
}

// 다시보기.....! 아이디어와 논리들을 고려하기