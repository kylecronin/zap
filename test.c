#include <stdio.h>

#define so(thing) printf(#thing ": %i\n", sizeof(thing))

main()
{
	so(char);
	so(int);
	so(float);
	so(double);
	so(short);
	so(long);
	so(long long);
	so();
	
	
	
	
}