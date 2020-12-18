#include <msp430.h>

int main(void)
{
    WDTCTL  = WDTPW | WDTHOLD;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL  = CALDCO_1MHZ;

    P1DIR = -1;
    P1OUT = -1;

    P2SEL &= ~(BIT6 | BIT7);

    P2DIR = -1;
    P2OUT = 0;

    _BIS_SR(LPM4_bits);

    return 0;
}
