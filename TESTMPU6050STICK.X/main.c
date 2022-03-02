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
#include <stdio.h>
/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/

/* i.e. uint16_t <variable_name>; */

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

void User_Init(void) {
    OSCCON=0b0001000110101110;
    CLKDIV=0x0000;
    ANSA=0b1111111111111100; // RAO should be my DAC1 ref+ , RA1 A/D input, rest all digital    
    TRISA= 0b0000000000000000; // all should be digital output exept RA0 and RA1    
    LATA = 0b0000000000000000;  //initialize all to 0
    ANSB=  0x0000; // RB15 should be DAC2 ref+, RB14  DAC2OUT, RB12 DAC1OUT, rest all digital    
    TRISB= 0b1101001100000100;// all digital output exept RB15, RB14 and RB12, RB2 is digital input for UART RX1, RB8 and RB9 are Input for I2C    
    LATB = 0b0000001100000000; //initialize all to 0    
    ODCB = 0b0000001100000000; //initialize all to 0 
}  
    //**************************************
 
void ssp1_Init(void) {


    //SSP1STAT    
    SSP1STATbits.SMP = 1;       //Slew Rate control. 1 = disabled (1Mhz).    
    SSP1STATbits.CKE = 1;       //1=Enable SMBuss Specific inputs.       
    SSP1STATbits.BF =0;          //Flag, 1 = Buffer full, 0 = Buffer empty.     
    //SSP1CON2    
    SSP1CON2bits.GCEN = 0;      //General Call Interrupt enable.    
    SSP1CON2bits.ACKSTAT = 0;   //Ack Status bit (Master TX mode only)    
    SSP1CON2bits.ACKDT = 0;     //Ack Data bit   (Master Rx mode only)    
    SSP1CON2bits.ACKEN = 0;     //Ack Sequence Enable (Master mode only)    
    SSP1CON2bits.RCEN = 0;      //Receive Enable bit (Master Rx mode only)    
    SSP1CON2bits.PEN = 0;       //Stop condition Enable (Master mode only)    
    SSP1CON2bits.RSEN = 0;      //Repeated Start Enable (Master mode only)    
    SSP1CON2bits.SEN = 0;       //Start Condition Enable.
    //SSP1CON3    
    SSP1CON3bits.ACKTIM = 0;    //Ack Time Status bit.    
    SSP1CON3bits.PCIE = 0;      //Stop condition Interrupt Enable.    
    SSP1CON3bits.SCIE = 1;      //Start condition Interrupt Enable.    
    SSP1CON3bits.BOEN = 0;      //Buffer overwrite enable.    
    SSP1CON3bits.SDAHT = 1;     //SDA Hold time (1 = 300nS, 0= 100nS)    
    SSP1CON3bits.SBCDE = 0;     //Slave Buss Colision detect (Slave mode)    
    SSP1CON3bits.AHEN = 0;      //Address hold (Slave mode)    
    SSP1CON3bits.DHEN = 0;      //Data hold (Slave mode)     
    SSP1MSK = 0;                //Slave Address Mask register.
    SSP1ADD=0x09; // Formula for I2C clock = Fosc/( 2*(SSPxADD+1)) , so if I want clock 1 Mhz I put 15, since 32Mhz/(2*(15+1))=1Mhz
    //SSP1CON1    
    SSP1CON1bits.WCOL = 0;      //Write Collision detect.    
    SSP1CON1bits.SSPOV = 0;     //Receive overflow indicator, unused in master mode    
    SSP1CON1bits.CKP = 0;       //1 = SCK Release, unused in master mode    
    SSP1CON1bits.SSPM = 8;      //Master Synchronous Serial Port Mode.                                   
    SSP1CON1bits.SSPEN = 1;     //Enables MSSP Port. 1 = Enable.
 }

void initializeI2C()
{
    SSP1STAT = 0x00;   
    SSP1CON1bits.SSPEN = 1;
    SSP1CON1bits.SSPM = 0b1000;
    SSP1CON3 = 0x00;
    SSP1ADD = 0x09;
}

void writeByte(uint8_t highByte, uint8_t lowByte, uint8_t data)
{
    SSP1CON2bits.SEN = 1;
    
    while(SSP1CON2bits.SEN){}
    
    SSP1CON2bits.PEN = 1;
}

int16_t main(void)
{
    /* Configure the oscillator for the device */
    ConfigureOscillator();
    /* Initialize IO ports and peripherals */
    InitApp();

    /* TODO <INSERT USER APPLICATION CODE HERE> */ 

    //initializeI2C();
    //writeByte(0xFF, 0xFF, 0xA0);
    User_Init();
    ssp1_Init();     
    ANSAbits.ANSA0 = 0; //digital pin
    TRISAbits.TRISA0 = 0; //output
    LATAbits.LATA0 = 1;//active RA0
    Setup_PWM1();
    //MPU6050_Init();
       
    
    while(1)
    {
	    //MPU6050_Read();    
        //__delay_ms(5);
        
        //LATBbits.LATB3=~LATBbits.LATB3; 
        SSP1CON2bits.SEN = 1;   //Generate Start Condition 
        while (SSP1CON2bits.SEN);    //Wait for Start Condition 
        SSP1BUF=0xD1; 
        while(SSP1STATbits.R_W);
        SSP1BUF=0xD0; 
        while(SSP1STATbits.R_W);     
        SSP1CON2bits.PEN = 1;   // Initiate Stop on SDA and SCL pins 
        while(SSP1CON2bits.PEN);  // Wait for Stop conditon to send          
    }
}
