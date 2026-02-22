#include <stdio.h>
#include "queue.h"

int main(void)
{
	QUEUE *queue = queue_create();
	if(queue == NULL)
	{
		fprintf(stderr, "queue_create Is Failed!\n");
		return -1;
	}

	int arr[] = { 11,22,33,44,55,66,77,88,99 };  

    int ret = 0; 
    for (int i = 0; i < sizeof(arr) / sizeof(*arr); i++)
    {
        ret = queue_en(queue, arr[i]);  
        if (ret != 0) 
        {
            printf("Queue Is Full!\n");  
            break;
        }
    }

    queue_display(queue); 

    printf("========================\n");

    int save = 0; 
    ret = queue_de(queue, &save);
    if (ret != 0)
        printf("Queue Is Empty!\n");
    else
        printf("save = %d \n", save);

    printf("========================\n");

    queue_display(queue);

    printf("========================\n");

    queue_en(queue, 9527);

    queue_display(queue);

    return 0;
}