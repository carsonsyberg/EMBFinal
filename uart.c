#include "msp.h"
#include "uart.h"

void config_uart(void) {
    // set UCSWRST bit before configuring UART
    // Initialize all eUSCI_A registers with UCSWRST = 1 (including UCAxCTL1)
    // pick asynchronus mode
    // set Baud Rate
        // 1. calculate N = freq of BRCLK / Baud Rate
        // [if N > 16 continue with step 3, otherwise with step 2]
        // 2. OS16 = 0, UCBRx = INT(N) [continue with step 4]
        // 3. OS16 = 1, UCBRx = INT(N/16), UCBRFx = INT([(N/16) – INT(N/16)] × 16)
        // 4. UCBRSx can be found by looking up the fractional part of N ( = N - INT(N) ) in table Table 24-4
        // 5. If OS16 = 0 was chosen, a detailed error calculation is recommended to be performed
    // set stop bits UCAxCTLW0
    // set parity bits UCAxCTLW0
    // set number of data bits UCAxCTLW0
}

void write_data(uint8_t data) {
    //enable transmit
    // clear UCSWRST bit to get to IDLE
    // write data to UCAxTXBUF to initiate transmission
    // set UCSWRST bit to turn off
}

uint8_t read_data(uint8_t data) {
    // clear UCSWRST bit to get to IDLE
    // take data from UCAxRXBUF
    // set UCSWRST bit to turn off
}
