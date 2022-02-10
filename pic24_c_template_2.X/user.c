/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__PIC24E__)
    	#include <p24Exxxx.h>
    #elif defined (__PIC24F__)||defined (__PIC24FK__)
	#include <p24Fxxxx.h>
    #elif defined(__PIC24H__)
	#include <p24Hxxxx.h>
    #endif
#endif

#include <stdint.h>          /* For uint32_t definition */
#include <stdbool.h>         /* For true/false definition */

#include "user.h"            /* variables/params used by user.c */

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* <Initialize variables in user.h and insert code for user algorithms.> */

/* TODO Initialize User Ports/Peripherals/Project here */

void InitApp(void)
{
    /* Setup analog functionality and port direction */

    /* Initialize peripherals */
}

void InitLED(void)
{
    ANSAbits.ANSA0 = 0; //digital pin
    TRISAbits.TRISA0 = 0; //output
}

void InitPWM(void)
{
    ANSAbits.ANSA2 = 0; //digital pin
    TRISAbits.TRISA2 = 0; //output
    
  
    TRISBbits.TRISB7=0;//output
    ANSBbits.ANSB7=0;//digital pin
    // Set MCCP operating mode
    CCP1CON1Lbits.CCSEL = 0; // Set MCCP operating mode (OC mode)
    CCP1CON1Lbits.MOD = 0b0101; // Set mode (Buffered Dual-Compare/PWM mode)
    //Configure MCCP Timebase
    CCP1CON1Lbits.T32 = 0; // Set timebase width (16-bit)
    CCP1CON1Lbits.TMRSYNC = 0; // Set timebase synchronization (Synchronized)
    CCP1CON1Lbits.CLKSEL = 0b000; // Set the clock source (Tcy)
    CCP1CON1Lbits.TMRPS = 0b00; // Set the clock pre-scaler (1:1)
    CCP1CON1Hbits.TRIGEN = 0; // Set Sync/Triggered mode (Synchronous)
    CCP1CON1Hbits.SYNC = 0b00000; // Select Sync/Trigger source (Self-sync)
    //Configure MCCP output for PWM signal
    CCP1CON2Hbits.OCAEN = 1; // Enable desired output signals (OC1A)
    CCP1CON3Hbits.OUTM = 0b000; // Set advanced output modes (Standard output)
    CCP1CON3Hbits.POLACE = 0; //Configure output polarity (Active High)
    CCP1TMRL = 0x0000; //Initialize timer prior to enable module.
    CCP1PRL = 97; //Configure timebase period
    CCP1RA = 0; // Set the rising edge compare value
    CCP1RB = 50; // Set the falling edge compare value
    CCP1CON1Lbits.CCPON = 1; // Turn on MCCP module
}

void InitPWM2(void)
{
    ANSAbits.ANSA1 = 0; //digital pin
    TRISAbits.TRISA1 = 0; //output
    

    //ANSAbits. = 0; //digital pin
    TRISAbits.TRISA7 = 0; //output
    
    
    TRISBbits.TRISB10=0;//output
    // Set MCCP operating mode
    CCP3CON1Lbits.CCSEL = 0; // Set MCCP operating mode (OC mode)
    CCP3CON1Lbits.MOD = 0b0101; // Set mode (Buffered Dual-Compare/PWM mode)
    //Configure MCCP Timebase
    CCP3CON1Lbits.T32 = 0; // Set timebase width (16-bit)
    CCP3CON1Lbits.TMRSYNC = 0; // Set timebase synchronization (Synchronized)
    CCP3CON1Lbits.CLKSEL = 0b000; // Set the clock source (Tcy)
    CCP3CON1Lbits.TMRPS = 0b00; // Set the clock pre-scaler (1:1)
    CCP3CON1Hbits.TRIGEN = 0; // Set Sync/Triggered mode (Synchronous)
    CCP3CON1Hbits.SYNC = 0b00000; // Select Sync/Trigger source (Self-sync)
    //Configure MCCP output for PWM signal
    CCP3CON2Hbits.OCAEN = 1; // Enable desired output signals (OC3A)
    CCP3CON3Hbits.OUTM = 0b000; // Set advanced output modes (Standard output)
    CCP3CON3Hbits.POLACE = 0; //Configure output polarity (Active High)
    CCP3TMRL = 0x0000; //Initialize timer prior to enable module.
    CCP3PRL = 97; //Configure timebase period
    CCP3RA = 0; // Set the rising edge compare value
    CCP3RB = 50; // Set the falling edge compare value
    CCP3CON1Lbits.CCPON = 1; // Turn on MCCP module
}