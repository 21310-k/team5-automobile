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

/****************************************************************************************************************/
/*	�����ԓ����ԊĎ��E�n���h������^�X�N itask_control														*/
/****************************************************************************************************************/
int agv_state(void);
unsigned char cal_sensor_position(void);
void handle_control(unsigned char sens_position);

#pragma	interrupt itask_control
void	itask_control(void)
{
	unsigned char sensor_position;
	agv_state();

	switch(AGV_STATE){
		
		case AGV_BOOT:{
			if((A & 0x03) == 0x01){
				AGV_STATE = AGV_READY;
			}
			break;
		}
		
		case AGV_READY:{
			if(((A & 0x82) == 0x02) && (bios_sensor_input() != 0)){
				AGV_STATE = AGV_RUN_WAIT;
				MOTOR_STATE = MOTOR_ACC;
				TSTR|=0x02;
			}
			else{
				AGV_STATE=AGV_READY;
			}
			break;
		}
		
		case AGV_RUN_WAIT:{
			if(MOTOR_STATE != MOTOR_STOP){
				AGV_STATE = AGV_RUN;
			}
			break;
		}
		
		case AGV_RUN:{
			if(SENS_ERROR == 0x01) {
				AGV_STATE = AGV_RUN_ALM;
				MOTOR_STATE = MOTOR_BREAK;
			}
			if((A & 0x02) == 0x00){
				AGV_STATE = AGV_STOP_WAIT;
				MOTOR_STATE = MOTOR_BREAK;
			}
			sensor_position = cal_sensor_position();
			handle_control(sensor_position);
			break;
		}
		
		case AGV_RUN_ALM:{
			if(((A & 0x02) == 0x00)&&(MOTOR_STATE==MOTOR_STOP)){
				AGV_STATE = AGV_READY;
				SENS_ERROR=0x00;
			}
			break;
		}
		
		case AGV_STOP_WAIT:{
			if ((((A & 0x02) == 0x02)&&(MOTOR_STATE==MOTOR_BREAK))||(SENS_ERROR == 0x01)){
				AGV_STATE = AGV_RUN_ALM;
			}
			if(MOTOR_STATE == MOTOR_STOP){
				AGV_STATE = AGV_READY;
			}
			break;
		}
	}

/*
    // AGV_STATE��AGV_BOOT�ŁAA��bit0��1����bit1��0�̏ꍇ�AAGV_STATE��AGV_READY�ɑJ��
    if (AGV_STATE == AGV_BOOT && (A & 0x01) && !(A & 0x02)) {
        AGV_STATE = AGV_READY;
    }
    // AGV_STATE��AGV_READY�ŁAA��bit1��1�Abit7��0�ASENS_DATA��0x00�łȂ��ꍇ�AAGV_STATE��AGV_RUN_WAIT�ɑJ��
    if (AGV_STATE == AGV_READY && (A & 0x02) && !(A & 0x80) && bios_sensor_input() != 0x00) {
        AGV_STATE = AGV_RUN_WAIT;
		MOTOR_STATE = MOTOR_ACC;
		TSTR|=0x02;
    }
    // AGV_STATE��AGV_RUN_WAIT�ŁAMOTOR_STATE��MOTOR_STOP�łȂ��ꍇ�AAGV_STATE��AGV_RUN�ɑJ��
    if (AGV_STATE == AGV_RUN_WAIT && MOTOR_STATE != MOTOR_STOP) {
        AGV_STATE = AGV_RUN;
		cal_sensor_position();
		handle_control(sensor_position);
    }
    // AGV_STATE��AGV_RUN�ŁAA��bit1��0�̏ꍇ�AAGV_STATE��AGV_STOP_WAIT�ɑJ��
    if (AGV_STATE == AGV_RUN && !(A & 0x02)) {
        AGV_STATE = AGV_STOP_WAIT;
		MOTOR_STATE = MOTOR_BREAK;
    }
    // AGV_STATE��AGV_STOP_WAIT�ŁAA��bit1��1���ASENS_ERROR��0x01�̏ꍇ�AAGV_STATE��AGV_RUN_ALM�ɑJ��
    if (((AGV_STATE == AGV_STOP_WAIT) && (A & 0x02)) ||( (AGV_STATE == AGV_STOP_WAIT) && (SENS_ERROR == 0x01))){
        AGV_STATE = AGV_RUN_ALM;
		MOTOR_STATE = MOTOR_BREAK;
    }
	 // AGV_STATE��AGV_RUN�ŁASENS_ERROR��0x01�̏ꍇ�AAGV_STATE��AGV_RUN_ALM�ɑJ��
    if ((AGV_STATE == AGV_RUN) && (SENS_ERROR == 0x01)) {
        AGV_STATE = AGV_RUN_ALM;
		MOTOR_STATE = MOTOR_BREAK;
    }
    // AGV_STATE��AGV_RUN_ALM�ŁAA��bit1��0�AMOTOR_STATE��MOTOR_STOP�̏ꍇ�AAGV_STATE��AGV_READY�ɑJ��
    if ((AGV_STATE == AGV_RUN_ALM) && !(A & 0x02) && (MOTOR_STATE == MOTOR_STOP)) {
        AGV_STATE = AGV_READY;
		SENS_ERROR=0x00;
    }
    // AGV_STATE��AGV_STOP_WAIT�ŁAMOTOR_STATE��MOTOR_STOP�̏ꍇ�AAGV_STATE��AGV_READY�ɑJ��
    if (AGV_STATE == AGV_STOP_WAIT && MOTOR_STATE == MOTOR_STOP) {
        AGV_STATE = AGV_READY;
    }
*/	
	TSR2&=~0x01;
}
/****************************************************************************************************************/
/*	AGV�X�e�[�g�Ǘ����W���[�� agv_state																			*/
/****************************************************************************************************************/
int agv_state(void)
{
	A= 0x00;   // �����l��0x00�ɐݒ�
	
    // SW_DATA��0bit�ڂ�1�̏ꍇ�AA��0bit�ڂ�1�ɂ���
    // SW_DATA��0bit�ڂ�0�̏ꍇ�AA��0bit�ڂ�0�ɂ���
    if (SW_DATA & 0x01) {
        A |= 0x01;   // A��0bit�ڂ�1�ɂ���
    }
    else {
        A &= ~0x01;  // A��0bit�ڂ�0�ɂ���
    }
	// SW_DATA��1bit�ڂ�1�̏ꍇ�AA��1bit�ڂ�1�ɂ���
    // SW_DATA��1bit�ڂ�0�̏ꍇ�AA��1bit�ڂ�0�ɂ���
    if (SW_DATA & 0x02) {
        A |= 0x02;   // A��1bit�ڂ�1�ɂ���
    }
    else {
        A &= ~0x02;  // A��1bit�ڂ�0�ɂ���
    }
    if (MOTOR_STATE == MOTOR_ACC) {   // �������
        A |= 0x08;   // A��3bit�ڂ�1�ɂ���
    }
    else if (MOTOR_STATE == MOTOR_CONST) {   // �葬���
        A &= ~0x08;  // A��3bit�ڂ�0�ɂ���
        A |= 0x10;   // A��4bit�ڂ�1�ɂ���
    }
    else if (MOTOR_STATE == MOTOR_BREAK) {   // �������
        A &= ~0x08;  // A��3bit�ڂ�0�ɂ���
        A &= ~0x10;  // A��4bit�ڂ�0�ɂ���
        A |= 0x20;   // A��5bit�ڂ�1�ɂ���
    }
    else if (MOTOR_STATE == MOTOR_STOP) {   // ��~���
        A &= ~0x20;  // A��5bit�ڂ�0�ɂ���    
    }
    // AGV_STATE�̒l�ɉ�����A��bit7��ύX
    if (AGV_STATE == AGV_RUN_ALM) {
        A |= 0x80;   // A��bit7��1�ɂ���
    }
    else if (AGV_STATE == AGV_READY) {
        A &= ~0x80;  // A��bit7��0�ɂ���
    }
}
/****************************************************************************************************************/
/*	�Z���T�ʒu�v�Z���W���[�� cal_sensor_position																*/
/****************************************************************************************************************/
unsigned char cal_sensor_position(void)
{
    int binary[8];                /* �e�r�b�g�̒l���i�[����z�� */
    int weight[8];                /* �e�r�b�g�̏d�݂��i�[����z�� */
    int sum_weighted_bits = 0;    /* ��(weight(i) * bit(i)) */
    int count = 0;                /* ��(bit(i)) */
    unsigned char position = 0;       /* �v�Z���ʂ�Position */
    int i = 0;

    /* 2�i���ϊ��� weight(i) �v�Z */
    for (i = 0; i < 8; i++) {
        binary[i] = (SENS_DATA >> i) & 1;     /* bit(i)�̌v�Z */
        weight[i] = 16 + 32 * i;              /* weight(i)�̌v�Z */

        /* ��(weight(i) * bit(i)) �� ��(bit(i)) �̌v�Z */
        sum_weighted_bits += weight[i] * binary[i];
        count += binary[i];
    }
    /* Position�v�Z */
        if (count != 0) {
        position = sum_weighted_bits / count;
    }
    return (position);
}
/****************************************************************************************************************/
/*	�n���h�����䃂�W���[�� handle_control																		*/
/****************************************************************************************************************/
void handle_control(unsigned char sens_position)
{
    unsigned char control = 0;
    int d;                              /*�����*/
    int diff;                           /*�v�Z�l*/
    unsigned char ad;                   /* ���݂̃n���h���؂�p */
    unsigned char correction;           /* �C���l */
	d = sens_position - SENS_CENTER;          /*�����*/
    ad = bios_ad_input();                     /* ���݂̃n���h���؂�p */
    diff = d * 17 / 32;
    control = diff + ad;
    bios_da_output(control);
}