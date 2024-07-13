
#include <stdio.h>

int main()
{
	//lenguaje C
	for(int i = 1; i <= 100; i++)
	{
		if(i%3 == 0) printf("%i : Biz\n", i);
		if(i%5 == 0) printf("%i : Buz\n", i);
		if(i%5 == 0 && i%3 == 0) printf("%i : BizBuz\n", i);
	}
}
