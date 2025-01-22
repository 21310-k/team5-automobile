/*���Ȃ��̃v���O�������ȉ��Ƀv���O������}������*/
/****************************************************************************************************************/
/*  FILE        : itask_input.c                                                                                 */
/*  DATE        :Tue, Jan 17, 2012                                                                              */
/*  DESCRIPTION :AGV�̓��͊��荞�݃^�X�N                                                                      */
/*  CPU TYPE    :H8/3052F                                                                                       */
/****************************************************************************************************************/
#include    "machine.h"                             /* �g���݊֐��̒�`                                         */
#include    "reg3052.h"                             /* H8/3052F�̓������W���[����`                               */
#include    "agvdef.h"                              /* ���l�����Ԃ̃f�o�C�X��`                                 */
#include    "agvvars.h"                             /* ���l�����Ԃ̋��ʕϐ����`                                */
#include    "def_monitor_printf.h"

/****************************************************************************************************************/
/*  ���̓^�X�N itask_input                                                                                       */
/****************************************************************************************************************/
//#pragma interrupt itask_input
void itask_input(void)
{
	static unsigned char sw_data0,sw_data1,sw_data2;
	static int i=0,j=0;
	if(i==0){
		sw_data0=bios_sw_input(); //i��0�̎��A�X�C�b�`�̒l��sw_data0�ɑ��
	}
	else if(i==1){
		sw_data1=bios_sw_input(); //i���P�̎��A�X�C�b�`�̒l��sw_data1�ɑ��
	}
	else{
		sw_data2=bios_sw_input(); //i���Q�̎��A�X�C�b�`�̒l��sw_data2�ɑ��
	}
	i++;
	if(i==3){//i���R�ɂȂ�����O�ɂȂ�悤�ɂ���
		i=0;
	}
	
	if((sw_data0==sw_data1)&&(sw_data1==sw_data2)){//�ߋ��R��̃X�C�b�`�̒l����v�����ꍇ��SW_DATA�ɃX�C�b�`�̒l����
	    SW_DATA=bios_sw_input();
	}
	
    if(0x00!=bios_sensor_input()){
		SENS_DATA=bios_sensor_input(); 
		j=0;
	}
	else if(0==bios_sensor_input()){
		j++;
	}
	if(j==10){
		SENS_DATA=0;
		j=0;
	}
}

