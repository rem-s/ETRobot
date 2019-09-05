#ifndef __SENSOR_PROCESS_HEADER__
#define __SENSOR_PROCESS_HEADER__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef TOPPERS_MACRO_ONLY

#define BLACK 20
#define N 11               // サンプリングの個数
#define MIN_COLOR_VAL  1   // フィルタ後のセンサデータの最小値
#define MAX_COLOR_VAL 60   // フィルタ後のセンサデータの最大値
#define MIN_NORM_VAL   0   // 正規化範囲の最小値
#define MAX_NORM_VAL  100   // 正規化範囲の最大値


//通常の関数
void filtering(int sensor_value, float *filtered_value);
void normalization(float sensor_value, float *normalized_value);

// 一番最初のカラーの値に対する処理のためのフラグ
static volatile int first_color_flag = 0;

#endif /* TOPPERS_MACRO_ONLY */

#ifdef __cplusplus
}
#endif

#endif