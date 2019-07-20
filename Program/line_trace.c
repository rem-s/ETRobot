#include "line_trace.h"

//カラー値よりTurnの値を決定する
void line_trace(float color_value, int *turn)
{
	//ON/OFF制御
/*
	if(color_value < BLACK) *turn = 20;
	else *turn = -20;
*/
	//変数の宣言
	static float p, i, d = 0;
	static float e = 0, e_last = 0;
	static float integral = 0;
	static float differential = 0;

	//偏差eを更新
	e_last = e;
	//偏差e ＝（目標値）ー（現在時間ｔの取得値）
	e = BLACK - color_value;
	//積分値
	integral += e / 2.0 * DELTA_T;
	//微分値
	differential = (e - e_last) / DELTA_T;
	
	p = KP * e;
	i = KI * integral;
	d = KD * differential;

	*turn = p + i + d;
	//*turn = 0;
}
