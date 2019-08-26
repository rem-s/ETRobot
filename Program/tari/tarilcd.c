/*    *                 *   **             *
 *  *****                    *             *
 *    *   ****   ****  **    *    ****  ****     ****
 *    *       * *       *    *   *     *        *
 * 	  *    **** *       *    *   *     *        *
 * 	  *   *   * *       *    *   *     *     ** *
 * 	  *** ***** *       *    **   ****  **** **  ****
 * 
 */
#include "tarilcd.h"
#include "ev3api.h"
	//フォントサイズの格納
	static int32_t h_fm,h_fs,w_fm,w_fs;

	//カーソルの現在位置
	static int h_px;
	static int line;
	static int cursor;

	//初期化済判定
	static int flag=0;


int init_lcd(int size)
{
	char* tstr;
	ev3_font_get_size(EV3_FONT_MEDIUM,&w_fm,&h_fm);
	ev3_font_get_size(EV3_FONT_SMALL,&w_fs,&h_fs);
	sprintf(tstr,"MEDIUM: %d",(int)w_fm);
	ev3_lcd_draw_string (tstr,0,h_px);
	sprintf(tstr,"%d",(int)h_fm);
	ev3_lcd_draw_string (tstr,0,h_px);
	sprintf(tstr,"%d",(int)w_fs);
	ev3_lcd_draw_string (tstr,0,h_px);
	sprintf(tstr,"%d",(int)h_fs);
	ev3_lcd_draw_string (tstr,0,h_px);
	ev3_lcd_draw_string (tstr,0,h_px);
	ev3_lcd_draw_string (tstr,0,h_px);
	flag++;
	ev3_speaker_set_volume(10);
	ev3_speaker_play_tone(NOTE_C4,100);
	return 0;
}


/*  tarilcd
 *  LCDに何か表示する。
 *  第1引数: モード指定 第2引数: 1行分の文字列
 * 
 *  モードについて
 *  1:第2引数で受け取った文字列を中フォントで表示する。
 *  2:第2引数で受け取った文字列を小フォントで表示する。
 *  3:EV3のステータスを表示する。
 * 
 */

int tarilcd_disp(int a,const char* str)
{
	if(flag==0)
	{
		ev3_lcd_set_font(EV3_FONT_MEDIUM);
		ev3_lcd_draw_string ("PLEASE INIT LCD!!",0,0);
		return -1;
	}
	else
	{
		switch(a)
		{
			case 1:
				ev3_lcd_set_font(EV3_FONT_MEDIUM);
				ev3_lcd_draw_string (str,0,h_px);
				h_px += h_fm;
				line++;
				//cursor += strlen(str);
				break;
			case 2:
				ev3_lcd_set_font(EV3_FONT_SMALL);
				ev3_lcd_draw_string (str,0,h_px);
				h_px += h_fs;
				line++;
				//cursor += strlen(str);
				break;
			default:
				break;
		}
	}
	
	return 0;
}
