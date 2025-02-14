/*****************************************************************************************************************
/*    FILE        : itask_motor.c                                                                                    */
/*    DATE        :Tue, Jan 17, 2012                                                                                */
/*    DESCRIPTION :AGVの走行管理割り込みタスク                                                                    */
/*    CPU TYPE    :H8/3052F                                                                                        */
/****************************************************************************************************************/
#include    "machine.h"                                /* 組込み関数の定義                                        */
#include    "reg3052.h"                                    /* H8/3052Fの内蔵モジュール定義                            */
#include    "agvdef.h"                                    /* 無人搬送車のデバイス定義                                */
#include    "agvvars.h"                                    /* 無人搬送車の共通変数を定義                            */
#include    "def_monitor_printf.h"
#include    "Acc_Table.h"                                /* 加速テーブルの読み出し                                */
/****************************************************************************************************************/
/*    走行モータ制御タスク itask_motor                                                                            */
/****************************************************************************************************************/
/***********************************************************************************************/
/*
この関数では, 
1. モータの回転状態を変化する.
2. モータの状態を制御する.
3. モータの状態をもとに, 加速テーブルとGRA1を用いた, 割り込みするタイミングを変化する.  
   つまり, itask_motorが呼び出されるタイミングを変化することでモータの回転はじめを変える. 
*/
unsigned char frag_state_motor;
unsigned char frag_motor_state_motor;

#pragma interrupt itask_motor
void itask_motor(void)
{

    int static pointer = 0;
	frag_state_motor = AGV_STATE;
	frag_motor_state_motor = MOTOR_STATE;
    /*
    このタスクが起動されたときに、MOTOR_STATEが「MOTOR_STOP以外の条件」ではモータの励磁出力を順番に出力する
    つまり, モータを回転する
    */
    if (MOTOR_STATE != MOTOR_STOP) {
        //return MOTOR_STATE;
        bios_step_output();
        //printf("モータの状態は%u", MOTOR_STATE);
    }
    else {
        MOTOR_STATE = MOTOR_STOP;
    }


    switch (MOTOR_STATE) {
    //モータ停止状態のとき
    case MOTOR_STOP:
        pointer = 0; /*ポインタ変数を初期化*/
        GRA1 = ACC_TABLE[pointer]; /*加速テーブルの先頭の値をGRA1にセットする.*/
        MOTOR_STATE = MOTOR_ACC; /*モータの状態を加速状態に遷移する*/
        break;

    //モータが加速状態の時
    case MOTOR_ACC: {
        /*
        加速テーブルのポインタを現在の状態からインクリメントする
        つまり, 割り込み周期が変わっている. つまり, itask_motorの呼び出す周期が変わる. 
        */
		
        pointer++;
        GRA1 = ACC_TABLE[pointer]; 

        /*テーブルポインタが(テーブル数-1)と等しくなったときモータが定速状態になる*/
        if (pointer == 400) {
            MOTOR_STATE = MOTOR_CONST;
        }
        /*加速中にAGV_STOP_WAITあるいはAGV_RUN_ALMになれば減速状態になる*/
        if ((AGV_STATE == AGV_STOP_WAIT) || (AGV_STATE == AGV_RUN_ALM)) {
            MOTOR_STATE = MOTOR_BREAK;
        }
        break;
    }

    /*モータが定速状態の時*/
    case MOTOR_CONST: {
        /*定速中にAGV_STOP_WAITあるいはAGV_RUN_ALMになれば減速状態になる*/
        if ((AGV_STATE == AGV_STOP_WAIT) || (AGV_STATE == AGV_RUN_ALM)) {
            MOTOR_STATE = MOTOR_BREAK;
        }
        break;
    }

    /*モータが減速状態の時*/
    case MOTOR_BREAK: {
        //itask_motorの呼び出す速度を遅くする. 
        pointer--; /*ポインタの値をデクリメント*/
        GRA1 = ACC_TABLE[pointer]; /*GRA1にテーブルの値をセット*/
        /*GRA1の値が999になるとMOTOR_STATEを停止状態にする*/
        if (GRA1 == 999) {
            MOTOR_STATE = MOTOR_STOP;
            /*ITUの動作を停止する*/
            TSTR &= ~0x02; 
        }
		//減速状態のまま走行状態に移行すると加速する
		//もし現在の速度GRA1が51より下だったら加速
		//pointerが200以下の条件を追加
		//if((AGV_STATE == AGV_RUN) && (GRA1 < 51)){
		//	MOTOR_STATE = MOTOR_ACC;
		//	}
		//if ((AGV_STATE == AGV_RUN) && (from_run_alm == 1) && (GRA1 > 100)){
		//		MOTOR_STATE = MOTOR_ACC;
		//		from_run_alm = 0;
		//		}
		
		//else if((AGV_STATE == AGV_RUN) && (from_run_alm == 1)){
		//		from_run_alm = 0;
		//		}
		
        break;
    }

    default:
        break;
    }
    //printf("モータの状態は%u", MOTOR_STATE); //テスト用
	
	//テスト用
	
		if ((frag_state_motor != AGV_STATE) || (frag_motor_state_motor != MOTOR_STATE)){
		printf("AGV_STATE:%d , ", AGV_STATE);
		printf("MOTOR_STATE:%d\n" , MOTOR_STATE);
		}
	
	
	
	
	//ITU1の割り込みフラグをクリア
	TSR1 &= ~0x01;
}
