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

int beep(void);
void agv_init(void);
void led_out(void);

/****************************************************************************************************************/
/*	AGV�f�o�C�X���������W���[�� agv_init																		*/
/****************************************************************************************************************/
/*!
    @brief ���l�����Ԃ̏��������s���܂�
    @param void
    @return void
    @details ���l�����Ԃ̏��������s���܂�
*/

void agv_init(void)
{
	//���荞�ݏ��������ݒ�
	TCR0=0xA0;
	TIOR0=0x8B;
	GRA0=(625-1);
	
	

	TIER1=0xf9;
	TCR1=0xA6;
//	TIOR1=0x89;
	

	TIER2=0xf9;
	TCR2=0xA6;
//	TIOR2=0x89;
	GRA2=(100-1);
	



	TIER3=0xf9;
	TCR3=0xA6;
//	TIOR3=0x89;
	GRA3=(10-1);
	
	
    // DA�̏�����
    bios_da_output(0x7f);
    // �X�e�b�s���O���[�^�̏�����
    bios_step_output(0x00);
    // �Z���T�̏�����
    SENS_DATA = 0x00;
	SW_DATA = 0x00;
	
	IPRA=0x02;
	TSTR |= 0x0f;
//	TSTR |= 0x0d;
	SYSCR&=~0x08; 
	and_ccr(0x7f);
}


/****************************************************************************************************************/
/*	main�֐�																									*/
/****************************************************************************************************************/
/*int main(void)
{	
	for(;;)
	{
		agv_init();
		itask_input();
		bios_led_output(SENS_DATA);
		itask_control();

	}
	return 0;
}*/
int main(void)
{

 	agv_init();
	
//	MOTOR_STATE=MOTOR_ACC;
	for (;;) {
		led_out();
		//bios_led_output(SENS_DATA);
		//LED_PORT = AGV_STATE;
		//bios_led_output(SENS_DATA);
		//	MOTOR_STATE=MOTOR_ACC;
		/*
		bios_da_output(127);
		KAKU_STATE=(bios_ad_input()-127)/10;	
		if (KAKU_STATE==0){
			KAKU_STATE=1;
		}
		else if(KAKU_STATE<0)
		{
			KAKU_STATE=(KAKU_STATE*-1);
		}
		bios_led_output(KAKU_STATE);
		*/
		/*
		SENS_DATA = ~SENS_PORT;
		SW_DATA = SW_PORT;
		MOTOR_STATE = MOTOR_ACC;
		itask_control();
		LED_PORT = AGV_STATE;
		
		*/		
		/*
		(SW_DATA & 0x07) == 1;
		SENS_DATA = 0x01;
		*/
	//	beep();
	}
  	return 0; 
}


/****************************************************************************************************************/
/*	beep���W���[�� beep																							*/
/****************************************************************************************************************/
int beep(void){
int i;
for ( i = 0; i < 3; i++) {
			bios_beep_output(0x01);
			sleep_func(45000);
			bios_beep_output(0x00);
			sleep_func(45000);
}
sleep_func(180000);

for ( i = 0; i < 3; i++) {
			bios_beep_output(0x01);
			sleep_func(45000);
			bios_beep_output(0x00);
			sleep_func(45000);
}
sleep_func(180000);

for ( i = 0; i < 7; i++) {
			bios_beep_output(0x01);
			sleep_func(45000);
			bios_beep_output(0x00);
			sleep_func(45000);
}
sleep_func(180000);
}

/*
 sleep function;
*/

int sleep_func(unsigned int sleep_time) {
	int i = 0;
	for (i = 0; i <= sleep_time ; i++);
}
unsigned char led_state[]={0x55,0xaa};
int state_index=0;

void led_out(){
	switch(AGV_STATE){
		case AGV_READY:
		bios_led_output(0xFF);
		break;
		
	
		case AGV_RUN:
		if (GRA1 >=300){
                        previos_led_state=0x55;
			bios_led_output(0x55);
		}
		else if (GRA1<300){
			previos_led_state=0xaa;
			bios_led_output(0xAA);
		}
		else {
			bios_led_output(0x00);
		}
		break;
	
		case AGV_STOP_WAIT:
		bios_led_output(0xff);
		break;
		
		case AGV_RUN_AGAIN:
		bios_led_output(previos_led_state);
		break;
		
		default:
		bios_led_output(0x00);
		break;
		}
	
	
}

