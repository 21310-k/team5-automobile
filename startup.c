/************************************************************************/
/*  FILE        :startup.c												*/
/*	DATE        :Tue, Jan 17, 2012										*/
/*  DESCRIPTION :Reset Program											*/
/*  CPU TYPE    :H8/3052F												*/
/************************************************************************/
#include	"stacksct.h"

extern void main(void);				/* main関数は外部宣言である					*/

#pragma	entry PowerON_Reset			/* PowerON_Reset関数からプログラム開始　	*/


void  PowerON_Reset(void)
{ 
	_INITSCT();						/* データ処理領域の初期化関数(ライブラリ)	*/

	main();							/* main関数の呼出し							*/
}

