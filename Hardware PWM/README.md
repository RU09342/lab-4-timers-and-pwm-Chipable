# Hardware PWM
## description
Applicable for code written for the following dev boards:
* FR5994
* FR2311
* F5529
* G2553
* FR6989
The code configures the onboard timer to output a 1kHz pwm signal with a duty cycle based on a button press.

## Similarities 
All of the code frist starts by configuring the timer control register for upmode with a ccr0 of 500 to get a base frequency of 1kHz. Then the output of a CCR1CTL register is set to mode 7 reset/set to gennerate a pwm output on the desired pin. A button interrupt is then used to increase the value of the CCR1 register to change the duty cycle.

## Differences
One of the main differences is that the FR boards require and extra line of code "PM5CTL0 &= ~LOCKLPM5;" to adjust the power settings in order to be able to adjust the P1 registers another difference is the timer and ccrx regiester used. This is because not all timers are hooked up to the onboard leds.