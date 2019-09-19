#ifndef __LINE_TRACE_HEADER__
#define __LINE_TRACE_HEADER__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef TOPPERS_MACRO_ONLY

//マクロ
#define DESIRED_VALUE 30

// #define KU 0.65f
#define KU 0.70f
#define PU 0.6f
#define TI 0.5*PU
#define TD 0.125*PU
#define A 1.0

#define KP 0.60f*KU*A
#define KI KP/TI
#define KD KP*TD
#define DELTA_T 0.004f

//通常の関数
void line_trace(float color_value, int *turn);

extern float e;
extern float p;
extern float i;
extern float d;

#endif /* TOPPERS_MACRO_ONLY */

#ifdef __cplusplus
}
#endif

#endif