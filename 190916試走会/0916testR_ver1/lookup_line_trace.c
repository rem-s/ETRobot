#include "lookup_line_trace.h"
#include "ev3api.h"

float e2 = 0;
float p2 = 0;
float i2 = 0;
float d2 = 0;

extern int DESIRED_VALUE2;

void lookup_line_trace(float color_value, int *turn)
{
	//変数の宣言
	static float integral2 = 0, differential2 = 0, e_last2 = 0;

	//偏差eを更新
	e_last2 = e2;
	//偏差e ＝（目標値）ー（現在時間ｔの取得値）
	e2 = DESIRED_VALUE2 - color_value;
	//積分値
	integral2 += (e2 + e_last2) / 2.0 * DELTA_T2;
	//微分値
	differential2 = (e2 - e_last2) / DELTA_T2;
	
	p2 = KP2 * e2;
	i2 = KI2 * integral2;
	d2 = KD2 * differential2;

	// if(p2 + i2 + d2 > 100) {
		// *turn = 100;
	// }
	// else if(p2 + i2 + d2 < -100) {
		// *turn = -100;
	// }
	// else {
		// *turn = p2 + i2 + d2;
	// }
	
	if(p2 > 100) {
		*turn = 100;
	}
	else if(p2 < -100) {
		*turn = -100;
	}
	else {
		*turn = p2;
	}
}