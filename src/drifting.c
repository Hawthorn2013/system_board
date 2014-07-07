#include "includes.h"

void drift_right1(void)//砖地面180度 背景布100度//2>1>5
{
	set_speed_target(70);
	delay_ms(1500);
	set_steer_helm(STEER_HELM_RIGHT);
	delay_ms(400);
	set_speed_target(0);
	
	delay_ms(200);
	set_steer_helm(STEER_HELM_CENTER);
	set_speed_target(40);
	delay_ms(1000);
	set_speed_target(0);
	delay_ms(2000);
	
}

void drift_right5(void)//砖地面180度 背景布100度
{
	set_speed_target(80);
	delay_ms(1500);
	set_steer_helm(STEER_HELM_RIGHT);
	delay_ms(400);
	set_speed_target(0);
	
	delay_ms(200);
	set_steer_helm(STEER_HELM_CENTER);
	set_speed_target(40);
	delay_ms(1000);
	set_speed_target(0);
	delay_ms(2000);
	
}

void drift_right6(void)//砖地面180度 背景布100度
{
	set_speed_target(70);
	delay_ms(1500);
	set_steer_helm(STEER_HELM_RIGHT);
	delay_ms(600);
	set_speed_target(0);
	
	delay_ms(200);
	set_steer_helm(STEER_HELM_CENTER);
	set_speed_target(40);
	delay_ms(1000);
	set_speed_target(0);
	delay_ms(2000);
	
}
void drift_right4(void)//砖地面180度 背景布100度
{
	set_speed_target(80);
	delay_ms(1500);
	set_steer_helm(STEER_HELM_RIGHT);
	delay_ms(200);
	set_speed_target(0);
	delay_ms(800);

	set_steer_helm(STEER_HELM_CENTER);
	set_speed_target(40);
	delay_ms(1000);
	set_speed_target(0);
	delay_ms(2000);
	
}


void drift_right2(void)//
{
	set_speed_target(70);
	delay_ms(800);
	
	set_steer_helm(STEER_HELM_RIGHT);
	set_speed_target(100);
	delay_ms(700);
	set_speed_target(0);
	
	set_steer_helm(STEER_HELM_CENTER);
	delay_ms(200);
	
	set_speed_target(50);
	delay_ms(1000);
	set_speed_target(0);
	delay_ms(2000);
	
}

void drift_right3(void)//砖地面180度
{
	set_speed_target(20);
	delay_ms(500);
	set_speed_target(80);
	set_steer_helm(STEER_HELM_RIGHT);
	delay_ms(400);
	set_speed_target(0);
	set_steer_helm(STEER_HELM_RIGHT-(STEER_HELM_RIGHT-STEER_HELM_CENTER)/2);
	delay_ms(300);
	set_steer_helm(STEER_HELM_CENTER);
	set_speed_target(40);
	
	//delay_ms(100);
	//set_speed_target(50);
	
	delay_ms(100);
	set_speed_target(0);
	/*set_steer_helm(STEER_HELM_CENTER);
	set_speed_target(40);
	delay_ms(1000);
	set_speed_target(0);
	delay_ms(2000);*/
	
}

void drift_test(void)
{
	set_steer_helm(STEER_HELM_RIGHT);
}
void send_speed(void)//max speed=160
{
	WORD j=0x0000;
	BYTE o,s;
	
	j=0xffff&&data_encoder.speed_now;
	o=(BYTE)j;
	s=(BYTE)(j>>8);
	
	serial_port_1_TX(s);
	serial_port_1_TX(o);
}