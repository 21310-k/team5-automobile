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

//変数の仮定義
unsigned char agv_init(void);
void task_main(unsigned char agv_state);

/****************************************************************************************************************/
/*	AGVデバイス初期化モジュール agv_init																		*/
/****************************************************************************************************************/
/*!
    @brief 無人搬送車の初期化を行います
    @param void
    @return void
    @details 無人搬送車の初期化を行います
*/

unsigned char agv_init(void)
{
	
	/*ハードウェアの初期化*/
	MOTOR_STATE = MOTOR_STOP;        /*モータの停止*/
	bios_led_output(0x00);         /*LEDの消灯*/
	bios_da_output(HANDLE_CENTER);  /*ハンドルを中心に移動する*/
	AGV_STATE = AGV_BOOT;		 /*起動モード*/
	
	/*ITU0の初期化*/
	TCR0=0xa0;    /*GRA0で内部クロックφでコンペアマッチ*/
	TIOR0=0x8b;   /*GRA0はアウトプットコンペアレジスタとして使用*/
	GRA0=649;     /*100μs*/
	 
	/*ITU1の初期化*/
	TCR1=0xa6;    /*外部クロックCを使用*/
	TIER1|=0xf9;  /*ITU1は割込みを行う*/
	 
	/*ITU2の初期化*/
	TCR2=0xa6;    /*外部クロックCを使用*/
	TIER2|=0xf9;  /*ITU2は割込みを行う*/
	GRA2=99;      /*100μs×100=10ms*/
	 
	/*ITU3の初期化*/
	TCR3=0xa6;    /*外部クロックCを使用*/
	TIER3|=0xf9;  /*ITU3は割込みを行う*/
	GRA3=9;        /*100μs×10=1ms*/

	IPRA|=0x02;  /*ITU1の割込みをプライオリティレベル1に設定*/
	SYSCR&=~0x08;          /*SYSCRのUIビットをクリア*/
	and_ccr(~0x80);        /*すべての割込み許可*/
	TSTR|=0x0d;            /*ITU0,2,3をスタートする*/
}



/****************************************************************************************************************/
/*	main関数																									*/
/****************************************************************************************************************/
int main(void){
    while(-1){
        task_main(AGV_STATE);
		
    }
    return(0);
}

void task_main(unsigned char agv_state) {
	
    counter++;
	
    if(counter>1000){
        counter=0;
	}
    switch (agv_state) {
		
        case AGV_READY_ALM:
			break;
			
        case AGV_BOOT_ALM:
			break;
			
        case AGV_RUN_ALM:
            if (counter < 500) { // 1秒の間隔で点滅（500ms ON, 500ms OFF）
                bios_led_output(0xFF); // LEDを点灯
                bios_beep_output(1); // ブザーをON
            } else {
                bios_led_output(0x00); // LEDを消灯
                bios_beep_output(0); // ブザーをOFF
            }
            break;
 
        default: // それ以外の状態
            bios_led_output(0x00); // LEDを全部消灯
            bios_beep_output(0); // ブザーをOFF
            break;
    }
}




