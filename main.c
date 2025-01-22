/****************************************************************************************************************/
/*	FILE        :main.c																							*/
/*	DATE        :Tue, Jan 17, 2012																				*/
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
int count=0;

void agv_init(void)
{
	LED_PORT = 0x00;
	DA_PORT=0x80;
	STEP_PORT=0x00;

	IPRA = 0x02;//ITU1を最優先にする
	SYSCR &= 0xf7;

	TCR0 = 0xa0;/* TCR0の設定						*/
	TIOR0 = 0x8b;/* TIOR0の設定						*/
	GRA0 = 624;/* GRA0の設定						*/

	TCR1 = 0xa6;/* TCR1の設定						*/
	TIER1 = 0xf9;/* TIER1の設定						*/
	GRA1=99;

	TCR2 = 0xa6;/* TCR2の設定						*/
	TIER2 = 0xf9;/* TIER2の設定						*/
	GRA2 = 999;

	TCR3 = 0xa0;/* TCR3の設定						*/
	TIER3 = 0xf9;/* TIER3の設定						*/
	GRA3 = 12499;
	
	TCR4=0xa6;
	TIER4=0xf9;
	GRA4=9999;
	

	TSTR |= 0x0d;//ITU0/ITU2/ITU3 Start					*/
	and_ccr(~0x80);/* ccrの割込み禁止フラグ(bit7)をクリア	*/

}

void LED(void)//LEDを光らせる関数（現在の状態を知らせる）
{
	bios_led_output(A);
}

/****************************************************************************************************************/
/*	main関数																									*/
/****************************************************************************************************************/
int main(void)
{	
	int i;
	int B;
	agv_init();//関数の初期化

	for (i = 0;; i++)//出力をさせる
	{
		LED();
		
		// bios_led_output(AGV_STATE);		
		
		if ((A& 0x80) == 0x80)
		{
			TSTR|=0x10;
		}
	}
	return 0;
}


#pragma	interrupt main_beep
void main_beep(void)
{
	TSR4&=~0x01;
	if(count==6)
	{
		count=0;
		TSTR &=~0x10;
		TCNT4=0;
	}
	else
	{
		count +=1;
		
		if(count%2==1)
		{
			bios_beep_output(1);
		}
		else
		{
			bios_beep_output(0);
		}
	}
}
