/****************************************************************************************************************/
/*  FILE        : itask_control.c                                                                               */
/*  DATE        :Tue, Jan 17, 2012                                                                              */
/*  DESCRIPTION :AGVの動作状態管理割り込みタスク                                                              */
/*  CPU TYPE    :H8/3052F                                                                                       */
/****************************************************************************************************************/
#include    "machine.h"                                 /* 組込み関数の定義                                     */
#include    "reg3052.h"                                 /* H8/3052Fの内蔵モジュール定義                           */
#include    "agvdef.h"                                  /* 無人搬送車のデバイス定義                             */
#include    "agvvars.h"                                 /* 無人搬送車の共通変数を定義                            */
#include    "def_monitor_printf.h"                      /* 組込みprintfの定義                                 */




/****************************************************************************************************************/
/*  搬送車動作状態監視・ハンドル制御タスク itask_control                                                       */
/****************************************************************************************************************/
//関数の仮宣言
void agv_state(void);
unsigned char cal_sensor_position(void);
void handle_control(unsigned char spos);

//変数の定義
unsigned char calculated_pos;
int i;
//int counter_state = 0;
unsigned char frag_state;
unsigned char frag_motor_state;
#pragma interrupt itask_control
void    itask_control(void)
{
	//優先度1の割り込みを許可
	and_ccr(~0x40);
	
    // ①ステートのフェッチと変更 (agv_state())
    // ②センサ位置の計算(cal_sensor_position())
    // ③ハンドル制御(handle_control())
    //以上3つをitask_controlが割り込まれた際に実行する。
	agv_state();
	calculated_pos = cal_sensor_position();
	handle_control(calculated_pos);
	
	//ITU2の割り込みフラグをクリア
	TSR2 &= ~0x01;
	
}

/****************************************************************************************************************/
/*  AGVステート管理モジュール agv_state                                                                            */
/****************************************************************************************************************/
void agv_state(void)
{
	frag_state = AGV_STATE;
	frag_motor_state = MOTOR_STATE;
	//センサ検出とはSENS_DATAが0x00の値以外であること
    switch (AGV_STATE) {
    case AGV_BOOT:
        if (SENS_DATA == 0x00 || SW_DATA == 0x01) {
            AGV_STATE = AGV_BOOT_ALM;
        }
        if (SENS_DATA != 0x00 && SW_DATA == 0x00) {
            AGV_STATE = AGV_READY;
        }
        break;

    case AGV_BOOT_ALM:
    
        if (SENS_DATA == 0x00 || SW_DATA == 0x01) {
            AGV_STATE = AGV_BOOT_ALM;
        }
        if (SENS_DATA != 0x00 && SW_DATA == 0x00) {
            AGV_STATE = AGV_READY;
        }
        break;

    case AGV_READY:
        if (SENS_DATA != 0x00 && SW_DATA == 0x00) {
            AGV_STATE = AGV_READY;
        }
        if (SENS_DATA == 0x00) {
            AGV_STATE = AGV_READY_ALM;
        }
        if (SW_DATA == 0x01) {
            AGV_STATE = AGV_RUN_WAIT;
			GRA1 = 999;
            TSTR |= 0x02; 
        }
        break;

    case AGV_READY_ALM:
        if (SENS_DATA == 0x00 && SW_DATA == 0x00) {
            AGV_STATE = AGV_READY_ALM;
        }
        if (SENS_DATA != 0x00 && SW_DATA == 0x00) {
            AGV_STATE = AGV_READY;
        }
        break;

    case AGV_RUN_WAIT:
        if (MOTOR_STATE != MOTOR_STOP) {
            AGV_STATE = AGV_RUN;
        }
        break;

    case AGV_RUN:

        if (SW_DATA == 0x00) {
            AGV_STATE = AGV_STOP_WAIT;
        }
        if (SW_DATA == 0x01 && SENS_DATA == 0x00) {
            AGV_STATE = AGV_RUN_ALM;
        }
        if (SW_DATA == 0x01 && MOTOR_STATE != MOTOR_STOP && SENS_DATA!=0x00) {
            AGV_STATE = AGV_RUN;
        }
        break;

    case AGV_STOP_WAIT:
        if (SENS_DATA == 0x00) {
            AGV_STATE = AGV_RUN_ALM;
        }
        if (MOTOR_STATE == MOTOR_STOP) {
            AGV_STATE = AGV_READY;
        }
        break;

    case AGV_RUN_ALM:
        if (SENS_DATA == 0x00 && MOTOR_STATE == MOTOR_STOP) {
            AGV_STATE = AGV_RUN_ALM;
        }
        if (SENS_DATA != 0x00 && SW_DATA == 0x01 && MOTOR_STATE != MOTOR_STOP) {
            AGV_STATE = AGV_RUN;
			from_run_alm = 1;
        }
        if (SENS_DATA != 0x00 && SW_DATA == 0x01 && MOTOR_STATE == MOTOR_STOP) {
            AGV_STATE = AGV_READY;
        }
        break;

    default:

        break;

    }
	if ((frag_state != AGV_STATE) || (frag_motor_state != MOTOR_STATE)){
		printf("AGV_STATE:%d , ", AGV_STATE);
		printf("MOTOR_STATE:%d\n" , MOTOR_STATE);
		
		}
	
}       

/****************************************************************************************************************/
//  センサ位置計算モジュール cal_sensor_position
// 引数：なし
// 戻り値：センサの位置情報                                                             */
/****************************************************************************************************************/

unsigned char cal_sensor_position(void)
{
    // weight(i)=16+32*iの計算式に則り、重みを設定
    unsigned char weight[8] = {16, 48, 80, 112, 144, 176, 208, 240};
    // センサ入力を取得
    unsigned char sens_input = SENS_DATA;
    // 必要な変数を定義
    unsigned int molecule = 0;
    unsigned int denominator = 0;
    unsigned char position = 0;

    // 各桁について見ていく
    for (i = 0; i < 8; i++) {
        // sens_inputのi桁目が1の場合
        if ((sens_input & (1 << i))!=0) {
            // 分子に重みを加算
            molecule += weight[i];
            // 分母を加算
            denominator ++;
        }
    }

    //  positionを算出
	if(denominator == 0){return 0;}
    position =(unsigned char)( molecule / denominator);
	//printf("postion = %d\n",position);
    // positionを返す
    return position;
    
}

/****************************************************************************************************************/
//  ハンドル制御モジュール handle_control
// 引数：cal_sensor_pos()で算出した位置情報
// 戻り値：なし                                                                       */
/****************************************************************************************************************/
//sposはcal_sensor_pos()で算出した位置情報
void handle_control(unsigned char spos)
{
    // ずれ量diffを求める
    int diff = spos - HANDLE_CENTER;
    // 現在のハンドル切れ角adを取得
    unsigned char ad = bios_ad_input();
    // 変数定義
    int da_output = 0;

    // dに17/32を乗じたものを現在のハンドル切れ角adに加えたものが制御値に求められる。
	da_output = diff * 17 / 32;
	da_output = da_output + (int)(ad);
	
	//unsgined charがたに収めるため、範囲を制限
    if (da_output < 0){da_output = 0;}
	if (da_output > 255){da_output = 255;}
	
    
	//走行状態の時のみ出力
	if (AGV_STATE == AGV_RUN){
		
		//制御値を出力
		bios_da_output(da_output);
	}
	
    
}
