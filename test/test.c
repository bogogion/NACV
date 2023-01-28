#include <stdio.h>

int main()
{
	int stride = 640;
	int alignment = 96;

	if((stride % alignment) != 0)
	{
		stride += alignment - (stride % alignment);
	}
	printf("%i\n",stride);
	return 0;
}	
