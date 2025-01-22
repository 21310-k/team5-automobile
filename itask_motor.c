/*****************************************************************************************************************
/*	FILE        : itask_motor.c																					*/
/*	DATE        :Tue, Jan 17, 2012																				*/
/*	DESCRIPTION :AGV�̑��s�Ǘ����荞�݃^�X�N																	*/
/*	CPU TYPE    :H8/3052F																						*/
/****************************************************************************************************************/
#include	"machine.h"									/* �g���݊֐��̒�`										*/
#include	"reg3052.h"									/* H8/3052F�̓������W���[����`							*/
#include	"agvdef.h"									/* ���l�����Ԃ̃f�o�C�X��`								*/
#include	"agvvars.h"									/* ���l�����Ԃ̋��ʕϐ����`							*/
#include	"def_monitor_printf.h"
#include	"Acc_Table.h"								/* �����e�[�u���̓ǂݏo��								*/

/****************************************************************************************************************/
/*	���s���[�^����^�X�N itask_motor																			*/
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
