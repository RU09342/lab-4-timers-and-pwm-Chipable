# Debouncing
## description
Applicable for code written for the following dev boards:
* FR5994
* FR2311
* F5529
* G2553
* FR6989

The code debounces a button input when interrupts are used. It does this by setting IE to 0 in the interupt ISR, waiting approximately 50ms, and then re-enabling interrupts by setting IE to 1.

## Similarities 
the code is simillar in that they all disable interrupts in the button interrupt ISR, wait, and then enable interrupts again. 

## Differences
One of the main differences is that the FR boards require and extra line of code "PM5CTL0 &= ~LOCKLPM5;" to adjust the power settings in order to be able to adjust the P1 registers