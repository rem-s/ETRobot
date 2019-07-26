#include "line_trace.h"

float e = 0;
float p = 0;
float i = 0;
float d = 0;

//カラー値よりTurnの値を決定する
void line_trace(float color_value, int *turn)
{
	//ON/OFF制御
/*
	if(color_value < BLACK) *turn = 20;
	else *turn = -20;
*/
	//変数の宣言
	static float integral = 0, differential = 0, e_last = 0;
	//static float p = 0, i = 0, d = 0;

	//偏差eを更新
	e_last = e;
	//偏差e ＝（目標値）ー（現在時間ｔの取得値）
	e = DESIRED_VALUE - color_value;
	//積分値
	integral += (e + e_last) / 2.0 * DELTA_T;
	//微分値
	differential = (e - e_last) / DELTA_T;
	
	p = KP * e;
	i = KI * integral;
	d = KD * differential;

	if(p + i + d > 100) {
		*turn = 100;
	}
	else if(p + i + d < -100) {
		*turn = -100;
	}
	else {
		*turn = p + i + d;
	}
	//*turn = 0;
}