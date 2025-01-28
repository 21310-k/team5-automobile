/****************************************************************************************************************/
/*	FILE        :main.c																							*/
/*	DATE        :Tue, Jan 17, 2012																				*/
/*	DESCRIPTION :AGV Main Program																				*/
/*	CPU TYPE    :H8/3052F																						*/
/****************************************************************************************************************/
#include	"stdio.h"
#define	MAIN										/* �}�N��MAIN�̒�`											*/

#include	"machine.h"								/* �g���݊֐��̒�`											*/
#include	"reg3052.h"								/* H8/3052F�̓������W���[����`								*/
#include	"agvdef.h"								/* ���l�����Ԃ̃f�o�C�X��`									*/
#include	"agvvars.h"								/* ���l�����Ԃ̋��ʕϐ����`								*/
#include	"def_monitor_printf.h"

/****************************************************************************************************************/
/*	AGV�f�o�C�X���������W���[�� agv_init																		*/
/****************************************************************************************************************/
void agv_init(void) {
    //�{��
    //�n�[�h�E�F�A�̏�����
    MOTOR_STATE = MOTOR_STOP;        //���[�^�̒�~
    bios_led_output(0x00);         //LED�̏���
    bios_da_output(HANDLE_CENTER);  //�n���h���𒆐S�Ɉړ�����

    //ITU0�̏�����
    TCR0 = 0xa0;
    TIOR0 = 0x8b;
    GRA0 = 1249;

    //ITU1�̏�����
    TCR1 = 0xa6;
    TIER1 |= 0xf9;

    //ITU2�̏�����
    TCR2 = 0xa6;
    TIER2 |= 0xf9;
    GRA2 = 99;

    //ITU3�̏�����
    TCR3 = 0xa6;
    TIER3 |= 0xf9;
    GRA3 = 9;

    IPRA |= 0x02;  //ITU1�̊����݂��v���C�I���e�B���x��1�ɐݒ�


    TSTR |= 0x0d;            //ITU0�E2�E3���X�^�[�g����
    and_ccr(~0x80);        //�����݋���


    //�e�X�g�p
            //�n�[�h�E�F�A�̏�����
        MOTOR_STATE = MOTOR_STOP;        //���[�^�̒�~
    bios_led_output(0x00);         //LED�̏���
    bios_da_output(HANDLE_CENTER);  //�n���h���𒆐S�Ɉړ�����
    AGV_STATE = AGV_BOOT;

    //ITU0�̏�����
    TCR0 = 0xa0;
    TIOR0 = 0x8b;
    GRA0 = 1249;

    //ITU1�̏�����
    TCR1 = 0xa6;
    TIER1 |= 0xf9;
    GRA1 = 5000;

    //ITU2�̏�����
    TCR2 = 0xa6;
    TIER2 |= 0xf9;
    GRA2 = 5000;

    //ITU3�̏�����
    TCR3 = 0xa6;
    TIER3 |= 0xf9;
    GRA3 = 5000;

    TSTR |= 0x0f;            //ITU0�E1�E2�E3���X�^�[�g����
    and_ccr(~0x80);        //�����݋���
}

#pragma interrupt LED_interrupt
void LED_interrupt(void) {
    static unsigned char LED_FLAG = 0;// �ÓI�ϐ�LED_FLAG���`
    if (LED_FLAG) {
        bios_led_output(0x00);// LED_FLAG��0�Ȃ��LED�S�_��
    }
    else {
        bios_led_output(0xff);// LFLAG��0�łȂ����LED�S����
    }
    LED_FLAG = ~LED_FLAG;// LED_FLAG�𔽓]����
    TSR1 &= ~0x01;// ITU1�̊����݃t���O�̃N���A
}


/****************************************************************************************************************/
/*	main�֐�																									*/
/****************************************************************************************************************/
int main(void)
{	
	unsigned int i = 0; 
	while(-1){
		for(i = 0; i < 60000; i++){}
		bios_step_output();
	}
	return 0;
}

/****************************************************************************************************************/
/*	beep���W���[�� beep																							*/
/****************************************************************************************************************/
void beep(void)
{

}
