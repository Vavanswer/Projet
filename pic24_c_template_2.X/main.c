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

#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp              */
#include <libpic30.h>
/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/

/* i.e. uint16_t <variable_name>; */

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

int16_t main(void)
{

    /* Configure the oscillator for the device */
    ConfigureOscillator();

    /* Initialize IO ports and peripherals */
    InitApp();

    /* TODO <INSERT USER APPLICATION CODE HERE> */
    
    InitLED(); //configure the LED 

    InitPWM(); //configure the PWM
    
    InitPWM2(); //configure the PWM

    while(1)
    {
        LATAbits.LATA0 = 1;
        LATAbits.LATA2 = 0; //active la broche RA2
        LATAbits.LATA1 = 1;
        LATAbits.LATA7 = 0;

        //PORTAbits.RA0=0;
        __delay_ms(1000);    
        LATAbits.LATA0 = 0;
        LATAbits.LATA2 = 1; //active la broche RA2
        LATAbits.LATA1 = 0;
        LATAbits.LATA7 = 1;

        __delay_ms(1000);
    }
}
