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
#include "system.h"
#include <libpic30.h>
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

void Setup_PWM1(void)
{ 
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
    CCP1PRL = 99; //Configure timebase period
    CCP1RA = 0; // Set the rising edge compare value
    CCP1RB = 50; // Set the falling edge compare value
    CCP1CON1Lbits.CCPON = 1; // Turn on MCCP module
}
void PWM1_Set_DC(uint16_t DC)
{
   // Hard Limit The DC Value To Fit In Range
   if(DC<10)
       DC=0;
   if(DC>1023)
       DC = 1023;
   // Write The DC Value To PWM DC Registers
   //DC1B0 = DC & 1;
   //DC1B1 = DC & 2;
   //CCPR1L = DC >> 2;
   CCP1PRL = DC >> 2; 
}

//---------------[ I2C Routines ]-------------------
//--------------------------------------------------
void I2C_Master_Init()
{
  // SSPEN enabled; WCOL no_collision; CKP Clock Stretch; SSPM FOSC/(2 * (BRG_Value_I2C + 1)); SSPOV no_overflow;
  SSP1CON1  = 0x28;
  //control register 2 
  SSP1CON2 = 0x00;
  // SBCDE disabled; BOEN disabled; SCIE disabled; PCIE disabled; DHEN disabled; SDAHT 100ns; AHEN disabled;
  SSP1CON3 = 0x00;
  // SMP High Speed; CKE Idle to Active; 
  SSP1STAT = 0x00;
  
  SSP1ADD = ((_XTAL_FREQ/4)/I2C_BaudRate) - 1;
  
  SCL_D = 1;
  SDA_D = 1; 
}

void I2C_Master_Wait()
{
    while ((SSP1STAT & 0x04) || (SSP1CON2 & 0x1F));
}

void I2C_Master_Start()
{
    I2C_Master_Wait();
    SSP1CON2bits.SEN = 1;
}

void I2C_Start(char add)
{
    I2C_Master_Wait();
    SSP1CON2bits.SEN = 1;
    I2C_Master_Write(add);
}

void I2C_Master_RepeatedStart()
{
    I2C_Master_Wait();
    SSP1CON2bits.RSEN = 1;
}

void I2C_Master_Stop()
{
    I2C_Master_Wait();
    SSP1CON2bits.PEN = 1;
}

void I2C_ACK(void)
{
	SSP1CON2bits.ACKDT = 0;			// 0 -> ACK
    SSP1CON2bits.ACKEN = 1;			// Send ACK
    while(SSP1CON2bits.ACKEN);
}

void I2C_NACK(void)
{
	SSP1CON2bits.ACKDT = 1;			// 1 -> NACK
	SSP1CON2bits.ACKEN = 1;			// Send NACK
    while(SSP1CON2bits.ACKEN);
}

unsigned char I2C_Master_Write(unsigned char data)
{
    I2C_Master_Wait();
    SSP1BUF = data;
    while(!IFS1bits.SSP1IF);  // Wait Until Completion
	IFS1bits.SSP1IF = 0;
    return SSP1CON2bits.ACKSTAT;
}

unsigned char I2C_Read_Byte(void)
{
    //---[ Receive & Return A Byte ]---
    I2C_Master_Wait();
    SSP1CON2bits.RCEN = 1;		  // Enable & Start Reception
	while(!IFS1bits.SSP1IF);	  // Wait Until Completion
	IFS1bits.SSP1IF = 0;		  // Clear The Interrupt Flag Bit
    I2C_Master_Wait();
    return SSP1BUF;	  // Return The Received Byte
}

unsigned char I2C_Read(unsigned char ACK_NACK)
{  
    //---[ Receive & Return A Byte & Send ACK or NACK ]---
    unsigned char Data;
    SSP1CON2bits.RCEN = 1;              
    while(!SSP1STATbits.BF);      
    Data = SSP1BUF;           
    if(ACK_NACK==0)
        I2C_ACK();            
    else
        I2C_NACK();     
    while(!IFS1bits.SSP1IF);                 
    IFS1bits.SSP1IF=0;   
    return Data;
}

void MPU6050_Init()
{
    // Power-Up Delay & I2C_Init
	__delay_ms(1000);	
	//I2C_Master_Init();
    
    // Setting The Sample (Data) Rate
	I2C_Start(0xD0);    
    I2C_Master_Write(SMPLRT_DIV);	
	I2C_Master_Write(0x07);
	I2C_Master_Stop();
    
    // Setting The Clock Source
    I2C_Start(0xD0);
	I2C_Master_Write(PWR_MGMT_1);	
	I2C_Master_Write(0x01);
	I2C_Master_Stop();
    
    // Configure The DLPF
	I2C_Start(0xD0);
	I2C_Master_Write(CONFIG);	
	I2C_Master_Write(0x04);	 //  <-- Note This (Change The DLPF BandWidth If You Need)
	I2C_Master_Stop();
    
    // Configure The ACCEL (FSR= +-2g)
 	I2C_Start(0xD0);
	I2C_Master_Write(ACCEL_CONFIG);	
	I2C_Master_Write(0x00);
	I2C_Master_Stop();   
    
    // Configure The GYRO (FSR= +-2000d/s)
	I2C_Start(0xD0);
	I2C_Master_Write(GYRO_CONFIG);	
	I2C_Master_Write(0x18);
	I2C_Master_Stop();
    
    // Enable Data Ready Interrupts
	I2C_Start(0xD0);
	I2C_Master_Write(INT_ENABLE);	
	I2C_Master_Write(0x01);
	I2C_Master_Stop();
}

void MPU6050_Read()
{
   int Ax,Ay,Az,T,Gx,Gy,Gz;
   
   // Prepare For Reading, Starting From ACCEL_XOUT_H
   I2C_Start(0xD0);
   I2C_Master_Write(ACCEL_XOUT_H);
   I2C_Master_Stop();
   I2C_Start(0xD1);  
   I2C_Read(0); // dummy
   Ax = ((int)I2C_Read(0)<<8) | (int)I2C_Read(0);
   Ay = ((int)I2C_Read(0)<<8) | (int)I2C_Read(0);
   Az = ((int)I2C_Read(0)<<8) | (int)I2C_Read(0);
   T  = ((int)I2C_Read(0)<<8) | (int)I2C_Read(0);
   Gx = ((int)I2C_Read(0)<<8) | (int)I2C_Read(0);
   Gy = ((int)I2C_Read(0)<<8) | (int)I2C_Read(0);
   Gz = ((int)I2C_Read(0)<<8) | (int)I2C_Read(1);
   I2C_Master_Stop();
   
   // Set The PWM DC (Speed Control)
   uint16_t  Y = Ay + 16400; // Step1: Biaging ACC_Y
   PWM1_Set_DC(Y>>5);        // Step2: Mapping Y To DC
}