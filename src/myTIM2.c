#include <stdio.h>
#include "diag/Trace.h"
#include "cmsis/cmsis_device.h"
#include "myTIM2.h"
#include "globals.h"

void myTIM2_Init()
{
    /* Enable clock for TIM2 peripheral */
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    /* Configure TIM2: buffer auto-reload, count up, stop on overflow, enable update
    events, interrupt on overflow only */
    TIM2->CR1 = ((uint16_t)0x008C);
    TIM2->PSC = myTIM2_PRESCALER; /* Set clock prescaler value */
    TIM2->ARR = myTIM2_PERIOD;    /* Set auto-reloaded delay */

    /* Update timer registers */
    TIM2->EGR = ((uint16_t)0x0001);
    /* Assign TIM2 interrupt priority = 0 in NVIC */
    NVIC_SetPriority(TIM2_IRQn, 0);
    /* Enable TIM2 interrupts in NVIC */
    NVIC_EnableIRQ(TIM2_IRQn);
    /* Enable update interrupt generation */
    TIM2->DIER |= TIM_DIER_UIE;
}

void TIM2_IRQHandler()
{
    if ((TIM2->SR & TIM_SR_UIF) != 0)
    {
        trace_printf("\n*** Overflow! ***\n");
        TIM2->SR &= ~(TIM_SR_UIF); /* Clear update interrupt flag */
        tim2Overflow++;            /* Increment overflow counter */
    }
}