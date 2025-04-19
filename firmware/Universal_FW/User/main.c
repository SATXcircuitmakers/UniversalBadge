/*
    universal badge
    firmware framework by true
 
    TODO:
     - set up systick timer interrupt
     - set up button handling code
     - program an example rainbow spew 
*/


#include "ch32x035.h"

#include "code/timer.h"
#include "code/rgbled.h"



uint8_t rgb_prog_run = 0;



// various hardware init
static inline void clock_init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                            RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO |
                            RCC_APB2Periph_TIM1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3, ENABLE);                                                  
}

static inline void gpio_init()
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
    GPIOA->BSHR = GPIOB->BSHR = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_Init(GPIOA, &gpio);
    GPIO_Init(GPIOB, &gpio);    

    // Unused bonded pin
    gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    gpio.GPIO_Pin = GPIO_Pin_5;
    GPIO_Init(GPIOB, &gpio);
}

void systick_init()
{
    SysTick->SR   = 0;
    SysTick->CNT  = 0;
    SysTick->CMP  = (SystemCoreClock / 1024) - 1;
    SysTick->CTLR = 0xF;

    NVIC_SetPriority(SysTick_IRQn, 0);
    NVIC_EnableIRQ(SysTick_IRQn);
}


// here we go
int main(void)
{
    // configure system
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();

    // configure base hardware
    clock_init();
    gpio_init();

    // configure peripherals
    timers_init();
    
    // finally, start our output
    timers_on();

    // and start our main timer
    systick_init();

    while (1) {
        // low-priority tasks
        if (rgb_prog_run) {
            // run program
            rgb_prog_run = 0;

            for (uint8_t i = 0; i < 12; i++) {
                rgb[i].r = 100;
                rgb[i].g = 100;
                rgb[i].b = 100;
            }

            rgb[6].b = 800;
        }
    }
}

volatile uint32_t uptime = 0;
uint16_t tick = 0;

__attribute__((interrupt("WCH-Interrupt-fast")))
void SysTick_Handler(void) 
{
    // update LEDs - very important to do this first
    matrix_next();

    // do other stuff
    tick++;
    tick &= 0x3ff;
    if (!tick) {
        uptime++;
    }

    if (tick & 0xf) {
        rgb_prog_run = 1;
    }

    SysTick->SR = 0;
}
