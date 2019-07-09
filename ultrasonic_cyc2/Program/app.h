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
#define MAIN_PRIORITY    TMIN_APP_TPRI + 1  /* メインタスクの優先度 */
#define SUB_PRIORITY    TMIN_APP_TPRI + 2  /* サブタスクの優先度 */
#define US_PRIORITY      TMIN_APP_TPRI + 3  /* 測距タスクの優先度 */
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

extern void main_task(intptr_t exinf);
void initialize();
extern void sub_task(intptr_t exinf);
extern void us_task(intptr_t exinf);
extern void timer_act_1sec(intptr_t exinf);

#endif /* TOPPERS_MACRO_ONLY */

#ifdef __cplusplus
}
#endif
