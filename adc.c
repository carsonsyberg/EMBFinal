#include "msp.h"
#include "adc.h"

void config_adc(void) {

    // Enable ADC interrupt in NVIC module / enable global interrupts
    __enable_interrupt();//
    NVIC->ISER[0] = 1 << ((ADC14_IRQn) & 31);//

// Overall - configure ADC core (ADC14CTL0:1)

    // when ADC140N = 1 reset removed / core ready
    ADC14->CTL0 |= ADC14_CTL0_ON;

// HOW TO DECIDE WHAT CLOCK / DIVISORS WE NEED??
    // select clock source with ADC14SSELx bit
    ADC14->CTL0 |= BIT3 << 19;
        // divide clock if necessary with ADC14PDIV and ADC14DIV bits

// HOW TO DECIED WHAT RESOLUTION TO USE??
    // select resolution with ADC14RES bits in ADC14CTL1
    //ADC14->RES &= 0b01; // 10 bit (11 clock cycle conversion time)

// HOW TO DECIDE WHAT TO MAKE THESE?
    // select source for SH1 (sample input signal, measurements happen on its rising edge) w/ SHSx bits
    ADC14->CTL0 = ADC14_CTL0_SHT0__192 | ADC14_CTL0_SHP;      // Sampling time, S&H=16

    ADC14->MCTL[0] = ADC14_MCTLN_INCH_0;    // ref+=AVcc, channel = A0
    ADC14->MCTL[1] = ADC14_MCTLN_INCH_1 | ADC14_MCTLN_EOS;    // ref+=AVcc, channel = A1 and end of sequence

    ADC14->IER0 = ADC14_IER0_IE1;           // Enable ADC14IFG.1
    SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;   // Wake up on exit from ISR

    // Ensures SLEEPONEXIT takes effect immediately
        __DSB();


    // 22.2.7 Conversion Memory / Channels Stuff
    //select conversion mode repeat-sequence-of-channels
    //result stored into ADC14MEMx and ADC14IFG.x set after successful conversion
    //use multiple sample and convert function (ADC14MSC) bit 22.2.8.5
}

