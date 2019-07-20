#ifdef __cplusplus
extern "C" {
#endif

#ifndef TOPPERS_MACRO_ONLY

//マクロ
#define BLACK 20

#define KP 0.2
#define KI 0.4
#define KD 0.4
#define DELTA_T 0.004

//通常の関数
void line_trace(float color_value, int *turn);

#endif /* TOPPERS_MACRO_ONLY */

#ifdef __cplusplus
}
#endif
