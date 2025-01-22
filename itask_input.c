/*あなたのプログラムを以下にプログラムを挿入する*/
/****************************************************************************************************************/
/*  FILE        : itask_input.c                                                                                 */
/*  DATE        :Tue, Jan 17, 2012                                                                              */
/*  DESCRIPTION :AGVの入力割り込みタスク                                                                      */
/*  CPU TYPE    :H8/3052F                                                                                       */
/****************************************************************************************************************/
#include    "machine.h"                             /* 組込み関数の定義                                         */
#include    "reg3052.h"                             /* H8/3052Fの内蔵モジュール定義                               */
#include    "agvdef.h"                              /* 無人搬送車のデバイス定義                                 */
#include    "agvvars.h"                             /* 無人搬送車の共通変数を定義                                */
#include    "def_monitor_printf.h"

/****************************************************************************************************************/
/*  入力タスク itask_input                                                                                       */
/****************************************************************************************************************/
//#pragma interrupt itask_input
void itask_input(void)
{
	static unsigned char sw_data0,sw_data1,sw_data2;
	static int i=0,j=0;
	if(i==0){
		sw_data0=bios_sw_input(); //iが0の時、スイッチの値をsw_data0に代入
	}
	else if(i==1){
		sw_data1=bios_sw_input(); //iが１の時、スイッチの値をsw_data1に代入
	}
	else{
		sw_data2=bios_sw_input(); //iが２の時、スイッチの値をsw_data2に代入
	}
	i++;
	if(i==3){//iが３になったら０になるようにする
		i=0;
	}
	
	if((sw_data0==sw_data1)&&(sw_data1==sw_data2)){//過去３回のスイッチの値が一致した場合にSW_DATAにスイッチの値を代入
	    SW_DATA=bios_sw_input();
	}
	
    if(0x00!=bios_sensor_input()){
		SENS_DATA=bios_sensor_input(); 
		j=0;
	}
	else if(0==bios_sensor_input()){
		j++;
	}
	if(j==10){
		SENS_DATA=0;
		j=0;
	}
}

