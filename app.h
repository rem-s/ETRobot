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
#define SUB_PRIORITY    TMIN_APP_TPRI + 2  /* メインタスクの優先度 */
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
//main_tskなどの"extern"がついた関数は外部のファイル(app.cfg)から読み込むためつける
void initialize();
void create_system();
void delete_system();
void led_change();
void state_change();
extern void sub_task();
extern void timer_act_1sec(intptr_t exinf);
extern void wait_();

#endif /* TOPPERS_MACRO_ONLY */

#ifdef __cplusplus
}
#endif
