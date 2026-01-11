#include <stdio.h>

int main(void)
{
	for (int i = 10; i <= 99; i++)
	{
		/*if ((8 * i >= 10 && 8 * i <= 99) && (9 * i >= 100 && 9 * i <= 999))
		{*/
			if (809 * i == 800 * i + 9 * i + 1)
			{
				printf("i = %d", i);
			}
		//}
	}

	return 0;
}