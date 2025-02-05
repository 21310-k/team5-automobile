/****************************************************************************************************************/
/*  FILE        : itask_control.c                                                                               */
/*  DATE        :Tue, Jan 17, 2012                                                                              */
/*  DESCRIPTION :AGV�̓����ԊǗ����荞�݃^�X�N                                                              */
/*  CPU TYPE    :H8/3052F                                                                                       */
/****************************************************************************************************************/
#include    "machine.h"                                 /* �g���݊֐��̒�`                                     */
#include    "reg3052.h"                                 /* H8/3052F�̓������W���[����`                           */
#include    "agvdef.h"                                  /* ���l�����Ԃ̃f�o�C�X��`                             */
#include    "agvvars.h"                                 /* ���l�����Ԃ̋��ʕϐ����`                            */
#include    "def_monitor_printf.h"                      /* �g����printf�̒�`                                 */




/****************************************************************************************************************/
/*  �����ԓ����ԊĎ��E�n���h������^�X�N itask_control                                                       */
/****************************************************************************************************************/
//�֐��̉��錾
void agv_state(void);
unsigned char cal_sensor_position(void);
void handle_control(unsigned char spos);

//�ϐ��̒�`
unsigned char calculated_pos;
int i;

#pragma interrupt itask_control
void    itask_control(void)
{
	//�D��x1�̊��荞�݂�����
	and_ccr(~0x40);
	
    // �@�X�e�[�g�̃t�F�b�`�ƕύX (agv_state())
    // �A�Z���T�ʒu�̌v�Z(cal_sensor_position())
    // �B�n���h������(handle_control())
    //�ȏ�3��itask_control�����荞�܂ꂽ�ۂɎ��s����B
	agv_state();
	calculated_pos = cal_sensorposition();
	handle_control(calculated_pos);
	
	//ITU2�̊��荞�݃t���O���N���A
	TSR2 &= ~0x01;
	
}

/****************************************************************************************************************/
/*  AGV�X�e�[�g�Ǘ����W���[�� agv_state                                                                            */
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
//  �Z���T�ʒu�v�Z���W���[�� cal_sensor_position
// �����F�Ȃ�
// �߂�l�F�Z���T�̈ʒu���                                                             */
/****************************************************************************************************************/

unsigned char cal_sensor_position(void)
{
    // weight(i)=16+32*i�̌v�Z���ɑ���A�d�݂�ݒ�
    unsigned char weight[8] = {16, 48, 80, 112, 144, 176, 208, 240};
    // �Z���T���͂��擾
    unsigned char sens_input = bios_sensor_input();
    // �K�v�ȕϐ����`
    unsigned int molecule = 0;
    unsigned int denominator = 0;
    unsigned char position = 0;

    // �e���ɂ��Č��Ă���
    for (i = 0; i < 8; i++) {
        // sens_input��i���ڂ�1�̏ꍇ
        if ((sens_input & (1 << i))!=0) {
            // ���q�ɏd�݂����Z
            molecule += weight[i];
            // ��������Z
            denominator ++;
        }
    }

    //  position���Z�o
	if(denominator == 0){return 0;}
    position =(unsigned char)( molecule / denominator);
	printf("postion = %d\n",position);
    // position��Ԃ�
    return position;
    
}

/****************************************************************************************************************/
//  �n���h�����䃂�W���[�� handle_control
// �����Fcal_sensor_pos()�ŎZ�o�����ʒu���
// �߂�l�F�Ȃ�                                                                       */
/****************************************************************************************************************/
//spos��cal_sensor_pos()�ŎZ�o�����ʒu���
void handle_control(unsigned char spos)
{
    // �����diff�����߂�
    int diff = spos - HANDLE_CENTER;
    // ���݂̃n���h���؂�pad���擾
    unsigned char ad = bios_ad_input();
    // �ϐ���`
    int da_output = 0;

    // d��17/32���悶�����̂����݂̃n���h���؂�pad�ɉ��������̂�����l�ɋ��߂���B
    
	da_output = diff * 17 / 32;
	da_output = da_output + (int)(ad);
	
    if (da_output < 0){da_output = 0;}
	if (da_output > 255){da_output = 255;}
	
    // ����l���o��
	bios_led_output(ad);
    bios_da_output(da_output);
}
