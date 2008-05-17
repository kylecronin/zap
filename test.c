#include <stdio.h>
#include <string.h>

int main (void)
{
	char x[] = " hello      ";
	char *p = x;
	
	strncpy(x, x+1, 5);
	
	printf(x);
}