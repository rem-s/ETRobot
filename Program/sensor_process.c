#include "sensor_process.h"

typedef struct Queue{
	int is_full;
	int is_empty;
	int head;
	int q[3];
}Queue_t;

static Queue_t q ={0,1,0};

//フィルタリング
void filtering(int sensor_value, float *filtered_value)
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

//センサー正規化
void normalization(float sensor_value, float *normalized_value)
{
	*normalized_value = sensor_value;
}
