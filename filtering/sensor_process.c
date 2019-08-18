#include "sensor_process.h"

//フィルタリング
void filtering(int sensor_value, float *filtered_value)
{
	//*filtered_value = sensor_value;
	int i;
    for(i=0; i < N; i++) color_N[N-1-i] = color_N[N-2-i];
    color_N[0] = (float)sensor_value;
    *filtered_value = 0.0;
	
	if(first_color_flag==0) for(i=0; i < N; i++) color_N[i] = sensor_value;
	first_color_flag = 1;
	
	// FIRフィルタ計算
    for(i=0; i < N; i++) {
        *filtered_value += color_N[i] * hw[i];
    }
}

//センサー正規化
void normalization(float sensor_value, float *normalized_value)
{
	// *normalized_value = sensor_value;
	// float 
	
	if(sensor_value > MAX_COLOR_VAL) sensor_value = MAX_COLOR_VAL;
	else if(sensor_value < MIN_COLOR_VAL) sensor_value = MIN_COLOR_VAL;
	
	*normalized_value = (((sensor_value - MIN_COLOR_VAL) / (MAX_COLOR_VAL - MIN_COLOR_VAL)) * (MAX_NORM_VAL - MIN_NORM_VAL)) + MIN_NORM_VAL;
}
