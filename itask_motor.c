/*****************************************************************************************************************
/*	FILE        : itask_motor.c																					*/
/*	DATE        :Tue, Jan 17, 2012																				*/
/*	DESCRIPTION :AGVの走行管理割り込みタスク																	*/
/*	CPU TYPE    :H8/3052F																						*/
/****************************************************************************************************************/
#include	"machine.h"									/* 組込み関数の定義										*/
#include	"reg3052.h"									/* H8/3052Fの内蔵モジュール定義							*/
#include	"agvdef.h"									/* 無人搬送車のデバイス定義								*/
#include	"agvvars.h"									/* 無人搬送車の共通変数を定義							*/
#include	"def_monitor_printf.h"
#include	"Acc_Table.h"								/* 加速テーブルの読み出し								*/

/****************************************************************************************************************/
/*	走行モータ制御タスク itask_motor																			*/
/****************************************************************************************************************/
#pragma	interrupt itask_motor

void itask_motor(void)
{
	switch (MOTOR_STATE) {
		case MOTOR_STOP :
			ACC_STATE = 0;
			
			break;
		

		case MOTOR_ACC :
				bios_step_output();
				GRA1 = ACC_TABLE[ACC_STATE];
				ACC_STATE++;
				if(ACC_STATE == 400) {
					MOTOR_STATE = MOTOR_CONST;
				}

				if(AGV_STATE == AGV_STOP_WAIT || AGV_STATE == AGV_RUN_ALM) {
					MOTOR_STATE = MOTOR_BREAK;
			
				}
			
			break;

		

		case MOTOR_CONST :
			bios_step_output();
			GRA1=19;
			break;

		

		case MOTOR_BREAK :
				bios_step_output();
				GRA1 = ACC_TABLE[ACC_STATE];
				ACC_STATE--;
				if (ACC_STATE == 0) {
					MOTOR_STATE = MOTOR_STOP;
				}
			break;

		
	}
	TSR1&=~0x01;

}
