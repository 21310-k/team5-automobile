/*****************************************************************************************************************
/*    FILE        : itask_motor.c                                                                                    */
/*    DATE        :Tue, Jan 17, 2012                                                                                */
/*    DESCRIPTION :AGV�̑��s�Ǘ����荞�݃^�X�N                                                                    */
/*    CPU TYPE    :H8/3052F                                                                                        */
/****************************************************************************************************************/
#include    "machine.h"                                /* �g���݊֐��̒�`                                        */
#include    "reg3052.h"                                    /* H8/3052F�̓������W���[����`                            */
#include    "agvdef.h"                                    /* ���l�����Ԃ̃f�o�C�X��`                                */
#include    "agvvars.h"                                    /* ���l�����Ԃ̋��ʕϐ����`                            */
#include    "def_monitor_printf.h"
#include    "Acc_Table.h"                                /* �����e�[�u���̓ǂݏo��                                */
/****************************************************************************************************************/
/*    ���s���[�^����^�X�N itask_motor                                                                            */
/****************************************************************************************************************/
/***********************************************************************************************/
/*
���̊֐��ł�, 
1. ���[�^�̉�]��Ԃ�ω�����.
2. ���[�^�̏�Ԃ𐧌䂷��.
3. ���[�^�̏�Ԃ����Ƃ�, �����e�[�u����GRA1��p����, ���荞�݂���^�C�~���O��ω�����.  
   �܂�, itask_motor���Ăяo�����^�C�~���O��ω����邱�ƂŃ��[�^�̉�]�͂��߂�ς���. 
*/
unsigned char frag_state_motor;
unsigned char frag_motor_state_motor;

#pragma interrupt itask_motor
void itask_motor(void)
{

    int static pointer = 0;
	frag_state_motor = AGV_STATE;
	frag_motor_state_motor = MOTOR_STATE;
    /*
    ���̃^�X�N���N�����ꂽ�Ƃ��ɁAMOTOR_STATE���uMOTOR_STOP�ȊO�̏����v�ł̓��[�^�̗㎥�o�͂����Ԃɏo�͂���
    �܂�, ���[�^����]����
    */
    if (MOTOR_STATE != MOTOR_STOP) {
        //return MOTOR_STATE;
        bios_step_output();
        //printf("���[�^�̏�Ԃ�%u", MOTOR_STATE);
    }
    else {
        MOTOR_STATE = MOTOR_STOP;
    }


    switch (MOTOR_STATE) {
    //���[�^��~��Ԃ̂Ƃ�
    case MOTOR_STOP:
        pointer = 0; /*�|�C���^�ϐ���������*/
        GRA1 = ACC_TABLE[pointer]; /*�����e�[�u���̐擪�̒l��GRA1�ɃZ�b�g����.*/
        MOTOR_STATE = MOTOR_ACC; /*���[�^�̏�Ԃ�������ԂɑJ�ڂ���*/
        break;

    //���[�^��������Ԃ̎�
    case MOTOR_ACC: {
        /*
        �����e�[�u���̃|�C���^�����݂̏�Ԃ���C���N�������g����
        �܂�, ���荞�ݎ������ς���Ă���. �܂�, itask_motor�̌Ăяo���������ς��. 
        */
		
        pointer++;
        GRA1 = ACC_TABLE[pointer]; 

        /*�e�[�u���|�C���^��(�e�[�u����-1)�Ɠ������Ȃ����Ƃ����[�^���葬��ԂɂȂ�*/
        if (pointer == 400) {
            MOTOR_STATE = MOTOR_CONST;
        }
        /*��������AGV_STOP_WAIT���邢��AGV_RUN_ALM�ɂȂ�Ό�����ԂɂȂ�*/
        if ((AGV_STATE == AGV_STOP_WAIT) || (AGV_STATE == AGV_RUN_ALM)) {
            MOTOR_STATE = MOTOR_BREAK;
        }
        break;
    }

    /*���[�^���葬��Ԃ̎�*/
    case MOTOR_CONST: {
        /*�葬����AGV_STOP_WAIT���邢��AGV_RUN_ALM�ɂȂ�Ό�����ԂɂȂ�*/
        if ((AGV_STATE == AGV_STOP_WAIT) || (AGV_STATE == AGV_RUN_ALM)) {
            MOTOR_STATE = MOTOR_BREAK;
        }
        break;
    }

    /*���[�^��������Ԃ̎�*/
    case MOTOR_BREAK: {
        //itask_motor�̌Ăяo�����x��x������. 
        pointer--; /*�|�C���^�̒l���f�N�������g*/
        GRA1 = ACC_TABLE[pointer]; /*GRA1�Ƀe�[�u���̒l���Z�b�g*/
        /*GRA1�̒l��999�ɂȂ��MOTOR_STATE���~��Ԃɂ���*/
        if (GRA1 == 999) {
            MOTOR_STATE = MOTOR_STOP;
            /*ITU�̓�����~����*/
            TSTR &= ~0x02; 
        }
		//������Ԃ̂܂ܑ��s��ԂɈڍs����Ɖ�������
		//�������݂̑��xGRA1��51��艺�����������
		//pointer��200�ȉ��̏�����ǉ�
		//if((AGV_STATE == AGV_RUN) && (GRA1 < 51)){
		//	MOTOR_STATE = MOTOR_ACC;
		//	}
		//if ((AGV_STATE == AGV_RUN) && (from_run_alm == 1) && (GRA1 > 100)){
		//		MOTOR_STATE = MOTOR_ACC;
		//		from_run_alm = 0;
		//		}
		
		//else if((AGV_STATE == AGV_RUN) && (from_run_alm == 1)){
		//		from_run_alm = 0;
		//		}
		
        break;
    }

    default:
        break;
    }
    //printf("���[�^�̏�Ԃ�%u", MOTOR_STATE); //�e�X�g�p
	
	//�e�X�g�p
	
		if ((frag_state_motor != AGV_STATE) || (frag_motor_state_motor != MOTOR_STATE)){
		printf("AGV_STATE:%d , ", AGV_STATE);
		printf("MOTOR_STATE:%d\n" , MOTOR_STATE);
		}
	
	
	
	
	//ITU1�̊��荞�݃t���O���N���A
	TSR1 &= ~0x01;
}
