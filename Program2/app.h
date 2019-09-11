#ifndef __APP_HEADER__
#define __APP_HEADER__

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
// #define MAIN_PRIORITY   TMIN_APP_TPRI + 1  /* メインタスクの優先度 */
// #define CYC1_PRIORITY 	TMIN_APP_TPRI + 3 /* 周期タスク１の優先度 */
// #define CYC2_PRIORITY 	TMIN_APP_TPRI + 2 /* 周期タスク１の優先度 */
// #define CYC3_PRIORITY 	TMIN_APP_TPRI + 2 /* 周期タスク１の優先度 */
// #define CYC4_PRIORITY 	TMIN_APP_TPRI + 2
// #define CYC5_PRIORITY 	TMIN_APP_TPRI + 3 // 周期タスク4(周期タスク1と同じ)

#define MAIN_PRIORITY   TMIN_APP_TPRI + 1  /* メインタスクの優先度 */
#define CYC1_PRIORITY 	TMIN_APP_TPRI + 4 /* 周期タスク１の優先度 */
#define CYC2_PRIORITY 	TMIN_APP_TPRI + 2 /* 周期タスク１の優先度 */
#define CYC3_PRIORITY 	TMIN_APP_TPRI + 2 /* 周期タスク１の優先度 */
#define CYC4_PRIORITY 	TMIN_APP_TPRI + 3
#define CYC5_PRIORITY 	TMIN_APP_TPRI + 0 // 周期タスク4(周期タスク1と同じ)

/*
 *  ターゲットに依存する可能性のある定数の定義
 */
#ifndef STACK_SIZE
#define STACK_SIZE      4096        /* タスクのスタックサイズ */
#endif /* STACK_SIZE */

#define FIRST_TAIL_POSITION 84
#define START_TAIL_POSITION 97
#define ISRUN_TAIL_POSITION 5

#define LOOKUP_TAIL_POSITION 75
#define WAKEUP_TAIL_POSITION 86

//調整が必要
#define KFORWARD_START 0.020
#define KGYRO_OFFSET 0.025
#define TARGET_FORWARD 30
// #define LOOKUP_FORWARD 30
// #define TARGET_GYRO_OFFSET 2

typedef struct tail_type{
	int diff;
	int now_angle;
	int target_angle;
	
	float speed;
}tail_type;

extern FILE *bt;

/*
 *  関数のプロトタイプ宣言
 */
#ifndef TOPPERS_MACRO_ONLY

//通常の関数
void initialize();
void delete_system();
void backlash_cancel(signed char left_motor_pwm, signed char right_motor_pwm, int *right_motor_angle, int *left_motor_angle);

//タスク用の関数
extern void main_task(intptr_t exinf);
extern void cyc_task1(intptr_t exinf);
extern void cyc_handler1(intptr_t exinf);
extern void cyc_task2(intptr_t exinf);
extern void cyc_handler2(intptr_t exinf);
extern void cyc_task3(intptr_t exinf);
extern void cyc_handler3(intptr_t exinf);
extern void cyc_task4(intptr_t exinf);
extern void cyc_handler4(intptr_t exinf);
extern void cyc_task5(intptr_t exinf);
extern void cyc_handler5(intptr_t exinf);

#endif /* TOPPERS_MACRO_ONLY */

#ifdef __cplusplus
}
#endif

#endif
