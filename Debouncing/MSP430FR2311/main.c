/* Lab 3 Button LED Toggle w\ interrupt
 * MSP430G2553
 * 10/5/2017
 * Robert Page
 * main.c
 */

#include <msp430.h> 

int main(void){

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5; // Disable the GPIO power-on default high-impedance mode

    /* GPIO INIT*/
    P1SEL0 &= ~(BIT0|BIT1);       // sets port mux
    P1DIR |= BIT0;    // Sets P1.0 as an output

    P1DIR &= ~BIT1;   // sETS P1.1 as an input
    P1REN |= BIT1;    // Enables pull up resistor on P1.1
    P1OUT |= BIT0|BIT1;    // Sets pull up to VCC

    /* INTERRUPT INIT */
    P1IE |= BIT1;
    P1IES &= ~BIT1;
    P1IFG &= ~BIT1;

    _BIS_SR(CPUOFF + GIE);        // Enter LPM0 w/ interrupt

}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void){
    P1IE &= ~BIT1;
    _delay_cycles(50000);
    P1IE |= BIT1;

    P1OUT ^=(1<<0);
    P1IFG &= ~BIT1;
}
