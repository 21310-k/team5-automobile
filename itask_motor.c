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


#pragma    interrupt itask_motor
/*
���̊֐��ł�, 
1. ���[�^�̉�]��Ԃ�ω�����.
2. ���[�^�̏�Ԃ𐧌䂷��.
3. ���[�^�̏�Ԃ����Ƃ�, �����e�[�u����GRA1��p����, ���荞�݂���^�C�~���O��ω�����.  
   �܂�, itask_motor���Ăяo�����^�C�~���O��ω����邱�ƂŃ��[�^�̉�]�͂��߂�ς���. 
*/
void itask_motor(void)
{

    int static pointer = 0;

    /*
    ���̃^�X�N���N�����ꂽ�Ƃ��ɁAMOTOR_STATE���uMOTOR_STOP�ȊO�̏����v�ł̓��[�^�̗㎥�o�͂����Ԃɏo�͂���
    �܂�, ���[�^����]����
    */
    if (MOTOR_STATE != MOTOR_STOP) {
        //return MOTOR_STATE;
        bios_step_output();
        printf("���[�^�̏�Ԃ�%u", MOTOR_STATE);
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

    /*���[�^���ᑬ��Ԃ̎�*/
    case MOTOR_CONST: {
        /*�葬����AGV_STOP_WAIT���邢��AGV_RUN_ALM�ɂȂ�Ό�����ԂɂȂ�*/
        if ((AGV_STATE == AGV_STOP_WAIT) || (AGV_STATE == AGV_RUN_ALM)) {
            MOTOR_STATE = MOTOR_BREAK;
        }
        break;
    }

    /*���[�^����~��Ԃ̎�*/
    case MOTOR_BREAK: {
        //itask_motor�̌Ăяo�����x��x������. 
        pointer--; /*�|�C���^�̒l���f�N�������g*/
        GRA1 = ACC_TABLE[pointer]; /*GRA1�Ƀe�[�u���̒l���Z�b�g*/
        /*�e�[�u���̒l��0�ɂȂ��MOTOR_STATE���~��Ԃɂ���*/
        if (pointer == 0) {
            MOTOR_STATE = MOTOR_STOP;
            /*ITU�̓�����~����*/
            TCNT1 = 0;
        }
        break;
    }

    default:
        break;
    }
    //printf("���[�^�̏�Ԃ�%u", MOTOR_STATE); //�e�X�g�p
	//ITU1�̊��荞�݃t���O���N���A
	TSR1 &= ~0x01;
}

