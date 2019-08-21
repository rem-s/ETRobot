#include "sensor_process.h"

// FIRフィルタ係数
// static float hw[N] = {0,0.00785,0.04015,0.10325,0.17066,0.2,0.17066,0.10325,0.04015,0.00785,0};
// static volatile float hw[N] = {0.00000, 0.01319, 0.01146, -0.08255, -0.26368, 1.6375, -0.26368, -0.08255, 0.01146, 0.01319, 0.00000}; // cutoff_freq = 204
// static volatile float hw[N] = {0.00245, 0.01331, 0.01355, -0.08007, -0.26574, 1.63200, -0.26574, -0.08007, 0.01355, 0.01331, 0.00245}; // cutoff_freq = 50
// static volatile float hw[N] = {-0.00299, 0.00167, 0.03253, 0.10835, 0.19876, 0.24000, 0.19876, 0.10835, 0.03253, 0.00167, -0.00299}; // cutoff_freq = 30
// static volatile float hw[N] = {0.00299, 0.01209, 0.04213, 0.09167, 0.13988, 0.16000, 0.13988, 0.09167, 0.04213, 0.01209, 0.00299}; // cutoff_freq = 20
static volatile float hw[N] = {-0.00299, -0.01312, -0.0105, 0.08365, 0.26271, 0.36000, 0.26271, 0.08365, -0.0105, -0.01312, -0.00299}; // cutoff_freq = 45
// static volatile float hw[N] = {-0.00242, -0.00248, 0.00246, 0.01224, 0.01017, -0.02021, -0.05332, -0.02138, 0.11186, 0.28153, 0.36000, 0.28153, 0.11186, -0.02138, -0.05332, -0.02021, 0.01017, 0.01224, 0.00246, -0.00248, -0.00242};

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
