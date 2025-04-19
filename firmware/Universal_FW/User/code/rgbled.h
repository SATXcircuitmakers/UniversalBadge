/*

*/


#ifndef __CODE_RGBLED_H
#define __CODE_RGBLED_H



#include <stdint.h>
#include "ch32x035.h"


#define LED_COUNT   12
#define SEL_COUNT   4
#define LED_PER_SEL 3

#define SEL1_PORT   GPIOA       // top row
#define SEL1_PIN    GPIO_Pin_7

#define SEL2_PORT   GPIOA
#define SEL2_PIN    GPIO_Pin_6

#define SEL3_PORT   GPIOB       // bottom
#define SEL3_PIN    GPIO_Pin_6

#define SEL4_PORT   GPIOB
#define SEL4_PIN    GPIO_Pin_7




struct SelPin {
    GPIO_TypeDef *port;
    uint16_t pin;
};

typedef struct Rgb {
    uint16_t r;
    uint16_t g;
    uint16_t b;
} Rgb;



extern Rgb rgb[LED_COUNT];



void matrix_next();



#endif /* __CODE_RGBLED_H */
