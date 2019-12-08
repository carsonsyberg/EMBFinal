#include "msp.h"
#include "adc.h"

void config_adc(void) {

    // Enable ADC interrupt in NVIC module / enable global interrupts
    NVIC->ISER[0] = 1 << ((ADC14_IRQn) & 31);//

// Overall - configure ADC core (ADC14CTL0:1)

    // when ADC140N = 1 reset removed / core ready
    ADC14->CTL0 |= BIT4;

/* Using Default Clock
    // select clock source with ADC14SSELx bit
    ADC14->CTL0 |= BIT3 << 19;
    // divide clock if necessary with ADC14PDIV and ADC14DIV bits
*/

/* Using Default Resolution
    // select resolution with ADC14RES bits in ADC14CTL1
    //ADC14->RES &= 0b01; // 10 bit (11 clock cycle conversion time)
    0000010000000110000001111000b
*/

    // selects extended sampling time
    ADC14->CTL0 |= ADC14_CTL0_SHT0__192 | ADC14_CTL0_SHP | ADC14_CTL0_CONSEQ_3 | ADC14_CTL0_MSC;

    ADC14->MCTL[0] = ADC14_MCTLN_INCH_0;    // ref+=AVcc, channel = A0
    ADC14->MCTL[1] = ADC14_MCTLN_INCH_1 | ADC14_MCTLN_EOS;    // ref+=AVcc, channel = A1 and end of sequence

    ADC14->IER0 = ADC14_IER0_IE1;           // Enable ADC14IFG.1

}

