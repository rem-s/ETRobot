#include "measure_position.h"
#include <math.h>

float pre_right_radian;
float pre_left_radian;

void measure_position(float right_motor_count, float left_motor_count, float *theta, float *x_position, float *y_position, float *speed)
{
	//右モーター速度
	float right_radian = (right_motor_count * M_PI) / 180.0;
	float right_distance  = (WHEEL_DIAMETER / 2.0) * (right_radian - pre_right_radian);
	float right_speed = right_distance / INTERVAL;
	
	//左モーター速度
	float left_radian = (left_motor_count * M_PI) / 180.0;
	float left_distance  = (WHEEL_DIAMETER / 2.0) * (left_radian - pre_left_radian);
	float left_speed = left_distance / INTERVAL;
	
	//車体速度
	*speed = (right_speed + left_speed) / 2.0;
	
	//車体回転速度
	float omega = (right_speed - left_speed) / WHEEL_SPACE;
	
	//車体角度
	*theta += omega * INTERVAL;
	
	//車体位置
	*x_position += (*speed * cos((double)*theta) * INTERVAL);
	*y_position += (*speed * sin((double)*theta) * INTERVAL);
	
	//モーター角位置
	pre_right_radian  = right_radian;
	pre_left_radian = left_radian;
}