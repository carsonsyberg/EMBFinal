#include "msp.h"
#include <stdio.h>
#include "uart.h"
#include "adc.h"
#include "gpio.h"

volatile uint16_t sensorIN = 255;
volatile uint16_t sensorOUT = 255;

void main() {

    WDT_A->CTL = WDT_A_CTL_PW |             // Stop watchdog timer
                 WDT_A_CTL_HOLD;

    config_adc();
    config_gpio();
    config_uart();

    int numPeople = 0;
    uint8_t state = 0;

    while(1) {

        if(state == 0) {
            // while IR1 and IR2 at wall distance
                // continue sampling in ADC until one is not
                // if IR1 state = 1 if IR2 state = 2
        }
        else if(state == 1) {
            // while IR2 at wall distance
                // continue sampling in ADC until IR2 is not
                // go to state 3
        }
        else if(state == 2) {
            // while IR1 at wall distance
                // continue sampling in ADC until IR1 is not
                // go to state 4
        }
        else if(state == 3) {
            numPeople = numPeople + 1;
            state = 5;
        }
        else if(state == 4) {
            numPeople = numPeople - 1;
            state = 5;
        }
        else if(state == 5) {
            write_data(numPeople); // send new number of people to ATMega for displaying
            // read_data() to get success from ATMega
            state = 0;
        }
        else
            state = 0;



    }
}

// ADC14 interrupt service routine
void ADC14_IRQHandler()
{
    if(ADC14->IFGR0 & ADC14_IFGR0_IFG1)
    {
        sensorIN = ADC14->MEM[0];   // Move A0 results
        sensorOUT = ADC14->MEM[1];   // Move A1 results, IFG is cleared
        __no_operation();                   // Set Breakpoint1 here
    }
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
