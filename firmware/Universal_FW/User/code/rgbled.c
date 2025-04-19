/*

*/

#include "rgbled.h"


const struct SelPin sel[SEL_COUNT] = {
    {.port = SEL1_PORT, .pin = SEL1_PIN},
    {.port = SEL2_PORT, .pin = SEL2_PIN},
    {.port = SEL3_PORT, .pin = SEL3_PIN},
    {.port = SEL4_PORT, .pin = SEL4_PIN},
};

Rgb rgb[LED_COUNT];

uint8_t map[SEL_COUNT][LED_PER_SEL] = {
    {0, 2, 4}, {1, 3, 5},
    {6, 8, 10}, {7, 9, 11}
};

static uint8_t sel_idx = 0;



void matrix_next()
{
    uint8_t i;
    
    sel_idx++;
    sel_idx %= SEL_COUNT;

    for (i = 0; i < SEL_COUNT; i++) {
        sel[i].port->BSHR = sel[i].pin;
    }

    TIM1->CH4CVR = rgb[map[sel_idx][0]].b;
    TIM1->CH3CVR = rgb[map[sel_idx][0]].r;
    TIM1->CH2CVR = rgb[map[sel_idx][0]].g;

    TIM1->CH1CVR = rgb[map[sel_idx][1]].b;
    TIM2->CH4CVR = rgb[map[sel_idx][1]].r;
    TIM2->CH3CVR = rgb[map[sel_idx][1]].g;

    TIM2->CH2CVR = rgb[map[sel_idx][2]].b;
    TIM2->CH1CVR = rgb[map[sel_idx][2]].r;
    TIM3->CH1CVR = rgb[map[sel_idx][2]].g;

    TIM1->CNT = TIM2->CNT = TIM3->CNT = 0;

    sel[sel_idx].port->BCR = sel[sel_idx].pin;
}