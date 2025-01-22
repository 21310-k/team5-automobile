/****************************************************************************************************************/
/*	FILE        :main.c																							*/
/*	DATE        :Tue, Jan 17, 2012																				*/
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
int count=0;

void agv_init(void)
{
	LED_PORT = 0x00;
	DA_PORT=0x80;
	STEP_PORT=0x00;

	IPRA = 0x02;//ITU1���ŗD��ɂ���
	SYSCR &= 0xf7;

	TCR0 = 0xa0;/* TCR0�̐ݒ�						*/
	TIOR0 = 0x8b;/* TIOR0�̐ݒ�						*/
	GRA0 = 624;/* GRA0�̐ݒ�						*/

	TCR1 = 0xa6;/* TCR1�̐ݒ�						*/
	TIER1 = 0xf9;/* TIER1�̐ݒ�						*/
	GRA1=99;

	TCR2 = 0xa6;/* TCR2�̐ݒ�						*/
	TIER2 = 0xf9;/* TIER2�̐ݒ�						*/
	GRA2 = 999;

	TCR3 = 0xa0;/* TCR3�̐ݒ�						*/
	TIER3 = 0xf9;/* TIER3�̐ݒ�						*/
	GRA3 = 12499;
	
	TCR4=0xa6;
	TIER4=0xf9;
	GRA4=9999;
	

	TSTR |= 0x0d;//ITU0/ITU2/ITU3 Start					*/
	and_ccr(~0x80);/* ccr�̊����݋֎~�t���O(bit7)���N���A	*/

}

void LED(void)//LED�����点��֐��i���݂̏�Ԃ�m�点��j
{
	bios_led_output(A);
}

/****************************************************************************************************************/
/*	main�֐�																									*/
/****************************************************************************************************************/
int main(void)
{	
	int i;
	int B;
	agv_init();//�֐��̏�����

	for (i = 0;; i++)//�o�͂�������
	{
		LED();
		
		// bios_led_output(AGV_STATE);		
		
		if ((A& 0x80) == 0x80)
		{
			TSTR|=0x10;
		}
	}
	return 0;
}


#pragma	interrupt main_beep
void main_beep(void)
{
	TSR4&=~0x01;
	if(count==6)
	{
		count=0;
		TSTR &=~0x10;
		TCNT4=0;
	}
	else
	{
		count +=1;
		
		if(count%2==1)
		{
			bios_beep_output(1);
		}
		else
		{
			bios_beep_output(0);
		}
	}
}
