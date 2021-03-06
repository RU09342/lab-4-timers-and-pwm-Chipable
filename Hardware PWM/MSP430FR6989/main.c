/* Lab 4 HARDWARE PWM Toggle w\ interrupt
 * MSP430FR6989
 * 10/8/2017
 * Robert Page
 * main.c
 */

#include <msp430.h>

int main(void){

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;  // adjust power settings

    /* GPIO INIT*/
    P1SEL0 |= BIT0;         // sets port mux for timer output
    P9SEL0 &= ~BIT7;             // sets port mux
    P1DIR |=  BIT0 | BIT2;    // Sets P1.0 as an output
    P9DIR |= BIT7;    // Sets P1.0 as an output
    P9OUT &= ~BIT7;    // Sets pull up to VCC

    P1DIR &= ~(1<<1);   // sETS P1.1 as an input
    P1REN |= (1<<1);    // Enables pull up resistor on P1.1
    P1OUT |= (1<<1);    // Sets pull up to VCC

    /* INTERRUPT INIT */
    P1IE |= BIT1;
    P1IES |= BIT1;
    P1IFG &= ~BIT1;

    TA0CTL= (MC_1  + TASSEL_2);            //up timer, Aclk, div 8

    TA0CCTL1 = (OUTMOD_7);
    TA0CCR0= 500;                           // sets maximum timer value
    TA0CCR1= 250;

    _BIS_SR(CPUOFF + GIE);                  // Enter LPM0 w/ interrupt

}
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void){
    /* DEBOUNCE AND WAIT FLIP EDGE TRIGGER */
    P1IE &= ~BIT1;
    _delay_cycles(50000);
    P1IE |= BIT1;               //re-enable interrupt
    P1IES ^= BIT1;

    if (~P1IES & BIT1){           //if interrupt entered on posedge
        P9OUT ^= BIT7;              // flip led
        TA0CCR1 += 50;              // increase duty by 10%

        if(TA0CCR1 == 550)
            TA0CCR1 = 0;            // stay on if at 100%
    }
    else if (P1IES & BIT1){      // executes on the negedge
        P9OUT ^= BIT7;
    }

    P1IFG &= ~BIT1;
}
