/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2023/12/26
 * Description        : Main program body.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

/*
 *@Note
 *GPIO routine:
 */

#include "ch32x035.h"

#include "code/timer.h"



void clock_init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                            RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO |
                            RCC_APB2Periph_TIM1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3, ENABLE);                                                  
}

void gpio_init()
{
    GPIO_InitTypeDef gpio = {0};

    gpio.GPIO_Speed = GPIO_Speed_50MHz;

    // TIM3 needs to remap CH1 to PB6, T1C4 needs to remap to PB12
    AFIO->PCFR1 |= 0x200000 | 0x10000;

    // T2C1-C4
    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_Init(GPIOA, &gpio);

    // T3C1, T1C1-C4
    gpio.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_Init(GPIOB, &gpio);
    
    // RGB1, RGB2 (unused)
    gpio.GPIO_Mode = GPIO_Mode_IPU;
    gpio.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_Init(GPIOA, &gpio);

    // RGB5, RGB7, RGB6, RGB10 (unused)
    gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_8;
    GPIO_Init(GPIOB, &gpio);

    // Buttons, active low
    gpio.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOC, &gpio);

    // RGB3, RGB4 (top bar selects)
    // RGB8, RGB9 (bottom bar selects)
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIOA->BSHR = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_Init(GPIOA, &gpio);
    GPIO_Init(GPIOB, &gpio);    

    // Unused bonded pin
    gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    gpio.GPIO_Pin = GPIO_Pin_5;
    GPIO_Init(GPIOB, &gpio);
}

uint8_t rgb[12][3];

const uint8_t rgb_map[] = {0};

/*
    TODO:
    - set up systick timer interrupt
    - set up button handling code
    - set up the matrix
    - program an example rainbow spew
*/

/* Global define */

/* Global Variable */

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();

    clock_init();
    gpio_init();

    // configure peripherals
    timers_init();

    // temporary
    timers_on();
    TIM1->CH1CVR = 512;
    TIM1->CH2CVR = 0;
    TIM1->CH3CVR = 1000;

    volatile uint32_t i = 0;
    do {
        i = 20000;
        while (i--);

        TIM1->CH4CVR++;
        if (TIM1->CH4CVR > 1000) {
            TIM1->CH4CVR = 0;
        }
    }

    while(1);
}
