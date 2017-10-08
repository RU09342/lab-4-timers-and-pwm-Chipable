/* Lab 3 Button LED Toggle w\ interrupt
 * MSP430G2553
 * 10/5/2017
 * Robert Page
 * main.c
 */

#include <msp430.h> 

int main(void){

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    /* GPIO INIT*/
    P1SEL &= ~(1<<0);       // sets port mux
    P1DIR |= (1<<0);    // Sets P1.0 as an output

    P1DIR &= ~(1<<3);   // sETS P1.1 as an input
    P1REN |= (1<<3);    // Enables pull up resistor on P1.1
    P1OUT |= (1<<3);    // Sets pull up to VCC

    /* INTERRUPT INIT */
    P1IE |= BIT3;
    P1IES &= ~BIT3;
    P1IFG &= ~BIT3;

    _BIS_SR(CPUOFF + GIE);        // Enter LPM0 w/ interrupt

}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void){
    P1IE &= ~BIT3;
    _delay_cycles(50000);
    P1IE |= BIT3;

    P1OUT ^=(1<<0);
    P1IFG &= ~BIT3;
}
