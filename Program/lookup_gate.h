#ifndef __LOOKUP_GATE_HEADER__
#define __LOOKUP_GATE_HEADER__

#include "ev3api.h"
#include "app.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef TOPPERS_MACRO_ONLY

//通常の関数
void lookup_gate(motor_port_t left_motor, motor_port_t right_motor);
//void lookup_gate(int left_motor, int right_motor);

extern volatile int sonor;
extern volatile int sec_flag;

#endif /* TOPPERS_MACRO_ONLY */

#ifdef __cplusplus
}
#endif

#endif