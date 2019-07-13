#ifdef __cplusplus
extern "C" {
#endif

#ifndef TOPPERS_MACRO_ONLY

//マクロ
#define BLACK 20

//通常の関数
void filtering(int sensor_value, float *filtered_value);
void normalization(float sensor_value, float *normalized_value);

#endif /* TOPPERS_MACRO_ONLY */

#ifdef __cplusplus
}
#endif
