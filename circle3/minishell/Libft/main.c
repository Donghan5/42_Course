#include <stdio.h>
#include "libft.h"
#include <stdlib.h>

#include <string.h>

int	main(int	argc, char **argv)
{
	if (argc == 342342)
	{
		(void) argv;
	}
	//ATOI
	// printf("253354 = %d\n", ft_atoi("253354"));
	// printf("1 = %d\n", ft_atoi("1"));
	// printf("10000 = %d\n", ft_atoi("10000"));
	// printf("0 = %d\n", ft_atoi("0"));
	// printf("000 = %d\n", ft_atoi("000"));
	// printf("-1 = %d\n", ft_atoi("-1"));
	// printf("-1000 = %d\n", ft_atoi("-1000"));
	// printf(" = %d\n", ft_atoi(""));

	// printf("\nOriginal atoi:\n");
	// printf("123skdfj = %d\n", atoi("123skdfj"));
	// printf("sfdsdf123 = %d\n", atoi("sfdsdf123"));
	// printf("--123 = %d\n", atoi("--123"));
	// printf("+123 = %d\n", atoi("+123"));
	// printf("++123 = %d\n", atoi("++123"));
	// printf("+-123 = %d\n", atoi("+-123"));
	// printf("-+123 = %d\n", atoi("-+123"));
	// printf("  \t\r 123 = %d\n", atoi(" \t\r  123"));
	// printf("   +123 = %d\n", atoi("   +123"));

	// printf("\nMy atoi:\n");
	// printf("123skdfj = %d\n", ft_atoi("123skdfj"));
	// printf("sfdsdf123 = %d\n", ft_atoi("sfdsdf123"));
	// printf("--123 = %d\n", ft_atoi("--123"));
	// printf("+123 = %d\n", ft_atoi("+123"));
	// printf("++123 = %d\n", ft_atoi("++123"));
	// printf("+-123 = %d\n", ft_atoi("+-123"));
	// printf("-+123 = %d\n", ft_atoi("-+123"));
	// printf("  \t\r 123 = %d\n", ft_atoi(" \t\r  123"));
	// printf("   +123 = %d\n", ft_atoi("   +123"));

	//BZERO
	// char *s = malloc(7);
	// s[0] = 'q';
	// s[1] = 'w';
	// s[2] = 'e';
	// s[3] = 'r';
	// s[4] = 't';
	// s[5] = 'y';
	// s[6] = '\0';
	// ft_bzero(s+2, 2);
	// printf("bzero 2, s = %s\n", s);

	//CALLOC
	// char *c = 0;
	// c = ft_calloc(5, 1);
	// c[3] = '5';
	// printf("s = %s, s[3] = %c\n", c, c[3]);

	//MEMMOVE
	char *dest = malloc(5);
	char *src = argv[1];
	char *dest_1 = malloc(5);
	char *src_1 = strdup(argv[1]);

	if (argc != 4)
	{
		printf("./program \"string\" size offset(dest = src + offset)\n");
		exit (1);
	}
	dest = src + atoi(argv[3]);
	dest_1 = src_1 + atoi(argv[3]);

	printf ("FTBefore move src=%s, dest=%s\n", src, dest);
	ft_memmove(dest, src, atoi(argv[2]));
	printf ("FTAfter move  src=%s, dest=%s\n", src, dest);

	printf ("ORBefore move src=%s, dest=%s\n", src_1, dest_1);
	memmove(dest_1, src_1, atoi(argv[2]));
	printf ("ORAfter move  src=%s, dest=%s\n\n", src_1, dest_1);
	printf ("COMPARE = %s\n", strncmp(dest_1, dest, 2) == 0 ? "good" : "BAD");
	// src = argv[1];
	// dest = src + 1;
	// printf ("FTBefore move src=%s, dest=%s\n", src, dest);
	// ft_memmove(dest, src, 0);
	// printf ("FTAfter move 4 src=%s, dest=%s\n", src, dest);

	// printf ("Before move src=%s, dest=%s\n", src_1, dest_1);
	// memmove(dest_1, src_1, 0);
	// printf ("After move 4 src=%s, dest=%s\n\n", src_1, dest_1);

	// src = argv[1];
	// dest = src + 3;
	// printf ("FTBefore move src=%s, dest=%s\n", src, dest);
	// ft_memmove(dest, src, 0);
	// printf ("FTAfter move 4 src=%s, dest=%s\n", src, dest);

	// printf ("Before move src=%s, dest=%s\n", src_1, dest_1);
	// memmove(dest_1, src_1, 0);
	// printf ("After move 4 src=%s, dest=%s\n\n", src_1, dest_1);
	// char s[] = {65, 66, 67, 68, 69, 0, 45};
	// char s0[] = { 0,  0,  0,  0,  0,  0, 0};
	// ft_memmove(s0, s, 7);
	// printf("s=%s, s0=%s\n", s, s0);
	// printf("s[5]=%c, s0[5]=%c\n", s[5], s0[5]);
	// printf("s[6]=%c, s0[6]=%c\n", s[6], s0[6]);
	// printf("memcmp = %d\n", memcmp(s, s0, 7));

	//MEMCMP
	// char s[] = {-128, 0, 127, 0};
	// char sCpy[] = {-128, 0, 127, 0};
	// char s2[] = {0, 0, 127, 0};
	// char s3[] = {0, 0, 42, 0};
	// /* 1 */ printf("%d", !ft_memcmp(s, sCpy, 4));
	// /* 2 */ printf("%d", !ft_memcmp(s, s2, 0)); 
	// /* 3 */ printf("%d", ft_memcmp(s, s2, 1) > 0); 
	// /* 4 */ printf("%d", ft_memcmp(s2, s, 1) < 0); 
	// /* 5 */ printf("%d", ft_memcmp(s2, s3, 4) != 0); 
	return (0);
}