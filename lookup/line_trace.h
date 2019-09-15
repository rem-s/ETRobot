#ifndef __LINE_TRACE_HEADER__
#define __LINE_TRACE_HEADER__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef TOPPERS_MACRO_ONLY

//マクロ
// #define DESIRED_VALUE 30

#define KU 0.650 //KPの限界値
// #define PU 0.6f
#define PU 0.60
#define TI 0.50*PU  //積分時間
#define TD 0.1250*PU  //微分時間
// #define A 1.0

// #define KP 0.60f*KU*A
// #define KI KP/TI*A
// #define KD KP*TD*A
// #define KP 0.60f*KU
// #define KP 0.420
#define KP 0.800
#define KI KP/TI
#define KD KP*TD
#define DELTA_T 0.0040

//通常の関数
void line_trace(float color_value, int *turn);

extern float e;
extern float p;
extern float i;
extern float d;
extern volatile float DESIRED_VALUE;


#endif /* TOPPERS_MACRO_ONLY */

#ifdef __cplusplus
}
#endif

#endif