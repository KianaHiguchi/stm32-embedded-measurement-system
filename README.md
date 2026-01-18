# STM32 Embedded Measurement System

An embedded measurement system built on an STM32 Cortex-M0 microcontroller that integrates analog and digital peripherals to measure resistance and signal frequency in real time.

## Overview 
The system samples analog input from a potentiometer, maps it to an output voltage via a DAC, and computes frequency from multiple signal sources using timers and external interrupts. Results are displayed on an SPI-driven OLED interface.

## Features 
- Real-time resistance measurment using ADC input
- Analog voltage output via DAC to drive an external timing circuit
- Frequency measurement using timers and rising-edge interrupts
- Runtime switching between multiple frequency sources
- SPI-driven OLED display for live system output

## Technical Highlights 
- Register-level configuration of STM32 peripherals
- Modular firmware design with peripheral-specific drivers
- Use of timers and external interrupts for precise timing

## Notes
See Lab Report under 'report' for full project breakdown. Source code modular and separated into .c and .h files.