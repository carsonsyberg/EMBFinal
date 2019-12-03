#include "msp.h"
#include "uart.h"

void config_uart(void) {
    // set UCSWRST bit before configuring UART
    EUSCI_A3->CTLW0 |= EUSCI_A_CTLW0_SWRST;

    // select SMCLK as BRCLK source
    EUSCI_A3->CTLW0 |= EUSCI_B_CTLW0_SSEL__SMCLK;

    // pick asynchronus mode
    EUSCI_A3->CTLW0 &= ~(BIT9) & ~(BIT9 << 1);

// Baud Rate calculation //
    // 12000000/(16*9600) = 78.125
    // Fractional portion = 0.125
    // User's Guide Table 21-4: UCBRSx = 0x10
    // UCBRFx = int ( (78.125-78)*16) = 2
    EUSCI_A3->BRW = 78;                     // 12000000/16/9600
    EUSCI_A3->MCTLW = (2 << EUSCI_A_MCTLW_BRF_OFS) | EUSCI_A_MCTLW_OS16;

    EUSCI_A3->CTLW0 &= ~EUSCI_A_CTLW0_SWRST; // Initialize eUSCI
    EUSCI_A3->IFG &= ~EUSCI_A_IFG_RXIFG;    // Clear eUSCI RX interrupt flag
    EUSCI_A3->IE |= EUSCI_A_IE_RXIE;        // Enable USCI_A0 RX interrupt

    // set stop bits UCAxCTLW0
    // set parity bits UCAxCTLW0
    // set number of data bits UCAxCTLW0

    // Enable eUSCIA0 interrupt in NVIC module
    NVIC->ISER[0] = 1 << ((EUSCIA3_IRQn) & 31);
}

// UART interrupt service routine (For Recieving)
uint8_t EUSCIA3_IRQHandler(void)
{
    uint8_t data;

    if (EUSCI_A3->IFG & EUSCI_A_IFG_RXIFG) {
        EUSCI_A3->IFG &= ~EUSCI_A_IFG_RXIFG;// Clear interrupt
        data = EUSCI_A3->RXBUF;           // Clear buffer
    }

    return data;
}

void write_data(uint8_t data) {

    //while(!(EUSCI_A3->IFG & EUSCI_A_IFG_TXIFG));
            EUSCI_A3->TXBUF = data;           // Load data onto buffer
            P1->OUT |= EUSCI_A3->TXBUF;
}

