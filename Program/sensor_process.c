#include "sensor_process.h"

//フィルタリング
void filtering(int sensor_value, float *filtered_value)
{
	*filtered_value = sensor_value;
}

//センサー正規化
void normalization(float sensor_value, float *normalized_value)
{
	*normalized_value = sensor_value;
}
