#ifdef __cplusplus
extern "C" {
#endif

#include "ev3api.h"

/*
 *  Queueの最大サイズをマクロで宣言
 */
#define MAX_QUEUE 11

/*
 *  各タスクの優先度の定義
 */
#define MAIN_PRIORITY   TMIN_APP_TPRI + 1  /* メインタスクの優先度 */
#define CYC1_PRIORITY 	TMIN_APP_TPRI + 3 /* 周期タスク１の優先度 */
#define CYC4_PRIORITY 	TMIN_APP_TPRI + 2 /* 周期タスク4の優先度 */


/*
 *  ターゲットに依存する可能性のある定数の定義
 */
#ifndef STACK_SIZE
#define STACK_SIZE      4096        /* タスクのスタックサイズ */
#endif /* STACK_SIZE */

/*
 *  関数のプロトタイプ宣言
 */
#ifndef TOPPERS_MACRO_ONLY

//通常の関数
void initialize();
void delete_system();
void bluetooth_w();
void backlash_cancel(signed char left_motor_pwm, signed char right_motor_pwm, int *right_motor_angle, int *left_motor_angle);

//タスク用の関数
extern void main_task(intptr_t exinf);
extern void cyc_task1(intptr_t exinf);
extern void cyc_handler1(intptr_t exinf);
extern void cyc_task2(intptr_t exinf);
extern void cyc_handler2(intptr_t exinf);
extern void cyc_task4(intptr_t exinf);
extern void cyc_handler4(intptr_t exinf);

#endif /* TOPPERS_MACRO_ONLY */

#ifdef __cplusplus
}
#endif
