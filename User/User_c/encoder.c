//该文件用于将使用编码器采集到的数据转化为速度，并与期望速度进行比较，计算电机偏差值，并将相应数据录入电机偏差结构体中

#include "encoder.h"

void speedout(Road road,SPEED_now* speed_now,SPEED_state* speed_state)//根据路况选择目标速度并将其赋给当前速度结构体
{
	switch(road)
	{
		case(Straight)://直道
		speed_now-> speed_L_ai = speed_state->Strai_speed_L_ai;
		speed_now-> speed_R_ai = speed_state->Strai_speed_R_ai;
		break;
		
		case(Curve_Left)://左弯道
		speed_now-> speed_L_ai = speed_state->Cur_L_speed_L_ai;
		speed_now-> speed_R_ai = speed_state->Cur_L_speed_R_ai;
		break;
		
		case(Curve_Right)://右弯道
		speed_now-> speed_L_ai = speed_state->Cur_R_speed_L_ai;
		speed_now-> speed_R_ai = speed_state->Cur_R_speed_R_ai;
		break;
		case(Ring_Left): //环左
		speed_now-> speed_L_ai = speed_state->Ring_L_speed_L_ai;//左轮
		speed_now-> speed_R_ai = speed_state->Ring_L_speed_R_ai;//右轮
			break;
		
		case(Ring_Right): //环右
		speed_now-> speed_L_ai = speed_state->Ring_R_speed_L_ai;//左轮
		speed_now-> speed_R_ai = speed_state->Ring_R_speed_R_ai;//右轮
			break;
		
		case(Ring_In): //进大圆环
		speed_now-> speed_L_ai = speed_state->Ringin_speed_L_ai;
		speed_now-> speed_R_ai = speed_state->Ringin_speed_R_ai;
			break;
		
		case(Ring_Out): //出大圆环
		speed_now-> speed_L_ai = speed_state->Ringout_speed_L_ai;
		speed_now-> speed_R_ai = speed_state->Ringout_speed_R_ai;
			break;
		case(OutGarage):
		speed_now-> speed_L_ai = speed_state->Outgar_speed_L_ai;
		speed_now-> speed_R_ai = speed_state->Outgar_speed_R_ai;
			break;
		case(Stop):
		speed_now-> speed_L_ai =0;
		speed_now-> speed_R_ai =0;
			break;
		case(InGarage):
		speed_now-> speed_L_ai =70;
		speed_now-> speed_R_ai =100;
		  break;
		default://别的默认
	  speed_now-> speed_L_ai = speed_state->Strai_speed_L_ai;
		speed_now-> speed_R_ai = speed_state->Strai_speed_R_ai;
			break;
	}
}

void speed_cal(FOOT_COUNTER *foot_counter, SPEED_now* speed_now)//通过编码器的数据计算速度，并将其写入当前速度结构体
{
	int Speed_L = 0;
	int Speed_R = 0;
	int dat_L = 0 ;
	int dat_R = 0 ;
	if(DIR_L == 1)//左轮速度
		{
			dat_L = ctimer_count_read(Encoder_L);
		//	Speed_L= MFBL*(dat_L/(CONTROL_T*DECO));//用编码器在中断间隔之内的读数除以执行程序的中断间隔及一米对应的脉冲数来表示当前速度(m/s),已乘PID分辨率
			//Speed_L = dat_L; //测试用
			//滤波
		//	Speed_L = Speed_L>1?Speed_L:1;
			Speed_L = dat_L;
		}
		else
		{
			dat_L = ctimer_count_read(Encoder_L) * -1;
		 // Speed_L = (MFBL*(dat_L/(CONTROL_T*DECO))) * -1;//用编码器在中断间隔之内的读数除以执行程序的中断间隔及一米对应的脉冲数来表示当前速度(m/s),已乘PID分辨率
			//Speed_L = dat_L;
			//Speed_L = Speed_L>1?Speed_L:1;
			Speed_L = dat_L;
		}

		speed_now->speed_L = Speed_L;
		
		
	if(DIR_R == 0)//右轮速度
		{
			dat_R = ctimer_count_read(Encoder_R);
		//	Speed_R = MFBL*(dat_R/(CONTROL_T*DECO));//用编码器在中断间隔之内的读数除以执行程序的中断间隔及一米对应的脉冲数来表示当前速度(m/s),已乘PID分辨率
			//Speed_R = dat_R;
		//	Speed_R = Speed_R>1?Speed_R:1;
			Speed_R = dat_R;
		}
		else
		{
			dat_R = ctimer_count_read(Encoder_R) * -1;
		//	Speed_R = (MFBL*(dat_R/(CONTROL_T*DECO))) * -1;//用编码器在中断间隔之内的读数除以执行程序的中断间隔及一米对应的脉冲数来表示当前速度(m/s),已乘PID分辨率
			//Speed_R = dat_R;
		//	Speed_R = Speed_R>1?Speed_R:1;
			Speed_R = dat_R;
		}
		//******计********步************
		//计数器1
		if(foot_counter->speed_counter0_EN==1){
				foot_counter->speed_counter0_0+=Speed_R;	
				if(foot_counter->speed_counter0_0>=10000){
					foot_counter->speed_counter0_1++;
					foot_counter->speed_counter0_0=0;
				}
				if(foot_counter->speed_counter0_1>=10000){
					foot_counter->speed_counter0_2++;
					foot_counter->speed_counter0_1=0;
				}
				if(foot_counter->speed_counter0_2>=10000){
					foot_counter->speed_counter0_2=0;
				}
		}
		else{
		foot_counter->speed_counter0_EN=0;
		foot_counter->speed_counter0_0=0;
		foot_counter->speed_counter0_1=0;
		foot_counter->speed_counter0_2=0;
		}
		//计数器2
		if(foot_counter->speed_counter1_EN==1){
				foot_counter->speed_counter1_0+=Speed_R;	
				if(foot_counter->speed_counter1_0>=10000){
					foot_counter->speed_counter1_1++;
					foot_counter->speed_counter1_0=0;
				}
				if(foot_counter->speed_counter1_1>=10000){
					foot_counter->speed_counter1_2++;
					foot_counter->speed_counter1_1=0;
				}
				if(foot_counter->speed_counter1_2>=10000){
					foot_counter->speed_counter1_2=0;
				}
		}
		else{
		foot_counter->speed_counter1_EN=0;
		foot_counter->speed_counter1_0=0;
		foot_counter->speed_counter1_1=0;
		foot_counter->speed_counter1_2=0;
		}
		

		//******计********步************
		speed_now->speed_R = Speed_R;
		ctimer_count_clean(Encoder_R);
		ctimer_count_clean(Encoder_L);
}


void calculate_err_m(SPEED_now* speed_now,Err_Motor* err_Mot)//通过左轮和右轮速度计算所需的增量式PID误差值，将其写入电机误差结构体，并更新相应的所需误差值
{
	err_Mot->err_L_m = speed_now->speed_L_ai - speed_now->speed_L;
	err_Mot->err_R_m = speed_now->speed_R_ai - speed_now->speed_R;
	
	err_Mot->err_derivative_L_m = err_Mot->err_L_m - err_Mot->err_last_L_m;//本次误差与上次误差之差
	err_Mot->err_derivative_R_m = err_Mot->err_R_m - err_Mot->err_last_R_m;
	
	err_Mot->err_derivative2_L_m = err_Mot->err_last_L_m - err_Mot->err_past_L_m;//上次误差和上上次误差之差
	err_Mot->err_derivative2_R_m = err_Mot->err_last_R_m - err_Mot->err_past_R_m;
	
	err_Mot->err_past_L_m = err_Mot->err_last_L_m;
	err_Mot->err_last_L_m = err_Mot->err_L_m;//左轮数据更新
	
	err_Mot->err_past_R_m = err_Mot->err_last_R_m;
	err_Mot->err_last_R_m = err_Mot->err_R_m;//右轮数据更新
	
}

