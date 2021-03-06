/* Lab 4 HARDWARE PWM Toggle w\ interrupt
 * MSP430FR5994
 * 10/5/2017
 * Robert Page
 * main.c
 */

#include <msp430.h>

int main(void){

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;  // adjust power settings

    /* GPIO INIT*/
    P1SEL0 |= BIT0;         // sets port mux for timer output
    P1SEL0 &= ~BIT1;             // sets port mux
    P5SEL0 &= ~BIT6;             // sets port mux
    P1DIR |= BIT0|BIT1;    // Sets P1.0 as an output
    P1OUT &= ~BIT1;    // Sets pull up to VCC

    P5DIR &= ~(1<<6);   // sETS P1.1 as an input
    P5REN |= (1<<6);    // Enables pull up resistor on P1.1
    P5OUT |= (1<<6);    // Sets pull up to VCC

    /* INTERRUPT INIT */
    P5IE |= BIT6;
    P5IES |= BIT6;
    P5IFG &= ~BIT6;

    TA0CTL= (MC_1  + TASSEL_2);            //up timer, Aclk, div 8

    TA0CCTL1 = (OUTMOD_7);
    TA0CCR0= 500;                           // sets maximum timer value
    TA0CCR1= 250;

    _BIS_SR(CPUOFF + GIE);                  // Enter LPM0 w/ interrupt

}
#pragma vector=PORT5_VECTOR
__interrupt void Port_1(void){
    /* DEBOUNCE AND WAIT FLIP EDGE TRIGGER */
    P5IE &= ~BIT6;
    _delay_cycles(50000);
    P5IE |= BIT6;               //re-enable interrupt
    P5IES ^= BIT6;

    if (~P5IES & BIT6){           //if interrupt entered on posedge
        P1OUT ^= BIT1;              // flip led
        TA0CCR1 += 50;              // increase duty by 10%

        if(TA0CCR1 == 550)
            TA0CCR1 = 0;            // stay on if at 100%
    }
    else if (P5IES & BIT6){      // executes on the negedge
        P1OUT ^= BIT1;
    }

    P5IFG &= ~BIT6;
}
