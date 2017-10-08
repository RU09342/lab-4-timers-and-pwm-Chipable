/* Lab 3 Button LED Toggle w\ interrupt
 * MSP430FR5994
 * 10/8/2017
 * Robert Page
 * main.c
 */

#include <msp430.h> 

int main(void){

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5; // Disable the GPIO power-on default high-impedance mode

    /* GPIO INIT*/
    P1SEL0 &= ~(BIT0|BIT1);     // sets port mux
    P5SEL0 &= ~(BIT6);     // sets port mux
    P1DIR |= BIT0;              // Sets P1.0 as an output
    P1OUT |= BIT0;         // Sets pull up to VCC

    P5DIR &= ~BIT6;             // sETS P1.1 as an input
    P5REN |= BIT6;              // Enables pull up resistor on P1.1
    P5OUT |= BIT6;         // Sets pull up to VCC

    /* INTERRUPT INIT */
    P5IE |= BIT6;
    P5IES &= ~BIT6;
    P5IFG &= ~BIT6;

    _BIS_SR(CPUOFF + GIE);        // Enter LPM0 w/ interrupt

}

#pragma vector=PORT5_VECTOR
__interrupt void Port_1(void){
    P5IE &= ~BIT6;
    _delay_cycles(50000);
    P5IE |= BIT6;

    P1OUT ^=(1<<0);
    P5IFG &= ~BIT6;
}
