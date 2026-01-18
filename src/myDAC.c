#include "myDAC.h"
#include "globals.h"
#include <stdio.h>
#include "diag/Trace.h"
#include "cmsis/cmsis_device.h"

void myDAC_Init(void)
{
    /* Enable clock for DAC peripheral */
    RCC->APB1ENR |= RCC_APB1ENR_DACEN;
    /* Configure DAC: output buffer enabled by default */
    DAC->CR &= ~(0b111); /* DAC channel1 trigger disabled(bit2) and DAC channel1 output
    buffer enabled (bit1) and DAC channel1 disabled (bit0) */
    DAC->CR |= 0b001;    /* DAC channel1 enable (bit0) */
}

void myDAC_SetValue(unsigned int value)
{
    /* Set the 12-bit right-aligned data for DAC channel 1 */
    DAC->DHR12R1 = value & 0x0FFF; /* Ensure only lower 12 bits are used */
}