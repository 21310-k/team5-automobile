/****************************************************************************************************************/
/*	FILE        : itask_control.c																				*/
/*	DATE        :Tue, Jan 17, 2012																				*/
/*	DESCRIPTION :AGV�̓����ԊǗ����荞�݃^�X�N																*/
/*	CPU TYPE    :H8/3052F																						*/
/****************************************************************************************************************/
#include	"machine.h"									/* �g���݊֐��̒�`										*/
#include	"reg3052.h"									/* H8/3052F�̓������W���[����`							*/
#include	"agvdef.h"									/* ���l�����Ԃ̃f�o�C�X��`								*/
#include	"agvvars.h"									/* ���l�����Ԃ̋��ʕϐ����`							*/
#include	"def_monitor_printf.h"						/* �g����printf�̒�`									*/

int i; //�S�֐��ŋ��ʂȕϐ�
/****************************************************************************************************************/
/*	�����ԓ����ԊĎ��E�n���h������^�X�N itask_control														*/
/****************************************************************************************************************/
#pragma	interrupt itask_control
void	itask_control(void)
{
	
}

/****************************************************************************************************************/
/*	AGV�X�e�[�g�Ǘ����W���[�� agv_state																			*/
/****************************************************************************************************************/
void agv_state(void)
{
	//�Z���T���o�Ƃ�SENS_DATA��0x00�̒l�ȊO�ł��邱��
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
        }
        if (SENS_DATA != 0x00 && SW_DATA == 0x01 && MOTOR_STATE == MOTOR_STOP) {
            AGV_STATE = AGV_READY;
        }
        break;

    default:

        break;

    }
}    	

/****************************************************************************************************************/
//	�Z���T�ʒu�v�Z���W���[�� cal_sensor_position
// �����F�Ȃ�
// �߂�l�F�Z���T�̈ʒu���																*/
/****************************************************************************************************************/

unsigned char cal_sensor_position(void)
{
    // weight(i)=16+32*i�̌v�Z���ɑ���A�d�݂�ݒ�
    unsigned char weight[8] = { 16, 32, 48, 64, 80, 96, 112, 128 };
    // �Z���T���͂��擾
    unsigned char sens_input = bios_sensor_input();
    // �K�v�ȕϐ����`
    unsigned char molecule = 0;
    unsigned char denominator = 0;
    unsigned char position = 0;

    // �e���ɂ��Č��Ă���
    for (i = 0; i < 8; i++) {
        // sens_input��i���ڂ�1�̏ꍇ
        if (sens_input & (1 << i)) {
            // ���q�ɏd�݂����Z
            molecule += weight[i];
            // ��������Z
            denominator ++;
        }
    }

    //  position���Z�o
    position = molecule / denominator;
    // position��Ԃ�
    return position;
    
}

/****************************************************************************************************************/
//	�n���h�����䃂�W���[�� handle_control
// �����Fcal_sensor_pos()�ŎZ�o�����ʒu���
// �߂�l�F�Ȃ�																		*/
/****************************************************************************************************************/

void handle_control(unsigned char spos)
{
    // �����diff�����߂�
    unsigned char diff = spos - HANDLE_CENTER;
    // ���݂̃n���h���؂�pad���擾
    unsigned char ad = bios_ad_input();
    // �ϐ���`
    unsigned char da_output = 0;

    // d��17/32���悶�����̂����݂̃n���h���؂�pad�ɉ��������̂�����l�ɋ��߂���B
    da_output = diff * 17 / 32 + ad;
    
    // ����l���o��
    bios_da_output(da_output);
}
