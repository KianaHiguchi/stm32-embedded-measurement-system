#include "myADC.h"
#include "globals.h"
#include <stdio.h>
#include "diag/Trace.h"
#include "cmsis/cmsis_device.h"

void myADC_Init(void)
{
    /* Enable clock for ADC1 peripheral */
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    /* Configure ADC1: 12-bit resolution, right data alignment, single conversion mode */
    ADC1->CFGR1 |= (0b11 << 12); /* Continuous conversion mode(bit13) and Overrun management mode (bit12) */
    ADC1->CFGR1 &= ~(0b111000);  /* Right alignment(bit5) and 12 bits resolution (bit3+4) */
    ADC1->CHSELR |= 0b10;        /* ADC channel selection register */
    ADC1->SMPR |= 0b111;         /* Sampling time selection -> 1.5 ADC clock cycles */
    ADC1->CR |= 0b1;             /* start ADC conversion */

    while ((ADC1->ISR & 0b1) == 0)
        ; /* wait for ADC to ‘warm up’ */
}

void myADC_StartConversion(void)
{
    ADC1->CR |= 0b100;
    while ((ADC1->ISR & 0b100) == 0)
        ;
    resistance = ADC1->DR;
}