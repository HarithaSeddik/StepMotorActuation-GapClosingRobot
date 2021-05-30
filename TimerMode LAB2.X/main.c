/*
 * File:   main.c
 
 */

#include <xc.h>
#include "config.h"
#define _XTAL_FREQ 4000000

char calc_dist(void);

void main(void) {
    // Create Distance Variable (char: 0->255)
    unsigned char dist=0;
    // Set PORTB To Be Output Port (All The 8 Pins)
    TRISB = 0x00;
    // Set PORTB To Be LOW For initial State
    PORTB = 0x00;
    // Set RC2 To Be Output Pin  ( Trigger )
    TRISC2 = 0;
    RC2 = 0;
    // Set RC3 To Be Input Pin    ( Echo )
    TRISC3 = 1;
    //--[ Configure Timer Module To Operate in Timer Mode ]--
    // Clear The Pre-Scaler Select Bits
    T1CKPS0=0;
    T1CKPS1=0;
    // Choose The Local Clock As Clock Source
    TMR1CS=0;
    // Write The System's Main Routine !
    while(1)
    {
        dist = calc_dist()/5;
        if(dist==1)
        {PORTB = 0x01; __delay_ms(100);}
        if(dist==2)
        {PORTB = 0x03; __delay_ms(100);}
        if(dist==3)
        {PORTB = 0x07; __delay_ms(100);}
        if(dist==4)
        {PORTB = 0x0F; __delay_ms(100);}
        else
        {PORTB = 0x00; __delay_ms(100);}
    }
    return;
}
// Definition Of The calc_dist() Function
char calc_dist(void)
{
    unsigned char distance=0; // For Precision Critical Situations, You'll have to make it a ((Floating-Point)) variable
    TMR1=0;
    RC2=1;
    __delay_us(10);
    RC2=0;
    while(!RC3);
    TMR1ON=1;
    while(RC3);
    TMR1ON=0;
    distance = (TMR1/58.82); // Note: fractions will be discarded. But it's OK in our basic tutorial
    if(distance>=2 && distance <150)
         return distance;
    else
        return 0;
}
