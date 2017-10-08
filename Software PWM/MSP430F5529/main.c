/* Lab 4 Software PWM Toggle w\ interrupt
 * MSP430G2553
 * 10/5/2017
 * Robert Page
 * main.c
 */

#include <msp430.h>

int main(void){

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    /* GPIO INIT*/
    P1SEL &= ~BIT0;       // sets port mux
    P4SEL &= ~BIT7;       // sets port mux
    P1DIR |= BIT0;    // Sets P1.0 as an output
    P4DIR |= BIT7;    // Sets P1.0 as an output
    P4OUT &= ~BIT7;

    P2DIR &= ~(1<<1);   // sETS P1.1 as an input
    P2REN |= (1<<1);    // Enables pull up resistor on P1.1
    P2OUT |= (1<<1);    // Sets pull up to VCC

    /* INTERRUPT INIT */
    P2IE |= BIT1;
    P2IES |= BIT1;
    P2IFG &= ~BIT1;

    TA0CTL= ( MC_1  + TASSEL_2);             //up timer, Aclk, div 8

    TA0CCTL1 = (CCIE);
    TA0CCTL0 = (CCIE);
    TA0CCR0= 500;        // sets maximum timer value
    TA0CCR1= 250;

    _BIS_SR(CPUOFF + GIE);        // Enter LPM0 w/ interrupt

}
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer0_A1_ISR (void){
    if(TA0CCR1==500){

    }
    else{
       P1OUT &= ~BIT0;
    }

    TA0CCTL1&=~BIT0;    //clears flags
}
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void){
    P1OUT |= BIT0;

    TA0CCTL0 &=~BIT0;    //clears flags
}
#pragma vector=PORT2_VECTOR
__interrupt void Port_1(void){
    /* DEBOUNCE AND WAIT FLIP EDGE TRIGGER */
    P2IE &= ~BIT1;
    _delay_cycles(50000);
    P2IE |= BIT1;               //re-enable interrupt
    P2IES ^= BIT1;

    if (~P2IES & BIT1){           //if interrupt entered on posedge
        P4OUT ^= BIT7;              // flip led
        TA0CCR1 += 50;              // increase duty by 10%

        if(TA0CCR1 == 550)
            TA0CCR1 = 0;            // stay on if at 100%
    }
    else if (P2IES & BIT1){      // executes on the negedge
        P4OUT ^= BIT7;
    }

    P2IFG &= ~BIT1;
}
