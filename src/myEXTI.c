#include <stdio.h>
#include "diag/Trace.h"
#include "cmsis/cmsis_device.h"
#include "myEXTI.h"
#include "globals.h"

void myEXTI_Init()
{
    /* Map EXTI0 line to PA0 */
    SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0;   // Clear EXTI0 bits
    SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA; // Map PA0 to EXTI0
    /* Map EXTI2 line to PB2 */
    SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI2;   // Clear EXTI2 bits
    SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI2_PB; // Map PB2 to EXTI2
    /* Map EXTI3 line to PB3 */
    SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI3;   // Clear EXTI3 bits
    SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PB; // Map PB3 to EXTI3

    /* Configure rising-edge trigger for EXTI0, EXTI2, and EXTI3 */
    EXTI->RTSR |= (EXTI_RTSR_TR0 | EXTI_RTSR_TR2 | EXTI_RTSR_TR3);
    /* Unmask interrupts for EXTI0, EXTI2, and EXTI3 */
    EXTI->IMR |= (EXTI_IMR_MR0 | EXTI_IMR_MR2 | EXTI_IMR_MR3);

    /* Enable NVIC interrupts:
    - EXTI0_1_IRQn handles EXTI0 and EXTI1
    - EXTI2_3_IRQn handles EXTI2 and EXTI3
    */
    NVIC_SetPriority(EXTI0_1_IRQn, 0);
    NVIC_EnableIRQ(EXTI0_1_IRQn);
    NVIC_SetPriority(EXTI2_3_IRQn, 0);
    NVIC_EnableIRQ(EXTI2_3_IRQn);
}

void EXTI2_3_IRQHandler()
{
    if ((EXTI->PR & EXTI_PR_PR2) != 0 && fivefivefive == 0)
    // rising edge on PB2
    {
        if (timerTriggered == 0) // first edge detected
        {
            tim2Overflow = 0;         // clear overflow counter
            TIM2->CNT = 0;            // reset counter
            TIM2->CR1 |= TIM_CR1_CEN; // start timer
            timerTriggered = 1;       // mark timer running
        }
        else // second edge detected
        {
            TIM2->CR1 &= ~(TIM_CR1_CEN); // stop timer
            /* Combine overflow counter and current CNT for full 64-bit cycle count */
            uint64_t cycles = ((uint64_t)tim2Overflow << 32) + (uint64_t)TIM2->CNT;
            // Avoid divide-by-zero
            uint32_t freq_hz = (cycles ? (uint32_t)((SystemCoreClock + (cycles / 2)) /
                                                    cycles)
                                       : 0);
            /* small hysteresis to reduce display oscillation at low frequencies */
            int diff = (freq_hz > lastFrequency) ? (freq_hz - lastFrequency) : (lastFrequency - freq_hz);
            if (diff > 2) // only update if change > 2 Hz
            {
                frequency = freq_hz;
                lastFrequency = freq_hz;
            }
            timerTriggered = 0; // ready for next
        }
        EXTI->PR |= EXTI_PR_PR2; // clear EXTI2 pending flag
    }

    if ((EXTI->PR & EXTI_PR_PR3) != 0 && fivefivefivefive == 1) // rising edge on PB3
    {
        if (timerTriggered == 0) // first edge detected
        {
            tim2Overflow = 0;         // clear overflow counter
            TIM2->CNT = 0;            // reset counter
            TIM2->CR1 |= TIM_CR1_CEN; // start timer
            timerTriggered = 1;       // mark timer running
        }
        else // second edge detected
        {
            TIM2->CR1 &= ~(TIM_CR1_CEN); // stop timer
            uint64_t cycles = ((uint64_t)tim2Overflow << 32) + (uint64_t)TIM2->CNT;
            uint32_t freq_hz = (cycles ? (uint32_t)((SystemCoreClock + (cycles / 2)) /
                                                    cycles)
                                       : 0);
            int diff = (freq_hz > lastFrequency) ? (freq_hz - lastFrequency) : (lastFrequency - freq_hz);
            if (diff > 2)
            {
                frequency = freq_hz;
                lastFrequency = freq_hz;
            }
            timerTriggered = 0; // ready for next
        }
        EXTI->PR |= EXTI_PR_PR3; // clear EXTI3 pending flag
    }
}

void EXTI0_1_IRQHandler()
{
    if ((EXTI->PR & EXTI_PR_PR0) != 0) // rising edge on PA0
    {
        if (GPIOA->IDR & GPIO_IDR_0) // confirm PA0 is high
        {
            trace_printf("Switching Freq source \n");
            if (fivefivefive == 0)
            {
                fivefivefive = 1;
            }
            else
            {

                fivefivefive = 0;
            }
            TIM2->CR1 &= ~(TIM_CR1_CEN); // ensure timer stopped
            TIM2->CNT = 0;               // reset counter
            timerTriggered = 0;          // ensure next edge is treated as first
        }
        EXTI->PR |= EXTI_PR_PR0; // clear EXTI0 pending flag
    }
}