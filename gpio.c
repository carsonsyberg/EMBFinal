#include "msp.h"
#include "gpio.h"

void config_gpio(void) {

    P7->DIR &= ~BIT6 & ~BIT7; // sets ADC pins P7.7 and P7.6 as inputs
    P7->SEL0 |= BIT6 | BIT7; // Setting P7SEL1.x and P7SEL0.x disables the output driver
                             // and the input Schmitt trigger to prevent parasitic cross currents when
                             // applying analog signals.

    P9->DIR &= ~BIT6; // P9.6 is RX pin
    P9->SEL0 |= BIT6; // with 01 in SEL1/0 P2.2 uses function (RX mode) that is not just I/O
    P9->SEL1 &= ~BIT6;

    P9->DIR |= BIT7; // P9.7 is TX pin
    P9->SEL0 |= BIT7; // with 01 in SEL1/0 P2.3 uses function (TX mode) that is not just I/O
    P9->SEL1 &= ~BIT7;

}
