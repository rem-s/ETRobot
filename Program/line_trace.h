#ifdef __cplusplus
extern "C" {
#endif

#ifndef TOPPERS_MACRO_ONLY

//マクロ
#define BLACK 20

#define DESIRED_VALUE 25

#define KU 0.6f
#define PU 1.0f
#define TI 0.5*PU
#define TD 0.125*PU
#define A 0.85

#define KP 0.60f*KU*A
#define KI KP/TI*A
#define KD KP*TD*A
#define DELTA_T 0.004f

//通常の関数
void line_trace(float color_value, int *turn);

extern float e;
extern float p;
extern float i;
extern float d;
//void getColorData();

#endif /* TOPPERS_MACRO_ONLY */

#ifdef __cplusplus
}
#endif
