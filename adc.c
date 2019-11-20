#include "msp.h"
#include "adc.h"

void config_adc(void) {

    // Enable ADC interrupt in NVIC module
    __enable_interrupt();
    NVIC_ISER0 = 1 << ((INT_ADC14 - 16) & 31);

// Overall - configure ADC core (ADC14CTL0:1)

    // when ADC140N = 1 reset removed / core ready
    ADC14->ADC14CTL0 |= BIT4;

    // select clock source with ADC14SSELx bit
    ADC14->ADC14CTL0 |= BIT3 << 19;
        // divide clock if necessary with ADC14PDIV and ADC14DIV bits

    // disable digital parts of pins P7.6 P7.7 w/ PySELx bits

    // select regular-power mode with ADC14PWRMD
    // supports sampling rates up to 1 Msps
    ADC14->ADC14CTL1 &= 0b00;

    // select resolution with ADC14RES bits in ADC14CTL1
    ADC14->ADC14RES &= 0b01; // 10 bit (11 clock cycle conversion time)

    // select source for SH1 (sample input signal, measurements happen on its rising edge) w/ SHSx bits
    ADC14CTL0 = ADC14SHT0_2 | ADC14SHP;      // Sampling time, S&H=16
    // select sample mode or pulse mode w/ ADC14SHP bit
    // get converted data out from ADC14MEMx register
    // ADC14IFG.x interrupt is set after measurement

    //22.2.6.1 Sampling Stuff

    // 22.2.7 Conversion Memory / Channels Stuff

    //select conversion mode repeat-sequence-of-channels

    //result stored into ADC14MEMx and ADC14IFG.x set after successful conversion

    //use multiple sample and convert function (ADC14MSC) bit 22.2.8.5
}
