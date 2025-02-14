/*****************************************************************************************************************
	FILE        :agvvars.h
	DATE        :Tue, Oct 18, 2011
	DESCRIPTION :AGVの共通変数等を定義する
	CPU TYPE    :H8/3052F

*****************************************************************************************************************/

#ifdef	MAIN		/* マクロMAINが定義されていれば変数領域を実体として確保する									*/

unsigned char	SW_DATA;							/* スイッチの状態が格納される変数							*/
unsigned char	SENS_DATA;							/* センサの状態が格納される変数								*/
unsigned char	MOTOR_STATE = MOTOR_STOP;			/* 走行用モータの状態変数									*/
unsigned char	AGV_STATE = AGV_BOOT;				/* AGVステート　初期値はAGV_BOOTとする						*/
unsigned short int	counter=0;
char from_run_alm = 0;

#else				/* マクロMAINが定義されていなければ変数領域を外部宣言として定義する							*/
	
extern	unsigned char	SW_DATA;		
extern	unsigned char	SENS_DATA;		
extern	unsigned char	MOTOR_STATE;		
extern	unsigned char	AGV_STATE;
extern	unsigned short int	counter;		
extern  char  from_run_alm;
#endif