# Software PWM
## description
Applicable for code written for the following dev boards:
* FR5994
* FR2311
* F5529
* G2553
* FR6989

The code used timers, interrupts, and ccrs to create a pwm output on and onboard led. A button was implemented to increase the duty cycle of the pwm by 10% each time it was pressed. Another onboard led is lit up when the button is pressed.

## Similarities 
The code here does not used the onboard timer output to create the pwm signal. Instead the ccr interrupts are used to set and reset the state of an led. To increase the duty cycle with a button press the ccr1 register was incremented in the button ISR.

## Differences
One of the main differences is that the FR boards require and extra line of code "PM5CTL0 &= ~LOCKLPM5;" to adjust the power settings in order to be able to adjust the P1 registers The other main differnce between what timer was used on what board.