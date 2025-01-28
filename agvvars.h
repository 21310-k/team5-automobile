/*****************************************************************************************************************
	FILE        :agvvars.h
	DATE        :Tue, Oct 18, 2011
	DESCRIPTION :AGV�̋��ʕϐ������`����
	CPU TYPE    :H8/3052F

*****************************************************************************************************************/

#ifdef	MAIN		/* �}�N��MAIN����`����Ă���Εϐ��̈�����̂Ƃ��Ċm�ۂ���									*/

unsigned char	SW_DATA;							/* �X�C�b�`�̏�Ԃ��i�[�����ϐ�							*/
unsigned char	SENS_DATA;							/* �Z���T�̏�Ԃ��i�[�����ϐ�								*/
unsigned char	MOTOR_STATE = MOTOR_STOP;			/* ���s�p���[�^�̏�ԕϐ�									*/
unsigned char	AGV_STATE = AGV_BOOT;				/* AGV�X�e�[�g�@�����l��AGV_BOOT�Ƃ���						*/

#else				/* �}�N��MAIN����`����Ă��Ȃ���Εϐ��̈���O���錾�Ƃ��Ē�`����							*/
	
extern	unsigned char	SW_DATA;		
extern	unsigned char	SENS_DATA;		
extern	unsigned char	MOTOR_STATE;		
extern	unsigned char	AGV_STATE;		

#endif