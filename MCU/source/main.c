//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <compiler_defs.h>
#include <C8051F410_defs.h>                // SFR declarations
#include <i2c.h>
#include <SPI0.h>
#include <Dev.h>
#include <EDID.h>
#include <AF.h>
#include <OV5640.h>

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

INTERRUPT_PROTO (UART0_ISR, INTERRUPT_UART0);
INTERRUPT_PROTO (INT0_ISR, INTERRUPT_INT0);
INTERRUPT_PROTO (INT1_ISR, INTERRUPT_INT1);

//-----------------------------------------------------------------------------
// Bit setting
//-----------------------------------------------------------------------------

SBIT(RF_EN, P2, 3);
SBIT(RF_RESTORE, P2, 2);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

#define UART_BUFFERSIZE 16
U8 xdata UART_Buffer[UART_BUFFERSIZE];
U8 xdata TX_UART_Buffer[UART_BUFFERSIZE];
U8 UART_Buffer_Size = 0;
U8 UART_Input_First = 0;
U8 UART_Output_First = 0;
U8 cis_setting = 1;   			// 1 : standby, 0 : ready for CIS setting
U8 new_reply = 1;     			// 1 : standby, 0 : sending new reply
U8 new_cmd = 1;       			// 1 : standby, 0 : receiving new cmd, 2 : a complete new cmd is ready for processing
U8 Byte = 0;

//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
//
void main (void)
{

	 U8 i;
	 U16 j;
	 Init_Device();
	 RF_EN = 0;
	 EA = 1;
	 SPI0_2BYTE_WRITE(0xFF, 0x00); // Refresh SPI bus
   while (1)
	 {			 
		 if(cis_setting == 0)
		 {
			 //OV5640 init
			 for(i = 0; i < 250; i++)
			 {
				 I2C_WRITE_OV5640(OV5640_INITI_1920x1080[i][0], OV5640_INITI_1920x1080[i][1]);
			 }
			 //OV5640 AF firmware download
			 I2C_WRITE_OV5640(0x3000, 0x20);
			 for(j = 0x8000; j < 0x8f5d; j++)
			 {
				 I2C_WRITE_OV5640(j, OV5640_AF[j - 0x8000]);
			 }
			 I2C_WRITE_OV5640(0x3022, 0x00);
			 I2C_WRITE_OV5640(0x3023, 0x00);
			 I2C_WRITE_OV5640(0x3024, 0x00);
			 I2C_WRITE_OV5640(0x3025, 0x00);
			 I2C_WRITE_OV5640(0x3026, 0x00);
			 I2C_WRITE_OV5640(0x3027, 0x00);
			 I2C_WRITE_OV5640(0x3028, 0x00);
			 I2C_WRITE_OV5640(0x3029, 0xff);
			 I2C_WRITE_OV5640(0x3000, 0x00);
			 I2C_WRITE_OV5640(0x3004, 0xff);
			 I2C_WRITE_OV5640(0x0000, 0x00);
			 I2C_WRITE_OV5640(0x0000, 0x00);
			 I2C_WRITE_OV5640(0x0000, 0x00);
			 I2C_WRITE_OV5640(0x0000, 0x00);
			 I2C_WRITE_OV5640(0x0000, 0xff);
			 //TFP410 init				
			 I2C_WRITE_TFP410(0x08,0x37);
			 cis_setting = 1;
		 }
		 if( (new_reply == 0) && (UART_Output_First == 0) )
		 {
			 TX_UART_Buffer[0] = '(';
			 TX_UART_Buffer[1] = 'R';
			 TX_UART_Buffer[2] = 'g';
			 TX_UART_Buffer[3] = 'g';
			 TX_UART_Buffer[4] = 'B';
			 TX_UART_Buffer[5] = 'e';
			 TX_UART_Buffer[6] = 'r';
			 TX_UART_Buffer[7] = ')';
			 SBUF0 = TX_UART_Buffer[UART_Output_First];
		 }
		 if(new_cmd == 2)
		 {
			 if( (UART_Buffer[1] == 'C') && (UART_Buffer[2] == 'H') && (UART_Buffer[3] == 'K') && (UART_Buffer[4] == 'H')
				 && (UART_Buffer[5] == 'W') && (UART_Buffer_Size == 6) && (new_reply == 1) )
			 {			 
				 UART_Output_First = 0;
				 new_reply = 0;				 				 
			 }
		 // demo1 AF
		 if( (UART_Buffer[1] == 'D') && (UART_Buffer[2] == 'M') && (UART_Buffer[3] == '1') && (UART_Buffer[4] == 'A')
				 && (UART_Buffer[5] == 'F') && (UART_Buffer_Size == 6) )
			 {
				 I2C_WRITE_OV5640(0x3022, 0x03);				 
			 }
		 // demo1 Release focus
     if( (UART_Buffer[1] == 'D') && (UART_Buffer[2] == 'M') && (UART_Buffer[3] == '1') && (UART_Buffer[4] == 'R')
				 && (UART_Buffer[5] == 'F') && (UART_Buffer[6] == 'u') && (UART_Buffer_Size == 7) )
			 {
				 I2C_WRITE_OV5640(0x3022, 0x08);
			 }
		 //demo2 Negative
     if( (UART_Buffer[1] == 'D') && (UART_Buffer[2] == 'M') && (UART_Buffer[3] == '2') && (UART_Buffer[4] == 'N')
				 && (UART_Buffer[5] == 'e') && (UART_Buffer[6] == 'g') && (UART_Buffer_Size == 7) )
			 {
				 SPI0_2BYTE_WRITE(0x00, 0x01);
			 }
		 //demo2 Embossing
     if( (UART_Buffer[1] == 'D') && (UART_Buffer[2] == 'M') && (UART_Buffer[3] == '2') && (UART_Buffer[4] == 'E')
				 && (UART_Buffer[5] == 'm') && (UART_Buffer[6] == 'b') && (UART_Buffer_Size == 7) )
			 {
				 SPI0_2BYTE_WRITE(0x00, 0x02);
			 }
		 //demo2 Grayscaler
		 if( (UART_Buffer[1] == 'D') && (UART_Buffer[2] == 'M') && (UART_Buffer[3] == '2') && (UART_Buffer[4] == 'G')
				 && (UART_Buffer[5] == 'y') && (UART_Buffer[6] == 's') && (UART_Buffer_Size == 7) )
			 {
				 SPI0_2BYTE_WRITE(0x00, 0x3);
			 }
		 //demo2 Wood carving
		 if( (UART_Buffer[1] == 'D') && (UART_Buffer[2] == 'M') && (UART_Buffer[3] == '2') && (UART_Buffer[4] == 'W')
				 && (UART_Buffer[5] == 'd') && (UART_Buffer[6] == 'c') && (UART_Buffer_Size == 7) )
			 {
				 SPI0_2BYTE_WRITE(0x00, 0x04);
			 }
		 //demo2 4 in 1
		 if( (UART_Buffer[1] == 'D') && (UART_Buffer[2] == 'M') && (UART_Buffer[3] == '2') && (UART_Buffer[4] == '4')
				 && (UART_Buffer[5] == 'n') && (UART_Buffer[6] == '1') && (UART_Buffer_Size == 7) )
			 {
				 SPI0_2BYTE_WRITE(0x00, 0x05);
			 }
		 //demo2 normal
		 if( (UART_Buffer[1] == 'D') && (UART_Buffer[2] == 'M') && (UART_Buffer[3] == '2') && (UART_Buffer[4] == 'N')
				 && (UART_Buffer[5] == 'm') && (UART_Buffer[6] == 'a') && (UART_Buffer_Size == 7) )
			 {
				 SPI0_2BYTE_WRITE(0x00, 0x00);
			 }
		 //demo3 Video1
		 if( (UART_Buffer[1] == 'D') && (UART_Buffer[2] == 'M') && (UART_Buffer[3] == '3') && (UART_Buffer[4] == 'V')
				 && (UART_Buffer[5] == 'd') && (UART_Buffer[6] == '1') && (UART_Buffer_Size == 7) )
			 {
				 SPI0_2BYTE_WRITE(0, 0x01);       // 
			   SPI0_2BYTE_WRITE(1, 0x00);       // ch0 base 
			   SPI0_2BYTE_WRITE(2, 0x00);       // ch1 base 
			   SPI0_2BYTE_WRITE(3, 0xF0);       // ch2 base 
			   SPI0_2BYTE_WRITE(5, 0x00);       // ch0 line base h
			   SPI0_2BYTE_WRITE(6, 0x00);       // ch0 line base l
			   SPI0_2BYTE_WRITE(7, 0x02);       // ch1 line base h
			   SPI0_2BYTE_WRITE(8, 0x1C);       // ch1 line base l
			   SPI0_2BYTE_WRITE(13, 0x02);      // ch2 line base h
			   SPI0_2BYTE_WRITE(14, 0x1C);      // ch2 line base l
			   SPI0_2BYTE_WRITE(9, 0x01);       // ch0 width
			   SPI0_2BYTE_WRITE(10, 0xE0);      // ch0 width
			   SPI0_2BYTE_WRITE(11, 0x00);      // ch1 width
			   SPI0_2BYTE_WRITE(12, 0x00);      // ch1 width
			   SPI0_2BYTE_WRITE(15, 0x00);      // ch2 width
			   SPI0_2BYTE_WRITE(16, 0x00);      // ch2 width
			 }
		 //demo3 Video2
		 if( (UART_Buffer[1] == 'D') && (UART_Buffer[2] == 'M') && (UART_Buffer[3] == '3') && (UART_Buffer[4] == 'V')
				 && (UART_Buffer[5] == 'd') && (UART_Buffer[6] == '2') && (UART_Buffer_Size == 7) )
			 {
				 SPI0_2BYTE_WRITE(0, 0x01);       // 
			   SPI0_2BYTE_WRITE(1, 0x00);       // ch0 base 
			   SPI0_2BYTE_WRITE(2, 0x00);       // ch1 base 
			   SPI0_2BYTE_WRITE(3, 0xF0);       // ch2 base 
			   SPI0_2BYTE_WRITE(5, 0x00);       // ch0 line base h
			   SPI0_2BYTE_WRITE(6, 0x00);       // ch0 line base l
			   SPI0_2BYTE_WRITE(7, 0x02);       // ch1 line base h
			   SPI0_2BYTE_WRITE(8, 0x1C);       // ch1 line base l
			   SPI0_2BYTE_WRITE(13, 0x02);      // ch2 line base h
			   SPI0_2BYTE_WRITE(14, 0x1C);      // ch2 line base l
			   SPI0_2BYTE_WRITE(9, 0x01);       // ch0 width
			   SPI0_2BYTE_WRITE(10, 0xE0);      // ch0 width
			   SPI0_2BYTE_WRITE(11, 0x00);      // ch1 width
			   SPI0_2BYTE_WRITE(12, 0x00);      // ch1 width
			   SPI0_2BYTE_WRITE(15, 0x00);      // ch2 width
			   SPI0_2BYTE_WRITE(16, 0x00);      // ch2 width
			 }
		 //demo3 Video3
		 if( (UART_Buffer[1] == 'D') && (UART_Buffer[2] == 'M') && (UART_Buffer[3] == '3') && (UART_Buffer[4] == 'V')
				 && (UART_Buffer[5] == 'd') && (UART_Buffer[6] == '3') && (UART_Buffer_Size == 7) )
			 {
				 SPI0_2BYTE_WRITE(0, 0x01);       // 
			   SPI0_2BYTE_WRITE(1, 0x00);       // ch0 base 
			   SPI0_2BYTE_WRITE(2, 0x00);       // ch1 base 
			   SPI0_2BYTE_WRITE(3, 0xF0);       // ch2 base 
			   SPI0_2BYTE_WRITE(5, 0x00);       // ch0 line base h
			   SPI0_2BYTE_WRITE(6, 0x00);       // ch0 line base l
			   SPI0_2BYTE_WRITE(7, 0x00);       // ch1 line base h
			   SPI0_2BYTE_WRITE(8, 0x00);       // ch1 line base l
			   SPI0_2BYTE_WRITE(13, 0x02);      // ch2 line base h
			   SPI0_2BYTE_WRITE(14, 0x1C);      // ch2 line base l
			   SPI0_2BYTE_WRITE(9, 0x00);       // ch0 width
			   SPI0_2BYTE_WRITE(10, 0x00);      // ch0 width
			   SPI0_2BYTE_WRITE(11, 0x01);      // ch1 width
			   SPI0_2BYTE_WRITE(12, 0xE0);      // ch1 width
			   SPI0_2BYTE_WRITE(15, 0x00);      // ch2 width
			   SPI0_2BYTE_WRITE(16, 0x00);      // ch2 width
			 }
		 //demo3 Video4
		 if( (UART_Buffer[1] == 'D') && (UART_Buffer[2] == 'M') && (UART_Buffer[3] == '3') && (UART_Buffer[4] == 'V')
				 && (UART_Buffer[5] == 'd') && (UART_Buffer[6] == '4') && (UART_Buffer_Size == 7) )
			 {
				 SPI0_2BYTE_WRITE(0, 0x01);       // 
			   SPI0_2BYTE_WRITE(1, 0x00);       // ch0 base 
			   SPI0_2BYTE_WRITE(2, 0x00);       // ch1 base 
			   SPI0_2BYTE_WRITE(3, 0x00);       // ch2 base 
			   SPI0_2BYTE_WRITE(5, 0x00);       // ch0 line base h
			   SPI0_2BYTE_WRITE(6, 0x00);       // ch0 line base l
			   SPI0_2BYTE_WRITE(7, 0x02);       // ch1 line base h
			   SPI0_2BYTE_WRITE(8, 0x1C);       // ch1 line base l
			   SPI0_2BYTE_WRITE(13, 0x00);      // ch2 line base h
			   SPI0_2BYTE_WRITE(14, 0x00);      // ch2 line base l
			   SPI0_2BYTE_WRITE(9, 0x00);       // ch0 width
			   SPI0_2BYTE_WRITE(10, 0x00);      // ch0 width
			   SPI0_2BYTE_WRITE(11, 0x00);      // ch1 width
			   SPI0_2BYTE_WRITE(12, 0x00);      // ch1 width
			   SPI0_2BYTE_WRITE(15, 0x01);      // ch2 width
			   SPI0_2BYTE_WRITE(16, 0xE0);      // ch2 width
			 }
		 //demo3 All Video
		 if( (UART_Buffer[1] == 'D') && (UART_Buffer[2] == 'M') && (UART_Buffer[3] == '3') && (UART_Buffer[4] == 'A')
				 && (UART_Buffer[5] == 'L') && (UART_Buffer[6] == 'V') && (UART_Buffer_Size == 7) )
			 {
				 SPI0_2BYTE_WRITE(0, 0x00);       // 
			   SPI0_2BYTE_WRITE(1, 0x00);       // ch0 base 
			   SPI0_2BYTE_WRITE(2, 0x00);       // ch1 base 
			   SPI0_2BYTE_WRITE(3, 0xF0);       // ch2 base 
			   SPI0_2BYTE_WRITE(5, 0x00);       // ch0 line base h
			   SPI0_2BYTE_WRITE(6, 0x00);       // ch0 line base l
			   SPI0_2BYTE_WRITE(7, 0x02);       // ch1 line base h
			   SPI0_2BYTE_WRITE(8, 0x1C);       // ch1 line base l
			   SPI0_2BYTE_WRITE(13, 0x02);      // ch2 line base h
			   SPI0_2BYTE_WRITE(14, 0x1C);      // ch2 line base l
			   SPI0_2BYTE_WRITE(9, 0x01);       // ch0 width
			   SPI0_2BYTE_WRITE(10, 0xE0);      // ch0 width
			   SPI0_2BYTE_WRITE(11, 0x00);      // ch1 width
			   SPI0_2BYTE_WRITE(12, 0xF0);      // ch1 width
			   SPI0_2BYTE_WRITE(15, 0x00);      // ch2 width
			   SPI0_2BYTE_WRITE(16, 0xF0);      // ch2 width
			 }
		 //demo4 video1
		 if( (UART_Buffer[1] == 'D') && (UART_Buffer[2] == 'M') && (UART_Buffer[3] == '4') && (UART_Buffer[4] == 'V')
				 && (UART_Buffer[5] == 'd') && (UART_Buffer[6] == '1') && (UART_Buffer_Size == 7) )
			 {
				 SPI0_2BYTE_WRITE(0, 0x00);       
			   SPI0_2BYTE_WRITE(1, 0);          
			   SPI0_2BYTE_WRITE(5, 0);          
			   SPI0_2BYTE_WRITE(6, 0);          
			   SPI0_2BYTE_WRITE(9, 0x01);      
			   SPI0_2BYTE_WRITE(10, 0xE0);      
			   SPI0_2BYTE_WRITE(11, 0x00);       
			   SPI0_2BYTE_WRITE(12, 0x00);         
			 }
		 //demo4 video2
     if( (UART_Buffer[1] == 'D') && (UART_Buffer[2] == 'M') && (UART_Buffer[3] == '4') && (UART_Buffer[4] == 'V')
				 && (UART_Buffer[5] == 'd') && (UART_Buffer[6] == '2') && (UART_Buffer_Size == 7) )
			 {
				 SPI0_2BYTE_WRITE(0, 0x00);       
				 SPI0_2BYTE_WRITE(2, 0);          
			   SPI0_2BYTE_WRITE(7, 0);          
			   SPI0_2BYTE_WRITE(8, 0);          
			   SPI0_2BYTE_WRITE(11, 0x01);          
			   SPI0_2BYTE_WRITE(12, 0xE0);        
			   SPI0_2BYTE_WRITE(9, 0x00);        
			   SPI0_2BYTE_WRITE(10, 0x00);         
			 }
		 //demo4 2 videos
		 if( (UART_Buffer[1] == 'D') && (UART_Buffer[2] == 'M') && (UART_Buffer[3] == '4') && (UART_Buffer[4] == '2')
				 && (UART_Buffer[5] == 'V') && (UART_Buffer[6] == 'd') && (UART_Buffer_Size == 7) )
			 {
				 SPI0_2BYTE_WRITE(0, 0x01);      
			   SPI0_2BYTE_WRITE(1, 0);         
			   SPI0_2BYTE_WRITE(2, 240);          
			   SPI0_2BYTE_WRITE(7, 0);          
			   SPI0_2BYTE_WRITE(8, 0);         
			   SPI0_2BYTE_WRITE(5, 0);        
			   SPI0_2BYTE_WRITE(6, 0);          
			   SPI0_2BYTE_WRITE(11, 0);       
			   SPI0_2BYTE_WRITE(12, 240);         
			   SPI0_2BYTE_WRITE(9, 0);        
			   SPI0_2BYTE_WRITE(10, 240);        
			 }
		 for(i = 0; i < UART_BUFFERSIZE; i++)
		 {
			 UART_Buffer[i] = 0;
		 }
		 UART_Buffer_Size = 0;
		 new_cmd = 1;		 
		 }	 
	 }		
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

INTERRUPT(UART0_ISR, INTERRUPT_UART0)
{
   if (RI0 == 1)
   {
      RI0 = 0;                         // Clear interrupt flag
      Byte = SBUF0;                    // Read a character from UART
		  if( (UART_Buffer_Size == 0) && (new_cmd == 1) && (Byte == '<') )
			{
				UART_Input_First = 0;
				new_cmd = 0;
			}
		  if ( (UART_Buffer_Size < UART_BUFFERSIZE) && (new_cmd == 0) )
			{
				UART_Buffer[UART_Input_First] = Byte; // Store in array
				if( UART_Buffer[UART_Input_First] == '>' )
				{
					new_cmd = 2;
				}
				else
				{
					UART_Buffer_Size++;           // Update array's size
				  UART_Input_First++;           // Update counter
				}				
			}
   }
   if (TI0 == 1)                       // Check if transmit flag is set
   {
      TI0 = 0;                         // Clear interrupt flag
		  if( TX_UART_Buffer[UART_Output_First] == ')' )
			{
				if( new_reply == 0 )
				{
					UART_Output_First = 0;
					new_reply = 1;
				}				
			}
			else
			{				
				if( new_reply == 0 )
				{
					UART_Output_First++;
					SBUF0 = TX_UART_Buffer[UART_Output_First];
				}				  				
			}
   }
}


INTERRUPT(INT1_ISR, INTERRUPT_INT1)
{
   SPI0_2BYTE_WRITE(0xFF, 0x00);     // 
}


INTERRUPT(INT0_ISR, INTERRUPT_INT0)
{
	 cis_setting = 0;
}



//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
