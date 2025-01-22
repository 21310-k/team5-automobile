/****************************************************************************************************************/
/*	FILE        : itask_control.c																				*/
/*	DATE        :Tue, Jan 17, 2012																				*/
/*	DESCRIPTION :AGVの動作状態管理割り込みタスク																*/
/*	CPU TYPE    :H8/3052F																						*/
/****************************************************************************************************************/
#include	"machine.h"									/* 組込み関数の定義										*/
#include	"reg3052.h"									/* H8/3052Fの内蔵モジュール定義							*/
#include	"agvdef.h"									/* 無人搬送車のデバイス定義								*/
#include	"agvvars.h"									/* 無人搬送車の共通変数を定義							*/
#include	"def_monitor_printf.h"						/* 組込みprintfの定義									*/

/****************************************************************************************************************/
/*	搬送車動作状態監視・ハンドル制御タスク itask_control														*/
/****************************************************************************************************************/
int agv_state(void);
unsigned char cal_sensor_position(void);
void handle_control(unsigned char sens_position);

#pragma	interrupt itask_control
void	itask_control(void)
{
	unsigned char sensor_position;
	agv_state();

	switch(AGV_STATE){
		
		case AGV_BOOT:{
			if((A & 0x03) == 0x01){
				AGV_STATE = AGV_READY;
			}
			break;
		}
		
		case AGV_READY:{
			if(((A & 0x82) == 0x02) && (bios_sensor_input() != 0)){
				AGV_STATE = AGV_RUN_WAIT;
				MOTOR_STATE = MOTOR_ACC;
				TSTR|=0x02;
			}
			else{
				AGV_STATE=AGV_READY;
			}
			break;
		}
		
		case AGV_RUN_WAIT:{
			if(MOTOR_STATE != MOTOR_STOP){
				AGV_STATE = AGV_RUN;
			}
			break;
		}
		
		case AGV_RUN:{
			if(SENS_ERROR == 0x01) {
				AGV_STATE = AGV_RUN_ALM;
				MOTOR_STATE = MOTOR_BREAK;
			}
			if((A & 0x02) == 0x00){
				AGV_STATE = AGV_STOP_WAIT;
				MOTOR_STATE = MOTOR_BREAK;
			}
			sensor_position = cal_sensor_position();
			handle_control(sensor_position);
			break;
		}
		
		case AGV_RUN_ALM:{
			if(((A & 0x02) == 0x00)&&(MOTOR_STATE==MOTOR_STOP)){
				AGV_STATE = AGV_READY;
				SENS_ERROR=0x00;
			}
			break;
		}
		
		case AGV_STOP_WAIT:{
			if ((((A & 0x02) == 0x02)&&(MOTOR_STATE==MOTOR_BREAK))||(SENS_ERROR == 0x01)){
				AGV_STATE = AGV_RUN_ALM;
			}
			if(MOTOR_STATE == MOTOR_STOP){
				AGV_STATE = AGV_READY;
			}
			break;
		}
	}

/*
    // AGV_STATEがAGV_BOOTで、Aのbit0が1かつbit1が0の場合、AGV_STATEをAGV_READYに遷移
    if (AGV_STATE == AGV_BOOT && (A & 0x01) && !(A & 0x02)) {
        AGV_STATE = AGV_READY;
    }
    // AGV_STATEがAGV_READYで、Aのbit1が1、bit7が0、SENS_DATAが0x00でない場合、AGV_STATEをAGV_RUN_WAITに遷移
    if (AGV_STATE == AGV_READY && (A & 0x02) && !(A & 0x80) && bios_sensor_input() != 0x00) {
        AGV_STATE = AGV_RUN_WAIT;
		MOTOR_STATE = MOTOR_ACC;
		TSTR|=0x02;
    }
    // AGV_STATEがAGV_RUN_WAITで、MOTOR_STATEがMOTOR_STOPでない場合、AGV_STATEをAGV_RUNに遷移
    if (AGV_STATE == AGV_RUN_WAIT && MOTOR_STATE != MOTOR_STOP) {
        AGV_STATE = AGV_RUN;
		cal_sensor_position();
		handle_control(sensor_position);
    }
    // AGV_STATEがAGV_RUNで、Aのbit1が0の場合、AGV_STATEをAGV_STOP_WAITに遷移
    if (AGV_STATE == AGV_RUN && !(A & 0x02)) {
        AGV_STATE = AGV_STOP_WAIT;
		MOTOR_STATE = MOTOR_BREAK;
    }
    // AGV_STATEがAGV_STOP_WAITで、Aのbit1が1か、SENS_ERRORが0x01の場合、AGV_STATEをAGV_RUN_ALMに遷移
    if (((AGV_STATE == AGV_STOP_WAIT) && (A & 0x02)) ||( (AGV_STATE == AGV_STOP_WAIT) && (SENS_ERROR == 0x01))){
        AGV_STATE = AGV_RUN_ALM;
		MOTOR_STATE = MOTOR_BREAK;
    }
	 // AGV_STATEがAGV_RUNで、SENS_ERRORが0x01の場合、AGV_STATEをAGV_RUN_ALMに遷移
    if ((AGV_STATE == AGV_RUN) && (SENS_ERROR == 0x01)) {
        AGV_STATE = AGV_RUN_ALM;
		MOTOR_STATE = MOTOR_BREAK;
    }
    // AGV_STATEがAGV_RUN_ALMで、Aのbit1が0、MOTOR_STATEがMOTOR_STOPの場合、AGV_STATEをAGV_READYに遷移
    if ((AGV_STATE == AGV_RUN_ALM) && !(A & 0x02) && (MOTOR_STATE == MOTOR_STOP)) {
        AGV_STATE = AGV_READY;
		SENS_ERROR=0x00;
    }
    // AGV_STATEがAGV_STOP_WAITで、MOTOR_STATEがMOTOR_STOPの場合、AGV_STATEをAGV_READYに遷移
    if (AGV_STATE == AGV_STOP_WAIT && MOTOR_STATE == MOTOR_STOP) {
        AGV_STATE = AGV_READY;
    }
*/	
	TSR2&=~0x01;
}
/****************************************************************************************************************/
/*	AGVステート管理モジュール agv_state																			*/
/****************************************************************************************************************/
int agv_state(void)
{
	A= 0x00;   // 初期値を0x00に設定
	
    // SW_DATAの0bit目が1の場合、Aの0bit目を1にする
    // SW_DATAの0bit目が0の場合、Aの0bit目を0にする
    if (SW_DATA & 0x01) {
        A |= 0x01;   // Aの0bit目を1にする
    }
    else {
        A &= ~0x01;  // Aの0bit目を0にする
    }
	// SW_DATAの1bit目が1の場合、Aの1bit目を1にする
    // SW_DATAの1bit目が0の場合、Aの1bit目を0にする
    if (SW_DATA & 0x02) {
        A |= 0x02;   // Aの1bit目を1にする
    }
    else {
        A &= ~0x02;  // Aの1bit目を0にする
    }
    if (MOTOR_STATE == MOTOR_ACC) {   // 加速状態
        A |= 0x08;   // Aの3bit目を1にする
    }
    else if (MOTOR_STATE == MOTOR_CONST) {   // 定速状態
        A &= ~0x08;  // Aの3bit目を0にする
        A |= 0x10;   // Aの4bit目を1にする
    }
    else if (MOTOR_STATE == MOTOR_BREAK) {   // 減速状態
        A &= ~0x08;  // Aの3bit目を0にする
        A &= ~0x10;  // Aの4bit目を0にする
        A |= 0x20;   // Aの5bit目を1にする
    }
    else if (MOTOR_STATE == MOTOR_STOP) {   // 停止状態
        A &= ~0x20;  // Aの5bit目を0にする    
    }
    // AGV_STATEの値に応じてAのbit7を変更
    if (AGV_STATE == AGV_RUN_ALM) {
        A |= 0x80;   // Aのbit7を1にする
    }
    else if (AGV_STATE == AGV_READY) {
        A &= ~0x80;  // Aのbit7を0にする
    }
}
/****************************************************************************************************************/
/*	センサ位置計算モジュール cal_sensor_position																*/
/****************************************************************************************************************/
unsigned char cal_sensor_position(void)
{
    int binary[8];                /* 各ビットの値を格納する配列 */
    int weight[8];                /* 各ビットの重みを格納する配列 */
    int sum_weighted_bits = 0;    /* Σ(weight(i) * bit(i)) */
    int count = 0;                /* Σ(bit(i)) */
    unsigned char position = 0;       /* 計算結果のPosition */
    int i = 0;

    /* 2進数変換と weight(i) 計算 */
    for (i = 0; i < 8; i++) {
        binary[i] = (SENS_DATA >> i) & 1;     /* bit(i)の計算 */
        weight[i] = 16 + 32 * i;              /* weight(i)の計算 */

        /* Σ(weight(i) * bit(i)) と Σ(bit(i)) の計算 */
        sum_weighted_bits += weight[i] * binary[i];
        count += binary[i];
    }
    /* Position計算 */
        if (count != 0) {
        position = sum_weighted_bits / count;
    }
    return (position);
}
/****************************************************************************************************************/
/*	ハンドル制御モジュール handle_control																		*/
/****************************************************************************************************************/
void handle_control(unsigned char sens_position)
{
    unsigned char control = 0;
    int d;                              /*ずれ量*/
    int diff;                           /*計算値*/
    unsigned char ad;                   /* 現在のハンドル切れ角 */
    unsigned char correction;           /* 修正値 */
	d = sens_position - SENS_CENTER;          /*ずれ量*/
    ad = bios_ad_input();                     /* 現在のハンドル切れ角 */
    diff = d * 17 / 32;
    control = diff + ad;
    bios_da_output(control);
}