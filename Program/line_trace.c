#include "line_trace.h"

//カラー値よりTurnの値を決定する
void line_trace(int color_value, int *turn)
{
	if(color_value < BLACK) *turn = 20;
	else *turn = -20;
}
