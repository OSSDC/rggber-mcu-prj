//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <compiler_defs.h>
#include <C8051F410_defs.h>                // SFR declarations
#include <i2c.h>

SBIT (SDA0, SFR_P2, 1);
SBIT (SCL0, SFR_P2, 0);

bit ack0;

void Start_I2c0()
{
	SDA0=1;   
	delay_2us0();
	SCL0=1;
	delay_2us0();
	SDA0=0;   
	delay_2us0();
	SCL0=0;   
	delay_2us0();
}

void Stop_I2c0()
{
	SDA0=0;  
	delay_2us0();
	SCL0=1;  
	delay_2us0();
	SDA0=1;  
	delay_2us0();
}

void SendByte0(U8 c)
{
	U8 BitCnt;
	for(BitCnt=0;BitCnt<8;BitCnt++)  
    {
		if((c<<BitCnt)&0x80)
			SDA0=1;   
		else
			SDA0=0;
		delay_2us0();
		SCL0=1;              
		delay_2us0();
		SCL0=0;
    }
	delay_2us0();
    SDA0=1;               
    delay_2us0();
    SCL0=1;
    delay_2us0();
    if(SDA0==1)
    	ack0=0;
    else
    	ack0=1;        
    SCL0=0;
    delay_2us0();
}

U8 RcvByte0()
{
	U8 retc;
	U8 BitCnt;
	retc=0;
	SDA0=1;             
	for(BitCnt=0;BitCnt<8;BitCnt++)
    {
		delay_2us0();
        SCL0=0;       
        delay_2us0();
        SCL0=1;      
        delay_2us0();
        retc=retc<<1;
        if(SDA0==1)retc=retc+1; 
        delay_2us0();
     }
	SCL0=0;
	delay_2us0();
	return(retc);
}

void Ack_I2c0(bit a)
{

	if(a==0)
		SDA0=0;     
    else
    	SDA0=1;
	delay_2us0();
	SCL0=1;
	delay_2us0();
	SCL0=0;                
	delay_2us0();
}

void delay_2us0()
{
	U8 i;
	for(i = 0; i<50; i++);
}

void delay_ms()
{
	U16 i;
	for(i = 0; i<0xFFFF; i++);
}

//********************************i2c eeprom**********************************//

U8 I2C_READ_EEPROM(U8 suba)
{
	U8 rec_data;
	rec_data = 0x00;
	Start_I2c0();
	SendByte0(0xA0);
	SendByte0(suba);
	Start_I2c0();
	SendByte0(0xA1);
	rec_data = RcvByte0();
	Ack_I2c0(1);                 
	Stop_I2c0();                   
	return(rec_data);
}

void I2C_WRITE_EEPROM(U8 suba,U8 dat)
{
	Start_I2c0();
	SendByte0(0xA0);
	SendByte0(suba);
	SendByte0(dat);
	Stop_I2c0();
}

//******************************i2c tfp410*************************************//

U8 I2C_READ_TFP410(U8 suba)
{
	U8 rec_data;
	rec_data = 0x00;
	Start_I2c0();
	SendByte0(0x70);
	SendByte0(suba);
	Start_I2c0();
	SendByte0(0x71);
	rec_data = RcvByte0();
	Ack_I2c0(1);                
	Stop_I2c0();                    
	return(rec_data);
}

void I2C_WRITE_TFP410(U8 suba,U8 dat)
{
	Start_I2c0();
	SendByte0(0x70);
	SendByte0(suba);
	SendByte0(dat);
	Stop_I2c0();
}

SBIT (SDA1, SFR_P1, 4);
SBIT (SCL1, SFR_P1, 5);

bit ack1;

void Start_I2c1()
{
	SDA1=1;   
	delay_2us1();
	SCL1=1;
	delay_2us1();
	SDA1=0;   
	delay_2us1();
	SCL1=0;   
	delay_2us1();
}

void Stop_I2c1()
{
	SDA1=0;  
	delay_2us1();
	SCL1=1; 
	delay_2us1();
	SDA1=1;  
	delay_2us1();
}

void SendByte1(U8 c)
{
	U8 BitCnt;
	for(BitCnt=0;BitCnt<8;BitCnt++) 
    {
		if((c<<BitCnt)&0x80)
			SDA1=1;   
		else
			SDA1=0;
		delay_2us1();
		SCL1=1;              
		delay_2us1();
		SCL1=0;
    }
	delay_2us1();
    SDA1=1;              
    delay_2us1();
    SCL1=1;
    delay_2us1();
    if(SDA1==1)
    	ack1=0;
    else
    	ack1=1;        
    SCL1=0;
    delay_2us1();
}

U8 RcvByte1()
{
	U8 retc;
	U8 BitCnt;
	retc=0;
	SDA1=1;          
	for(BitCnt=0;BitCnt<8;BitCnt++)
    {
		delay_2us1();
        SCL1=0;       
        delay_2us1();
        SCL1=1;       
        delay_2us1();
        retc=retc<<1;
        if(SDA1==1)retc=retc+1;
        delay_2us1();
     }
	SCL1=0;
	delay_2us1();
	return(retc);
}

void Ack_I2c1(bit a)
{

	if(a==0)
		SDA1=0;     
    else
    	SDA1=1;
	delay_2us1();
	SCL1=1;
	delay_2us1();
	SCL1=0;                
	delay_2us1();
}

void delay_2us1()
{
	U8 i;
	for(i = 0; i<50; i++);
}

//******************************i2c ov5640*************************************//

U8 I2C_READ_OV5640(U16 suba)
{
	U8 rec_data;
	rec_data = 0x00;
	Start_I2c1();
	SendByte1(0x78);
	SendByte1(suba>>8);
	SendByte1(suba);
	Start_I2c1();
	SendByte1(0x79);
	rec_data = RcvByte1();
	Ack_I2c1(1);
	Stop_I2c1();
	return(rec_data);
}

void I2C_WRITE_OV5640(U16 suba,U8 dat)
{
	Start_I2c1();
	SendByte1(0x78);
	SendByte1(suba>>8);
	SendByte1(suba);
	SendByte1(dat);
	Stop_I2c1();
}