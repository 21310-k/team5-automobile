/************************************************************************
	RAM領域に割込みベクタを設定する
	モニタ機能である仮想割込みベクタの使用を前提とするため，
	割込み番号1番から設定する
	ROM化する場合は変更が必要
	デフォルト値はPowerONResetを定義する
*************************************************************************/

/************************************************************************
	外部宣言領域				
	割込み関数を外部ファイルで定義する場合は，ここで外部宣言を行なう
*************************************************************************/
	extern	void	PowerON_Reset(void);
	extern	void	itask_control(void);
	extern	void	itask_input(void);
	extern  void    itask_motor(void);
/************************************************************************
	割込みベクタの定義
	割込みベクタはセクションVectTableに置く				
*************************************************************************/

#pragma section VectTable

const	unsigned int *IntVect[]={
	PowerON_Reset,						/* 0:  Reset		*/
	PowerON_Reset,						/* 1:  Reserved		*/
	PowerON_Reset,						/* 2:  Reserved		*/
	PowerON_Reset,						/* 3:  Reserved		*/
	PowerON_Reset,						/* 4:  Reserved		*/
	PowerON_Reset,						/* 5:  Reserved		*/
	PowerON_Reset,						/* 6:  Reserved		*/
	PowerON_Reset,						/* 7:  NMI			*/
	PowerON_Reset,						/* 8:  TRAPA0		*/
	PowerON_Reset,						/* 9:  TRAPA1		*/
	PowerON_Reset,						/* 10: TRAPA2		*/
	PowerON_Reset,						/* 11: TRAPA3		*/

	PowerON_Reset,						/* 12: IRQ0			*/
	PowerON_Reset,						/* 13: IRQ1			*/
	PowerON_Reset,						/* 14: IRQ2			*/
	PowerON_Reset,						/* 15: IRQ3			*/
	PowerON_Reset,						/* 16: IRQ4			*/
	PowerON_Reset,						/* 17: IRQ5			*/

	PowerON_Reset,						/* 18: Reserve		*/
	PowerON_Reset,						/* 19: Reserve		*/

	PowerON_Reset,						/* 20: WOVI(インターバルタイマ)		*/
	PowerON_Reset,						/* 21: CMI(コンペアマッチ)			*/

	PowerON_Reset,						/* 22: Reserve		*/
	PowerON_Reset,						/* 23: Reserve		*/

	PowerON_Reset,						/* 24: IMIA0(TIU0コンペアマッチ/インプットキャプチャA0)		*/
	PowerON_Reset,						/* 25: IMIB0(TIU0コンペアマッチ/インプットキャプチャB0)		*/
	PowerON_Reset,						/* 26: OVI0(TIU0オーバーフロー0)							*/

	PowerON_Reset,						/* 27: Reserve		*/

	itask_motor,						/* 28: IMIA1(TIU1コンペアマッチ/インプットキャプチャA1)		*/
	PowerON_Reset,						/* 29: IMIB1(TIU1コンペアマッチ/インプットキャプチャB1)		*/
	PowerON_Reset,						/* 30: OVI1(TIU1オーバーフロー1)							*/

	PowerON_Reset,						/* 31: Reserve		*/

	itask_control,						/* 32: IMIA2(TIU2コンペアマッチ/インプットキャプチャA2)		*/
	PowerON_Reset,						/* 33: IMIB2(TIU2コンペアマッチ/インプットキャプチャB2)		*/
	PowerON_Reset,						/* 34: OVI2(TIU2オーバーフロー2)							*/

	PowerON_Reset,						/* 35: Reserve		*/

	itask_input,						/* 36: IMIA3(TIU3コンペアマッチ/インプットキャプチャA3)		*/
	PowerON_Reset,						/* 37: IMIB3(TIU3コンペアマッチ/インプットキャプチャB3)		*/
	PowerON_Reset,						/* 38: OVI3(TIU3オーバーフロー3)							*/

	PowerON_Reset,						/* 39: Reserve		*/

	PowerON_Reset,						/* 40: IMIA4(TIU4コンペアマッチ/インプットキャプチャA4)		*/
	PowerON_Reset,						/* 41: IMIB4(TIU4コンペアマッチ/インプットキャプチャB4)		*/
	PowerON_Reset,						/* 42: OVI4(TIU4オーバーフロー4)							*/

	PowerON_Reset,						/* 43: Reserve		*/

	PowerON_Reset,						/* 44: DEND0A(DMAC0A 転送完了)		*/
	PowerON_Reset,						/* 45: DEND0B(DMAC0B 転送完了)		*/
	PowerON_Reset,						/* 46: DEND1A(DMAC1A 転送完了)		*/
	PowerON_Reset,						/* 47: DEND1B(DMAC1B 転送完了)		*/

	PowerON_Reset,						/* 48: Reserve		*/
	PowerON_Reset,						/* 49: Reserve		*/
	PowerON_Reset,						/* 50: Reserve		*/
	PowerON_Reset,						/* 51: Reserve		*/

	PowerON_Reset,						/* 52: ERI0 (SCI0 受信エラー)				*/
	PowerON_Reset,						/* 53: RXI0 (SCI0 受信完了)					*/
	PowerON_Reset,						/* 54: TXI0 (SCI0 送信データエンプティ)		*/
	PowerON_Reset,						/* 55: TEI0 (SCI0 送信終了)					*/

	PowerON_Reset,						/* 56: ERI1 (SCI1 受信エラー)				*/
	PowerON_Reset,						/* 57: RXI1 (SCI1 受信完了)					*/
	PowerON_Reset,						/* 58: TXI1 (SCI1 送信データエンプティ)		*/
	PowerON_Reset,						/* 59: TEI1 (SCI1 送信終了)					*/

	PowerON_Reset						/* 60: ADI (AD変換完了)		*/
};

#pragma	section	