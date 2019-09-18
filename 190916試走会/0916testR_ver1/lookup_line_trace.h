#ifndef __LINE_TRACE_HEADER__
#define __LINE_TRACE_HEADER__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef TOPPERS_MACRO_ONLY

//マクロ
// #define DESIRED_VALUE2 13

#define KU2 0.650
#define PU2 0.60
#define TI2 0.50*PU2
#define TD2 0.1250*PU2
// #define A2 1.0

// #define KP2 0.60f*KU2
// #define KP2 0.800
#define KP2 0.800
#define KI2 KP2/TI2
#define KD2 KP2*TD2
#define DELTA_T2 0.0040

//通常の関数
void lookup_line_trace(float color_value, int *turn);

extern float e2;
extern float p2;
extern float i2;
extern float d2;

#endif /* TOPPERS_MACRO_ONLY */

#ifdef __cplusplus
}
#endif

#endif