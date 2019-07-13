#include "line_trace.h"

//カラー値よりTurnの値を決定する
void line_trace(float color_value, int *turn)
{
	//ON/OFF制御
/*
	if(color_value < BLACK) *turn = 20;
	else *turn = -20;
*/
	*turn = 0;
}
