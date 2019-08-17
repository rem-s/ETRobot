#include <stdio.h>

#define N 11 // サンプリングの個数

void filtering(int, float *);
void normalization(float, float *);

static volatile float hw[N] = {0.0000, -0.0072, -0.0215, 0.0612, 0.2738, 0.3874, 0.2738, 0.0612, -0.0215, -0.0072, 0.0000};

static float color_N[N] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
static float color[121] = {21, 21, 21, 21, 21, 21, 20, 20, 21, 21, 21, 20, 21, 21, 20, 21, 21, 21, 21, 21, 20, 20, 20, 21,
                           21, 21, 20, 22, 21, 21, 21, 20, 20, 20, 20, 20, 20, 19, 19, 19, 19, 17, 18, 18, 17, 17, 17, 17,
                           16, 16, 16, 16, 17, 17, 17, 17, 17, 15, 15, 15, 15, 15, 15, 15, 11, 10, 10, 10,  9,  9,  9,  9,
                            9,  8,  8,  9,  9,  9,  9,  9,  9,  8,  8,  8,  8,  8,  8,  8,  8,  9,  9,  8,  8,  8,  7,  7,
                            7,  7,  7,  7,  7,  8,  8,  8,  8,  7,  7,  7,  7,  7,  7,  7,  7, 11, 12, 11, 11, 11, 10, 10, 10};


int main() {
    int i;
    int color_sensor_value;
    float y;
    float color_sensor_filtered_value;
    for(i=0; i < 121; i++){
        // printf("color[%d]=%f\n", i, color[i]);
        color_sensor_value = color[i];
        if(i==0) for(i=0; i < N; i++) color_N[i] = color_sensor_value;
	    filtering(color_sensor_value, &color_sensor_filtered_value);
        printf("i = %d\tf = %d\ty = %f\n", i, color_sensor_value, color_sensor_filtered_value);
    }
    return 0;
}

//フィルタリング
void filtering(int sensor_value, float *filtered_value) {
    int i;
    // float y = 0.0;

    for(i=0; i < N; i++) color_N[N-1-i] = color_N[N-2-i];
    color_N[0] = (float)sensor_value;
    // for(i=0; i < N; i++) printf("[%d]:%f\t", i, color_N[i]);
    // printf("\n");
    *filtered_value = 0.0;
    for(i=0; i < N; i++) {
        printf("color_N[%d]=%f, hw[%d]=%f\n", i, color_N[i], i, hw[i]);
        *filtered_value += color_N[i] * hw[i];
    }
    // printf("y = %f\n", *filtered_value);
	
}

//センサー正規化
void normalization(float sensor_value, float *normalized_value)
{
	*normalized_value = sensor_value;
}
