#ifndef __MEASURE_POSITION_HEADER__
#define __MEASURE_POSITION_HEADER__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef TOPPERS_MACRO_ONLY

//車体情報マクロ
#define WHEEL_DIAMETER 10.0
#define WHEEL_SPACE   18.0
#define INTERVAL 0.004

//通常の関数
void measure_position(float right_motor_count, float left_motor_count, float *theta, float *x_position, float *y_position, float *speed);

#endif /* TOPPERS_MACRO_ONLY */

#ifdef __cplusplus
}
#endif

#endif