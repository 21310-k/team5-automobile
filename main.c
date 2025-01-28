/****************************************************************************************************************/
/*	FILE        :main.c																							*/
/*	DATE        :Tue, Jan 17, 2012																				*/
/*	DESCRIPTION :AGV Main Program																				*/
/*	CPU TYPE    :H8/3052F																						*/
/****************************************************************************************************************/
#include	"stdio.h"
#define	MAIN										/* マクロMAINの定義											*/

#include	"machine.h"								/* 組込み関数の定義											*/
#include	"reg3052.h"								/* H8/3052Fの内蔵モジュール定義								*/
#include	"agvdef.h"								/* 無人搬送車のデバイス定義									*/
#include	"agvvars.h"								/* 無人搬送車の共通変数を定義								*/
#include	"def_monitor_printf.h"

/****************************************************************************************************************/
/*	AGVデバイス初期化モジュール agv_init																		*/
/****************************************************************************************************************/
void agv_init(void) {
    //本番
    //ハードウェアの初期化
    MOTOR_STATE = MOTOR_STOP;        //モータの停止
    bios_led_output(0x00);         //LEDの消灯
    bios_da_output(HANDLE_CENTER);  //ハンドルを中心に移動する

	SYSCR &= ~0x08;
	
    //ITU0の初期化
    TCR0 = 0xa0;
    TIOR0 = 0x8b;
    GRA0 = 1249;

    //ITU1の初期化
    TCR1 = 0xa6;
    TIER1 |= 0xf9;

    //ITU2の初期化
    TCR2 = 0xa6;
    TIER2 |= 0xf9;
    GRA2 = 99;

    //ITU3の初期化
    TCR3 = 0xa6;
    TIER3 |= 0xf9;
    GRA3 = 9;

    IPRA |= 0x02;  //ITU1の割込みをプライオリティレベル1に設定


    TSTR |= 0x0d;            //ITU0・2・3をスタートする
    and_ccr(~0x80);        //割込み許可


    //テスト用
    //ハードウェアの初期化
    MOTOR_STATE = MOTOR_STOP;        //モータの停止
    bios_led_output(0x00);         //LEDの消灯
    bios_da_output(HANDLE_CENTER);  //ハンドルを中心に移動する
    AGV_STATE = AGV_BOOT;
	SYSCR &= ~0x80;

    //ITU0の初期化
    TCR0 = 0xa0;
    TIOR0 = 0x8b;
    GRA0 = 1249;

    //ITU1の初期化
    TCR1 = 0xa6;
    TIER1 |= 0xf9;
    GRA1 = 5000;

    //ITU2の初期化
    TCR2 = 0xa6;
    TIER2 |= 0xf9;
    GRA2 = 5000;

    //ITU3の初期化
    TCR3 = 0xa6;
    TIER3 |= 0xf9;
    GRA3 = 5000;

    TSTR |= 0x0f;            //ITU0・1・2・3をスタートする
    and_ccr(~0x80);        //割込み許可
}

//#pragma interrupt LED_interrupt
void LED_interrupt(void) {
    static unsigned char LED_FLAG = 0;// 静的変数LED_FLAGを定義
    if (LED_FLAG) {
        //bios_led_output(0x00);// LED_FLAGが0ならばLED全点灯
    }
    else {
        //bios_led_output(0xff);// LFLAGが0でなければLED全消灯
    }
    LED_FLAG = ~LED_FLAG;// LED_FLAGを反転する
    TSR1 &= ~0x01;// ITU1の割込みフラグのクリア
}


/****************************************************************************************************************/
/*	main関数																									*/
/****************************************************************************************************************/
int main(void)
{	
	unsigned int i;
	agv_init();
	
	//走行状態に設定	
	AGV_STATE = AGV_RUN;
	

	//条件
	SW_DATA = 0x00;// SENS_DATA = 0x01; MOTOR_STATE = MOTOR_CONST;
	

	
	while(-1){
			agv_state();
			bios_led_output(AGV_STATE);	
	}
	//bios_step_output用テスト
	
	/*unsigned int i = 0;
	
	while(-1){
		bios_step_output();
	}
	return 0;*/
}

/****************************************************************************************************************/
/*	beepモジュール beep																							*/
/****************************************************************************************************************/
void beep(void)
{

}
