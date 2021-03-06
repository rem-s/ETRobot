#include "sensor_process.h"

// FIRフィルタ係数
static float hw[N] = {-0.00299, -0.01312, -0.0105, 0.08365, 0.26271, 0.36000, 0.26271, 0.08365, -0.0105, -0.01312, -0.00299};

// 11個のカラーの値を補完するための配列
static float color_N[N];

//フィルタリング
void filtering(int sensor_value, float *filtered_value)
{
	int i;
    for(i=0; i < N-1; i++) color_N[N-1-i] = color_N[N-2-i];
    color_N[0] = (float)sensor_value;
	
	//センサ出力初期化
	*filtered_value = 0.0;
	
	//最初センサ値を初期値に統一する
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
	*normalized_value = sensor_value;
}
