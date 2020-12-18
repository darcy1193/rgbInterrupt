//
//  powerModes.c
//
//
//  Created by Daniel Darcy on 2/14/17.
//  Assighment 3 CS 2230
//
#include <msp430.h>

int powerModeCounter = 0;

int main (void){                //RED = 1.4 | GREEN = 1.6 | BLUE = 2.1       //125000 cycles every second
    
    WDTCTL = WDTHOLD | WDTPW;       //hold washdogtimer so it doesnt reset
    BCSCTL1 = CALBC1_1MHZ;          //calibrate CPU to 1 megahertz
    DCOCTL = CALDCO_1MHZ;
    
    P2DIR = BIT1;                   //blue led 2.1
    P1OUT = 0b00001000;             //*set button as output
    P2SEL = BIT1;                   //blue led 2.1
    P1SEL = BIT6;                   //green led 1.6
    
    P1OUT = 0b00001000;             //*set button as output
    P1REN = BIT3;                   //button
    P1IE |= BIT3;
    P1IFG &= ~BIT3;
    P1IES |= BIT3;
    _BIS_SR(GIE);
    
    for(;;){
        
        //powerMode 1
        if(powerModeCounter == 1){
            
            P1OUT = 0b00011000;
            P1DIR = BIT4;
            P1OUT |= BIT4;
            P2DIR = 0b00000000;
            
            P1OUT = 0b00011000;                    //red led blinks for (1/2) second (execution loop)
            __delay_cycles(500000);
            P1OUT = 0b00001000;
            __delay_cycles(500000);
        }
        
        //powerMode 2                              //red led (1/2 second) + green led(1/3 second)
        else if(powerModeCounter == 2){
            
            P1SEL = BIT6;
            P1DIR = 0b01010000;                    //red led and green led8
            P1OUT |= 0b01011000;
            
            TA0CTL   = TASSEL_2 | MC_1 | ID_3;     //green led (1/3) second (change to interrupt and timer)
            TA0CCR0  = 41666 - 1;                  //calibrate to every 1/3 second)
            TA0CCTL1 = OUTMOD_4;
            
            P1OUT = 0b00011000;                    //red led (1/2) second (execution loop)
            __delay_cycles(500000);
            P1OUT = 0b00001000;
            __delay_cycles(500000);
        }
        
        //powerMode 3                              //blue led(1/4 second) + green led(1/3 second)
        else if(powerModeCounter == 3){
            
            P1DIR = 0b01000000;
            P2DIR = 0b00000010;
            P1OUT |= 0b00001000;
            P2OUT = 0b00000010;
            
            TA1CTL   = TASSEL_2 | MC_1 | ID_3;     //timer and signal (blue led)
            TA1CCR0  = 31250 - 1;                  //calibrate to every 1/4 second
            TA1CCTL1 = OUTMOD_4;
            
            TA0CTL   = TASSEL_2 | MC_1 | ID_3;     //green led (1/3) second (change to interrupt and timer)
            TA0CCR0  = 41666 - 1;                  //calibrate to every 1/3 second (timer and signal)
            TA0CCTL1 = OUTMOD_4;
        }
        
        //powerMode 4                              //blue led(1/2 second)
        else if(powerModeCounter == 4){
            
            P1OUT = 0b00001000;
            P1DIR &= ~BIT6;
            P2DIR = 0b00000010;
            P2OUT = 0b00000010;
            

            
        }
    }                                          //END OF LOOP
    return 0;
}                                          //END OF MAIN


//button_ISR
#pragma vector=PORT1_VECTOR
__interrupt void buttonInterrupt(void){
    
    powerModeCounter++;
    
    if(powerModeCounter > 4){
        powerModeCounter = 1;
    }
    
    while(!(BIT3 & P1IN))  {}
    __delay_cycles(32000);
    P1IFG &= ~BIT3;
}
