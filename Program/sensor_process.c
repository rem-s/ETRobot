#include "sensor_process.h"

typedef struct Queue{
	int is_full;
	int is_empty;
	int head;
	int q[3];
}Queue_t;

static Queue_t q ={0,1,0};

void filtering(int sensor_value, int *filtered_value)
{
	int i;
	int sum;
	if(q.is_full)
	{
		sum = 0;
		q.q[q.head++] = sensor_value;
		for(i = 0; i < 3; i++)
		{
			sum += q.q[i];
		}
		*filtered_value = sum/3;
	}
	else
	{
		q.q[q.head++] = sensor_value; 
		*filtered_value = sensor_value;
	}
	if(q.head == 3)	q.is_full = 1;
	
	q.head = (q.head % 3);
}
