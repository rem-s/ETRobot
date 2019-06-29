#include "app.h"

void *__dso_handle = 0;

//システムの初期化
void initialize()
{
	
}

//システム生成
void create_system()
{
	
}

//システムデリート
void delete_system()
{
	
}

//サブタスク
void sub_task(intptr_t exinf)
{
	
	//低音を出力する
	ev3_speaker_play_tone(NOTE_C4, SOUND_MANUAL_STOP);
	
	//周期タスクの動作開始
	ev3_sta_cyc(CYC_HANDLER2);
	
	//スリープタスク
	slp_tsk();
	
	//低音の出力を止める
	ev3_speaker_stop();
	
	//サブタスクの終了
	ext_tsk();
}

//周期タスク関数
void cyc_task1(intptr_t exinf)
{
	static unsigned short time_counter1 = 0;
	
	//１秒経過後メインタスクの起動
	if(time_counter1++ == 10) wup_tsk(MAIN_TASK);
	
	//周期タスクの終了
	ext_tsk();
}

//周期タスク関数
void cyc_task2(intptr_t exinf)
{
	static unsigned short time_counter2 = 0;
	
	//１秒経過後メインタスクの起動
	if(time_counter2++ == 10) wup_tsk(SUB_TASK);
	
	//周期タスクの終了
	ext_tsk();
}

//周期タスク1(100msec周期)
void cyc_handler1(intptr_t exinf)
{
	act_tsk(CYC_TASK1);
}

//周期タスク2(200msec周期)
void cyc_handler2(intptr_t exinf)
{
	act_tsk(CYC_TASK2);
}

//メインタスク
void main_task(intptr_t unused)
{
	//システムの生成
	create_system();

	//システムの初期化
	initialize();
	
	//高音鳴らす
	ev3_speaker_play_tone(NOTE_A4, SOUND_MANUAL_STOP);
	
	//周期ハンドラの起動
	ev3_sta_cyc(CYC_HANDLER1);
	
	//メインタスクのスリープ
	slp_tsk();
	
	//スピーカーの音を止める
	ev3_speaker_stop();
	
	//サブタスクを起動
	act_tsk(SUB_TASK);
	
	//システムの削除
	delete_system();

	//タスクの終了
	ext_tsk();
}
