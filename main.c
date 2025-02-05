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

//�ϐ��̉���`
unsigned char agv_init(void);
void task_main(unsigned char agv_state);

/****************************************************************************************************************/
/*	AGV�f�o�C�X���������W���[�� agv_init																		*/
/****************************************************************************************************************/
/*!
    @brief ���l�����Ԃ̏��������s���܂�
    @param void
    @return void
    @details ���l�����Ԃ̏��������s���܂�
*/

unsigned char agv_init(void)
{
	
	/*�n�[�h�E�F�A�̏�����*/
	MOTOR_STATE = MOTOR_STOP;        /*���[�^�̒�~*/
	bios_led_output(0x00);         /*LED�̏���*/
	bios_da_output(HANDLE_CENTER);  /*�n���h���𒆐S�Ɉړ�����*/
	AGV_STATE = AGV_BOOT;		 /*�N�����[�h*/
	
	/*ITU0�̏�����*/
	TCR0=0xa0;    /*GRA0�œ����N���b�N�ӂŃR���y�A�}�b�`*/
	TIOR0=0x8b;   /*GRA0�̓A�E�g�v�b�g�R���y�A���W�X�^�Ƃ��Ďg�p*/
	GRA0=649;     /*100��s*/
	 
	/*ITU1�̏�����*/
	TCR1=0xa6;    /*�O���N���b�NC���g�p*/
	TIER1|=0xf9;  /*ITU1�͊����݂��s��*/
	 
	/*ITU2�̏�����*/
	TCR2=0xa6;    /*�O���N���b�NC���g�p*/
	TIER2|=0xf9;  /*ITU2�͊����݂��s��*/
	GRA2=99;      /*100��s�~100=10ms*/
	 
	/*ITU3�̏�����*/
	TCR3=0xa6;    /*�O���N���b�NC���g�p*/
	TIER3|=0xf9;  /*ITU3�͊����݂��s��*/
	GRA3=9;        /*100��s�~10=1ms*/

	IPRA|=0x02;  /*ITU1�̊����݂��v���C�I���e�B���x��1�ɐݒ�*/
	SYSCR&=~0x08;          /*SYSCR��UI�r�b�g���N���A*/
	and_ccr(~0x80);        /*���ׂĂ̊����݋���*/
	TSTR|=0x0d;            /*ITU0,2,3���X�^�[�g����*/
}



/****************************************************************************************************************/
/*	main�֐�																									*/
/****************************************************************************************************************/
int main(void){
    while(-1){
        task_main(AGV_STATE);
		
    }
    return(0);
}

void task_main(unsigned char agv_state) {
	
    counter++;
	
    if(counter>1000){
        counter=0;
	}
    switch (agv_state) {
		
        case AGV_READY_ALM:
			break;
			
        case AGV_BOOT_ALM:
			break;
			
        case AGV_RUN_ALM:
            if (counter < 500) { // 1�b�̊Ԋu�œ_�Łi500ms ON, 500ms OFF�j
                bios_led_output(0xFF); // LED��_��
                bios_beep_output(1); // �u�U�[��ON
            } else {
                bios_led_output(0x00); // LED������
                bios_beep_output(0); // �u�U�[��OFF
            }
            break;
 
        default: // ����ȊO�̏��
            bios_led_output(0x00); // LED��S������
            bios_beep_output(0); // �u�U�[��OFF
            break;
    }
}




