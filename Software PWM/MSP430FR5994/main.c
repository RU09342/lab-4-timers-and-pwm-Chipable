/* Lab 4 Software PWM Toggle w\ interrupt
 * MSP430FR5994
 * 10/8/2017
 * Robert Page
 * main.c
 */

#include <msp430.h>

int main(void){

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;  // adjust power settings

    /* GPIO INIT*/
    P1SEL0 &= ~(BIT0);       // sets port mux
    P1SEL0 &= ~(BIT1);       // sets port mux
    P1DIR |= BIT0;    // Sets P1.0 as an output
    P1DIR |= BIT1;    // Sets P1.0 as an output
    P1OUT &= ~(BIT0|BIT1);

    P5DIR &= ~(1<<6);   // sETS P1.1 as an input
    P5REN |= (1<<6);    // Enables pull up resistor on P1.1
    P5OUT |= (1<<6);    // Sets pull up to VCC

    /* INTERRUPT INIT */
    P5IE |= BIT6;
    P5IES |= BIT6;
    P5IFG &= ~BIT6;

    TB0CTL= ( MC__UP  + TBSSEL__SMCLK);             //up timer, Aclk, div 8

    TB0CCTL1 = (CCIE);
    TB0CCTL0 = (CCIE);
    TB0CCR0= 500;        // sets maximum timer value
    TB0CCR1= 250;

    _BIS_SR(CPUOFF + GIE);        // Enter LPM0 w/ interrupt

}
#pragma vector=TIMER0_B1_VECTOR
__interrupt void Timer0_B1_ISR (void){
    if(TB0CCR1==500){

    }
    else{
       P1OUT &= ~BIT0;
    }

    TB0CCTL1&=~BIT0;    //clears flags
}
#pragma vector=TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR (void){
    P1OUT |= BIT0;

    TB0CCTL0 &=~BIT0;    //clears flags
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
        TB0CCR1 += 50;              // increase duty by 10%

        if(TB0CCR1 == 550)
            TB0CCR1 = 0;            // stay on if at 100%
    }
    else if (P5IES & BIT6){      // executes on the negedge
        P1OUT ^= BIT1;
    }

    P5IFG &= ~BIT6;
}
