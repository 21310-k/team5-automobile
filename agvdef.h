/*****************************************************************************************************************
	FILE        :agvdef.h
	DATE        :Tue, Oct 18, 2011
	DESCRIPTION :AGVの入出力デバイス関する定義を行う
	CPU TYPE    :H8/3052F

*****************************************************************************************************************/

/****	ディジタル入出力ポートに関するアドレス定義															*****/
#define	STEP_PORT	(*(volatile unsigned char *)0xa0000)		/* ステッピングモータ相出力ポートのアドレス定義	*/
#define	SENS_PORT	(*(volatile unsigned char *)0xa0000)		/* センサ情報入力ポートのアドレス定義			*/

/****	ワークベンチパネルデバイスのポートに関するアドレス定義												*****/
#define	SW_PORT		(*(volatile unsigned char *)0xa0007)		/* SWポートのアドレス定義						*/
#define	LED_PORT	(*(volatile unsigned char *)0xa0007)		/* LEDポートのアドレス定義						*/
#define	BEEP_PORT	(*(volatile unsigned char *)0xa0006)		/* BEEPポートのアドレス定義						*/

#define	BEEP_ON		0x01										/* BEEP ON コード								*/
#define	BEEP_OFF	0x00										/* BEEP OFF コード								*/	

/****	A/D関係のポートに関するアドレス定義およびbit定義													*****/
#define	SH_PORT		(*(volatile unsigned char *)0xa0003)		/* S&H動作指定ポートのアドレス定義				*/
																/* bit7=1でサンプルモード/0でホールド			*/
#define	SAMPLE_MODE	0x80										/* サンプルモードのbit定義						*/
#define	HOLD_MODE	0x00										/* ホールドモードのbit定義						*/

#define	AD_PORT		(*(volatile unsigned char *)0xa0002)		/* 任意値書込みで変換開始/読出し=変換値			*/

/****	D/Aポートに関するアドレス定義																		*****/
#define DA_PORT		(*(volatile unsigned char *)0xa0004)		/* D/Aポートのアドレス定義						*/

/**** 操作スイッチに関連する定義																			*****/
#define	RUN_SW	0x01											/* SW_DATAのこのbitが立つと走行開始				*/

/****	センサおよびハンドルに関連する定義																	*****/
#define SENS_CENTER		0x80									/* センサの中央値								*/
#define HANDLE_CENTER	0x80									/* ハンドルの中央値								*/

#define	Kp		0.8												/* ハンドル制御比例係数							*/
#define	Ki		0.05											/* ハンドル制御積分係数							*/
#define	INT_MAX	1000											/* 積分計算の上限値								*/

/**** モータ動作状態の定義																				*****/
#define	MOTOR_STOP	0											/* モータは停止状態								*/
#define	MOTOR_ACC	1											/* モータは加速状態								*/
#define	MOTOR_CONST	2											/* モータは定速状態								*/
#define	MOTOR_BREAK	3											/* モータは減速状態								*/


/**** AGVステートに関連する定義																				*****/
#define	AGV_BOOT		0x01									/* AGV起動状態									*/
#define	AGV_READY		0x02									/* AGV運転待機状態								*/
#define	AGV_RUN_WAIT	0x03									/* AGV走行開始要求								*/
#define	AGV_STOP_WAIT	0x04									/* AGV走行停止要求								*/
#define	AGV_RUN			0x05									/* AGV走行状態									*/
#define	AGV_BOOT_ALM	0x81									/* AGV起動アラーム状態							*/
#define	AGV_READY_ALM	0x82									/* AGV運転待機アラーム							*/
#define	AGV_RUN_ALM		0x83									/* AGV走行アラーム状態							*/

#define	AGV_ALM_BIT	0x80										/* AGVアラーム状態のSTATE bit					*/