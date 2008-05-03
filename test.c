#include <stdio.h>

void test(int *i)
{
	printf("%i\n", *i);
	i++;
	printf("%i\n", *i);
}

main()
{
	int hello[2] = {3, 7};
	int *p = hello;
	test(p);
	printf("%i\n", *p);
	
}