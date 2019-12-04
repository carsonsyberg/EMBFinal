#include "msp.h"
#include <stdio.h>
#include "uart.h"
#include "adc.h"
#include "gpio.h"

volatile uint16_t sensorIN = 255;
volatile uint16_t sensorOUT = 255;

//*********** LED TESTING CODE  ******************\

 void main() {

    P6->DIR |= BIT0 | BIT1;
    P3->DIR |= BIT2 | BIT3;
    P4->DIR |= BIT1 | BIT3 | BIT4;

    int z, i;
    int delay = 150000;

    while(1) {

        for(i = 9; i >= 0; i--) {
            displayNum(i); //display number
            for(z = 0; z < delay; z++); //delay
        }


    }

}

void turnOff(char letter) {
    if(letter == 'A')
        P4->OUT |= BIT4;
    else if(letter == 'B')
        P4->OUT |= BIT1;
    else if(letter == 'C')
        P4->OUT |= BIT3;
    else if(letter == 'D')
        P3->OUT |= BIT3;
    else if(letter == 'E')
        P6->OUT |= BIT0;
    else if(letter == 'F')
        P3->OUT |= BIT2;
    else
        P6->OUT |= BIT1;
}

void turnOn(char letter) {
    if(letter == 'A')
        P4->OUT &= ~BIT4;
    else if(letter == 'B')
        P4->OUT &= ~BIT1;
    else if(letter == 'C')
        P4->OUT &= ~BIT3;
    else if(letter == 'D')
        P3->OUT &= ~BIT3;
    else if(letter == 'E')
        P6->OUT &= ~BIT0;
    else if(letter == 'F')
        P3->OUT &= ~BIT2;
    else
        P6->OUT &= ~BIT1;
}

void displayNum(int num) {
    if(num == 1) {
        turnOff('A');
        turnOff('B');
        turnOn('C');
        turnOff('D');
        turnOn('F');
        turnOff('G');
        turnOff('E');
    }
    else if(num == 2) {
        turnOn('A');
        turnOff('B');
        turnOn('C');
        turnOn('D');
        turnOff('F');
        turnOn('G');
        turnOn('E');
    }
    else if(num == 3) {
        turnOn('A');
        turnOff('B');
        turnOn('C');
        turnOn('D');
        turnOn('F');
        turnOn('G');
        turnOff('E');
    }
    else if(num == 4) {
        turnOff('A');
        turnOn('B');
        turnOn('C');
        turnOn('D');
        turnOn('F');
        turnOff('G');
        turnOff('E');
    }
    else if(num == 5) {
        turnOn('A');
        turnOn('B');
        turnOff('C');
        turnOn('D');
        turnOn('F');
        turnOn('G');
        turnOff('E');
    }
    else if(num == 6) {
        turnOff('A');
        turnOn('B');
        turnOff('C');
        turnOn('D');
        turnOn('F');
        turnOn('G');
        turnOn('E');
    }
    else if(num == 7) {
        turnOn('A');
        turnOff('B');
        turnOn('C');
        turnOff('D');
        turnOn('F');
        turnOff('G');
        turnOff('E');
    }
    else if(num == 8) {
        turnOn('A');
        turnOn('B');
        turnOn('C');
        turnOn('D');
        turnOn('F');
        turnOn('G');
        turnOn('E');
    }
    else if(num == 9) {
        turnOn('A');
        turnOn('B');
        turnOn('C');
        turnOn('D');
        turnOn('F');
        turnOn('G');
        turnOff('E');
    }
    else {
        turnOn('A');
        turnOn('B');
        turnOn('C');
        turnOff('D');
        turnOn('F');
        turnOn('G');
        turnOn('E');
    }
}

/********** END OF LED TEST CODE ***************/


/*********ADC Test CODE************************\
void main() {
    WDT_A->CTL = WDT_A_CTL_PW |             // Stop watchdog timer
                 WDT_A_CTL_HOLD;

    uint16_t doorWidth = 255; // output of sensor if reflecting across whole doorway
    config_adc();
    config_gpio();

    P1->DIR |= BIT0;
    P1->OUT &= ~BIT0;

    SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;   // Wake up on exit from ISR

    // Ensures SLEEPONEXIT takes effect immediately
    __DSB();

    __enable_interrupt();//

    while(1) {
        // Start conversion-software trigger
        ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC;
        __sleep();
    }
}
**********END OF ADC TEST CODE**************/

// Actual Code for Integration... not finished yet
/*void main() {

    uint16_t doorWidth = 255; // output of sensor if reflecting across whole doorway

    WDT_A->CTL = WDT_A_CTL_PW |             // Stop watchdog timer
                 WDT_A_CTL_HOLD;

    config_adc();
    config_gpio();
    config_uart();

    P1->DIR |= 1;
    P1->OUT |= 1;

    SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;   // Wake up on exit from ISR

    // Ensures SLEEPONEXIT takes effect immediately
    __DSB();

    __enable_interrupt();//

    while(1) {
        // Start conversion-software trigger
        ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC;
        //__sleep();
    }

    int numPeople = 0;
    uint8_t state = 0;

    while(1) {

        if(state == 0) {
            // while IR1 and IR2 at wall distance
                // continue sampling in ADC until one is not
                // if IR1 state = 1 if IR2 state = 2
            P1->OUT |= 1;
            while(sensorIN >= doorWidth || sensorOUT >= doorWidth) {
                    // Start conversion-software trigger
                    ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC;
                    __sleep();
            }
            P1->OUT &= 0;
            if(sensorIN < doorWidth)
                state = 1;
            else if(sensorOUT < doorWidth)
                state = 2;
            else
                state = 0;
        }
        else if(state == 1) {
            P1->OUT &= 0;
            // while IR2 at wall distance
                // continue sampling in ADC until IR2 is not
                // go to state 3
            while(sensorOUT >= doorWidth) {
                // Start conversion-software trigger
                ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC;
                __sleep();
            }

            numPeople = numPeople + 1;
            state = 3;
        }
        else if(state == 2) {
            P1->OUT &= 0;
            // while IR1 at wall distance
                // continue sampling in ADC until IR1 is not
                // go to state 4
            while(sensorIN >= doorWidth) {
                // Start conversion-software trigger
                ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC;
                __sleep();
            }

            numPeople = numPeople - 1;
            state = 3;
        }
        else if(state == 3) {
            write_data(numPeople); // send new number of people to ATMega for displaying
            // read_data() to get success from ATMega
            state = 0;
        }
        else
            state = 0;

    }
}*/

// ADC14 interrupt service routine
void ADC14_IRQHandler()
{
    if(ADC14->IFGR0 & ADC14_IFGR0_IFG1) {
        sensorIN = ADC14->MEM[0];   // Move A0 results
        sensorOUT = ADC14->MEM[1];   // Move A1 results, IFG is cleared
        if(sensorIN >= 4000 || sensorOUT >= 4000)
            P1->OUT |= BIT0;
        else
            P1->OUT &= ~BIT0;
    }
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
