#include "msp.h"
#include <stdio.h>
#include "uart.h"
#include "adc.h"
#include "gpio.h"

volatile uint16_t sensorIN = 255;
volatile uint16_t sensorOUT = 255;
volatile uint16_t state = 0;

void turnOffR(char letter);
void turnOnR(char letter);
void turnOffL(char letter);
void turnOnL(char letter);
void displayNum(int num, char leftOrRight);

// Actual Code for Integration... not finished yet

void main() {

    uint16_t doorWidth = 6000; // output of sensor if reflecting across whole doorway

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

    int numPeople = 0;
    state = 0;

    while(1) {

        if(numPeople < 10) {
            displayNum(numPeople, 'R');
            displayNum(10, 'L');
        }
        else {
            displayNum((numPeople - 10), 'R');
            displayNum(numPeople/10, 'L');
        }

        //displayNum(state, 'L');

        if(state == 0) {
            // while IR1 and IR2 at wall distance
                // continue sampling in ADC until one is not
                // if IR1 state = 1 if IR2 state = 2
            sensorIN = 0;
            sensorOUT = 0;

            while(sensorIN < doorWidth && sensorOUT < doorWidth) {
                // Start conversion-software trigger
                ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC;
                //__sleep();
            }

            if(sensorIN >= doorWidth)
                state = 1;
            else if(sensorOUT >= doorWidth)
                state = 2;
            else
                state = 0;

            sensorIN = 0;
            sensorOUT = 0;
        }
        else if(state == 1) {
            // while IR2 at wall distance
                // continue sampling in ADC until IR2 is not
                // go to state 3
            while(sensorOUT < doorWidth) {
                // Start conversion-software trigger
                ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC;
                //__sleep();
            }

            sensorOUT = 0;
            numPeople = numPeople + 1;
            state = 3;
        }
        else if(state == 2) {
            // while IR1 at wall distance
                // continue sampling in ADC until IR1 is not
                // go to state 4
            while(sensorIN < doorWidth) {
                // Start conversion-software trigger
                ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC;
                //__sleep();
            }

            sensorIN = 0;
            numPeople = numPeople - 1;
            state = 3;
        }
        else if(state == 3) {
            //write_data(numPeople); // send new number of people to ATMega for displaying
            // read_data() to get success from ATMega
            int q;
            for(q = 0; q < 200000; q++);
            state = 0;
        }
        else
            state = 0;

    }
}

// ADC14 interrupt service routine
void ADC14_IRQHandler() {
    if(ADC14->IFGR0 & ADC14_IFGR0_IFG1) {
        sensorIN = ADC14->MEM[0];   // Move A0 results
        sensorOUT = ADC14->MEM[1];   // Move A1 results, IFG is cleared

        if((state == 2) && sensorIN >= 6000)
            ADC14->CTL0 &= ~ADC14_CTL0_ENC; // stop conversions
        else if((state == 1) && sensorOUT >= 6000)
            ADC14->CTL0 &= ~ADC14_CTL0_ENC; // stop conversions
        else if(sensorIN >= 6000 || sensorOUT >= 6000)
            ADC14->CTL0 &= ~ADC14_CTL0_ENC; // stop conversions)

    }
}
    
void turnOffR(char letter) {
    if(letter == 'A')
        P4->OUT |= BIT0;
    else if(letter == 'B')
        P6->OUT |= BIT0;
    else if(letter == 'C')
        P4->OUT |= BIT2;
    else if(letter == 'D')
        P6->OUT |= BIT1;
    else if(letter == 'E')
        P1->OUT |= BIT6;
    else if(letter == 'F')
        P5->OUT |= BIT0;
    else
        P1->OUT |= BIT7;
}

void turnOnR(char letter) {
    if(letter == 'A')
        P4->OUT &= ~BIT0;
    else if(letter == 'B')
        P6->OUT &= ~BIT0;
    else if(letter == 'C')
        P4->OUT &= ~BIT2;
    else if(letter == 'D')
        P6->OUT &= ~BIT1;
    else if(letter == 'E')
        P1->OUT &= ~BIT6;
    else if(letter == 'F')
        P5->OUT &= ~BIT0;
    else
        P1->OUT &= ~BIT7;
}

void turnOffL(char letter) {
    if(letter == 'A')
        P3->OUT |= BIT7;
    else if(letter == 'B')
        P3->OUT |= BIT6;
    else if(letter == 'C')
        P3->OUT |= BIT5;
    else if(letter == 'D')
        P5->OUT |= BIT2;
    else if(letter == 'E')
        P2->OUT |= BIT5;
    else if(letter == 'F')
        P5->OUT |= BIT7;
    else
        P3->OUT |= BIT0;
}

void turnOnL(char letter) {
    if(letter == 'A')
        P3->OUT &= ~BIT7;
    else if(letter == 'B')
        P3->OUT &= ~BIT6;
    else if(letter == 'C')
        P3->OUT &= ~BIT5;
    else if(letter == 'D')
        P5->OUT &= ~BIT2;
    else if(letter == 'E')
        P2->OUT &= ~BIT5;
    else if(letter == 'F')
        P5->OUT &= ~BIT7;
    else
        P3->OUT &= ~BIT0;
}

void displayNum(int num, char leftOrRight) {
    if(num == 1 && leftOrRight == 'R') {
        turnOffR('A');
        turnOffR('B');
        turnOnR('C');
        turnOffR('D');
        turnOnR('F');
        turnOffR('G');
        turnOffR('E');
    }
    else if(num == 1 && leftOrRight == 'L') {
        turnOffL('A');
        turnOffL('B');
        turnOnL('C');
        turnOffL('D');
        turnOnL('F');
        turnOffL('G');
        turnOffL('E');
    }
    else if(num == 2 && leftOrRight == 'R') {
        turnOnR('A');
        turnOffR('B');
        turnOnR('C');
        turnOnR('D');
        turnOffR('F');
        turnOnR('G');
        turnOnR('E');
    }
    else if(num == 2 && leftOrRight == 'L') {
        turnOnL('A');
        turnOffL('B');
        turnOnL('C');
        turnOnL('D');
        turnOffL('F');
        turnOnL('G');
        turnOnL('E');
    }
    else if(num == 3 && leftOrRight == 'R') {
        turnOnR('A');
        turnOffR('B');
        turnOnR('C');
        turnOnR('D');
        turnOnR('F');
        turnOnR('G');
        turnOffR('E');
    }
    else if(num == 3 && leftOrRight == 'L') {
        turnOnL('A');
        turnOffL('B');
        turnOnL('C');
        turnOnL('D');
        turnOnL('F');
        turnOnL('G');
        turnOffL('E');
    }
    else if(num == 4 && leftOrRight == 'R') {
        turnOffR('A');
        turnOnR('B');
        turnOnR('C');
        turnOnR('D');
        turnOnR('F');
        turnOffR('G');
        turnOffR('E');
    }
    else if(num == 4 && leftOrRight == 'L') {
        turnOffL('A');
        turnOnL('B');
        turnOnL('C');
        turnOnL('D');
        turnOnL('F');
        turnOffL('G');
        turnOffL('E');
    }
    else if(num == 5 && leftOrRight == 'R') {
        turnOnR('A');
        turnOnR('B');
        turnOffR('C');
        turnOnR('D');
        turnOnR('F');
        turnOnR('G');
        turnOffR('E');
    }
    else if(num == 5 && leftOrRight == 'L') {
        turnOnL('A');
        turnOnL('B');
        turnOffL('C');
        turnOnL('D');
        turnOnL('F');
        turnOnL('G');
        turnOffL('E');
    }
    else if(num == 6 && leftOrRight == 'R') {
        turnOffR('A');
        turnOnR('B');
        turnOffR('C');
        turnOnR('D');
        turnOnR('F');
        turnOnR('G');
        turnOnR('E');
    }
    else if(num == 6 && leftOrRight == 'L') {
        turnOffL('A');
        turnOnL('B');
        turnOffL('C');
        turnOnL('D');
        turnOnL('F');
        turnOnL('G');
        turnOnL('E');
    }
    else if(num == 7 && leftOrRight == 'R') {
        turnOnR('A');
        turnOffR('B');
        turnOnR('C');
        turnOffR('D');
        turnOnR('F');
        turnOffR('G');
        turnOffR('E');
    }
    else if(num == 7 && leftOrRight == 'L') {
        turnOnL('A');
        turnOffL('B');
        turnOnL('C');
        turnOffL('D');
        turnOnL('F');
        turnOffL('G');
        turnOffL('E');
    }
    else if(num == 8 && leftOrRight == 'R') {
        turnOnR('A');
        turnOnR('B');
        turnOnR('C');
        turnOnR('D');
        turnOnR('F');
        turnOnR('G');
        turnOnR('E');
    }
    else if(num == 8 && leftOrRight == 'L') {
        turnOnL('A');
        turnOnL('B');
        turnOnL('C');
        turnOnL('D');
        turnOnL('F');
        turnOnL('G');
        turnOnL('E');
    }
    else if(num == 9 && leftOrRight == 'R') {
        turnOnR('A');
        turnOnR('B');
        turnOnR('C');
        turnOnR('D');
        turnOnR('F');
        turnOnR('G');
        turnOffR('E');
    }
    else if(num == 9 && leftOrRight == 'L') {
        turnOnL('A');
        turnOnL('B');
        turnOnL('C');
        turnOnL('D');
        turnOnL('F');
        turnOnL('G');
        turnOffL('E');
    }
    else if(num == 0 && leftOrRight == 'R') {
        turnOnR('A');
        turnOnR('B');
        turnOnR('C');
        turnOffR('D');
        turnOnR('F');
        turnOnR('G');
        turnOnR('E');
    }
    else if(num == 0 && leftOrRight == 'L') {
        turnOnL('A');
        turnOnL('B');
        turnOnL('C');
        turnOffL('D');
        turnOnL('F');
        turnOnL('G');
        turnOnL('E');
    }
    else if(leftOrRight == 'L') {
        turnOffL('A');
        turnOffL('B');
        turnOffL('C');
        turnOffL('D');
        turnOffL('F');
        turnOffL('G');
        turnOffL('E');
    }
    else {
        turnOffR('A');
        turnOffR('B');
        turnOffR('C');
        turnOffR('D');
        turnOffR('F');
        turnOffR('G');
        turnOffR('E');
    }

}


/*********** LED TESTING CODE  ******************\

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

********** END OF LED TEST CODE ***************/

/*********** UART TEST CODE **********************
void main() {

    int z;

    config_gpio();
    config_uart();

    P1->DIR |= 1;
    P1->OUT |= 1;

    SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;   // Wake up on exit from ISR

    __DSB(); // Ensures SLEEPONEXIT takes effect immediately

    __enable_interrupt(); // enables global interrupts

    uint8_t numPeople = 0b10101010;

    while(1) {
        write_data(numPeople);
    }
}

*********** END UART TEST CODE *********************/

/*********ADC Test CODE************************\

void main() {
    WDT_A->CTL = WDT_A_CTL_PW |             // Stop watchdog timer
                 WDT_A_CTL_HOLD;

    uint16_t doorWidth = 255; // output of sensor if reflecting across whole doorway
    config_adc();
    config_gpio();

    P1->DIR |= BIT0;
    P1->OUT &= ~BIT0;

    P2->DIR |= BIT0;
    P2->OUT &= ~BIT0;

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
**********END OF ADC TEST CODE**************/    

/*********** Misc Test Code*******************/
/*if(sensorIN <= 2000)
            displayNum(0, 'R');
        else if(sensorIN > 2000 && sensorIN <= 4000)
            displayNum(1, 'R');
        else if(sensorIN > 4000 && sensorIN <= 6000)
            displayNum(2, 'R');
        else if(sensorIN > 6000 && sensorIN <= 8000)
            displayNum(3, 'R');
        else if(sensorIN > 8000 && sensorIN <= 10000)
            displayNum(4, 'R');
        else if(sensorIN > 10000 && sensorIN <= 12000)
            displayNum(5, 'R');
        else
            displayNum(6, 'R');

        if(sensorOUT <= 2000)
            displayNum(0, 'L');
        else if(sensorOUT > 2000 && sensorOUT <= 4000)
            displayNum(1, 'L');
        else if(sensorOUT > 4000 && sensorOUT <= 6000)
            displayNum(2, 'L');
        else if(sensorOUT > 6000 && sensorOUT <= 8000)
            displayNum(3, 'L');
        else if(sensorOUT > 8000 && sensorOUT <= 10000)
            displayNum(4, 'L');
        else if(sensorOUT > 10000 && sensorOUT <= 12000)
            displayNum(5, 'L');
        else
            displayNum(6, 'L');*/                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
