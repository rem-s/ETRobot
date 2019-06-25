#ifdef __cplusplus
extern "C"{
#endif;

#include "ev3api.h"

#define MAX_QUEUE 11
#define MAIN_PRIORITY TMIN_APP_TPRI + 1
#define SUB_PRIORITY TMIN_APP_TPRI + 2

#ifndef STACK_SIZE
#define STACK_SIZE 4096
#endif

#ifndef TOPPERS_MACRO_ONLY

extern void main_task(intptr_t exinf);
void initialize();
void create_system();
void delete_system();
void motor_change();
void state_change();
extern void sub_task(intptr_t exinf);
extern void timer_act_1sec(intptr_t exinf);

#endif

#ifdef __cplusplus
}
#endif
