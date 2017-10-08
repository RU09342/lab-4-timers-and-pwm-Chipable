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

    P2DIR &= ~(1<<1);   // sETS P1.1 as an input
    P2REN |= (1<<1);    // Enables pull up resistor on P1.1
    P2OUT |= (1<<1);    // Sets pull up to VCC

    /* INTERRUPT INIT */
    P2IE |= BIT1;
    P2IES &= ~BIT1;
    P2IFG &= ~BIT1;

    _BIS_SR(CPUOFF + GIE);        // Enter LPM0 w/ interrupt

}

#pragma vector=PORT2_VECTOR
__interrupt void Port_1(void){
    P2IE &= ~BIT1;
    _delay_cycles(50000);
    P2IE |= BIT1;

    P1OUT ^=(1<<0);
    P2IFG &= ~BIT1;
}
