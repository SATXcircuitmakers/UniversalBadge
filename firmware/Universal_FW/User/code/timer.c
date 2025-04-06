/*

*/

#include "ch32x035.h"


void timers_init()
{
    TIM_TimeBaseInitTypeDef tim = {0};
    TIM_OCInitTypeDef tim_oc = {0};

    // configure timers
    tim.TIM_Period = (1 << 10) - 1;
	tim.TIM_Prescaler = 0;
	tim.TIM_ClockDivision = TIM_CKD_DIV1;
	tim.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, &tim);
    TIM_TimeBaseInit(TIM2, &tim);
    TIM_TimeBaseInit(TIM3, &tim);

    // enable pwm channels
    tim_oc.TIM_OCMode = TIM_OCMode_PWM1;
    tim_oc.TIM_OutputState = TIM_OutputState_Enable;
	tim_oc.TIM_Pulse = 0;
	tim_oc.TIM_OCPolarity = TIM_OCPolarity_High;
	
    TIM_OC1Init(TIM1, &tim_oc);
    TIM_OC1Init(TIM2, &tim_oc);
    TIM_OC1Init(TIM3, &tim_oc);

    TIM_OC2Init(TIM1, &tim_oc);
    TIM_OC2Init(TIM2, &tim_oc);

    TIM_OC3Init(TIM1, &tim_oc);
    TIM_OC3Init(TIM2, &tim_oc);

    TIM_OC4Init(TIM1, &tim_oc);
    TIM_OC4Init(TIM2, &tim_oc);
}

void timers_off()
{
    TIM_Cmd(TIM1, DISABLE);
    TIM_Cmd(TIM2, DISABLE);
    TIM_Cmd(TIM3, DISABLE);
}

void timers_on()
{
    TIM1->CNT = TIM2->CNT = TIM3->CNT = 0;
    
    TIM_Cmd(TIM1, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
    TIM_Cmd(TIM3, ENABLE);
}
