#include <compiler_defs.h>
#include <C8051F410_defs.h>                // SFR declarations
#include <Dev.h>

#define SYSCLK         49000000/2      // SYSCLK frequency in Hz
#define BAUDRATE             9600      // Baud rate of UART in bps

// Initialization function for device,
// Call Init_Device() from your main program
void Init_Device(void)
{
    PCA_Init();
	  Oscillator_Init();
    Port_IO_Init();   
		Ext_Interrupt_Init();
		UART0_Init();
}

// Peripheral specific initialization functions,
// Called from the Init_Device() function
void Port_IO_Init()
{
    // P0.0  -  TX0 (UART0), Open-Drain, Analog
    // P0.1  -  RX0 (UART0), Open-Drain, Analog
    // P0.2  -  Unassigned,  Open-Drain, Analog
    // P0.3  -  Unassigned,  Open-Drain, Analog
    // P0.4  -  Skipped,     Open-Drain, Digital
    // P0.5  -  Skipped,     Open-Drain, Digital
    // P0.6  -  Skipped,     Open-Drain, Digital
    // P0.7  -  Skipped,     Open-Drain, Digital

    // P1.0  -  Skipped,     Push-Pull,  Digital
    // P1.1  -  Skipped,     Push-Pull,  Digital
    // P1.2  -  Skipped,     Push-Pull,  Digital
    // P1.3  -  Skipped,     Push-Pull,  Digital
    // P1.4  -  Skipped,     Open-Drain, Digital
    // P1.5  -  Skipped,     Open-Drain, Digital
    // P1.6  -  Skipped,     Open-Drain, Digital
    // P1.7  -  Skipped,     Open-Drain, Digital

    // P2.0  -  Skipped,     Open-Drain, Digital
    // P2.1  -  Skipped,     Open-Drain, Digital
    // P2.2  -  Skipped,     Push-Pull,  Digital
    // P2.3  -  Skipped,     Push-Pull,  Digital
    // P2.4  -  Unassigned,  Open-Drain, Analog
    // P2.5  -  Skipped,     Open-Drain, Digital
    // P2.6  -  Skipped,     Open-Drain, Digital
    // P2.7  -  Skipped,     Open-Drain, Digital

    P0MDIN    = 0xF0;
    P2MDIN    = 0xEF;
    P1MDOUT   = 0x0F;
    P2MDOUT   = 0x0C;
    P0SKIP    = 0x0F;
    P2SKIP    = 0x10;
    XBR0      = 0x01;
    XBR1      = 0x40;
}


void PCA_Init()
{
    PCA0MD    &= ~0x40;
    PCA0MD    = 0x00;
}

void Oscillator_Init()
{
    OSCICN    = 0x87;
}

void UART0_Init (void)
{
   SCON0 = 0x10;                       // SCON0: 8-bit variable bit rate
                                       //        level of STOP bit is ignored
                                       //        RX enabled
                                       //        ninth bits are zeros
                                       //        clear RI0 and TI0 bits
   if (SYSCLK/BAUDRATE/2/256 < 1) {
      TH1 = -(SYSCLK/BAUDRATE/2);
      CKCON |=  0x08;                  // T1M = 1; SCA1:0 = xx
   } else if (SYSCLK/BAUDRATE/2/256 < 4) {
      TH1 = -(SYSCLK/BAUDRATE/2/4);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 01
      CKCON |=  0x01;
   } else if (SYSCLK/BAUDRATE/2/256 < 12) {
      TH1 = -(SYSCLK/BAUDRATE/2/12);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 00
   } else if (SYSCLK/BAUDRATE/2/256 < 48) {
      TH1 = -(SYSCLK/BAUDRATE/2/48);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 10
      CKCON |=  0x02;
   } else {
      while (1);                       // Error.  Unsupported baud rate
   }

   TL1 = TH1;                          // init Timer1
   TMOD &= ~0xf0;                      // TMOD: timer 1 in 8-bit autoreload
   TMOD |=  0x20;
   TR1 = 1;                            // START Timer1
   IP |= 0x10;                         // Make UART high priority
   ES0 = 1;                            // Enable UART0 interrupts
}

void Ext_Interrupt_Init (void)
{
   TCON = 0x05;                        // /INT 0 and /INT 1 are edge triggered

   IT01CF = 0x76;                      // /INT0 active low; /INT0 on P0.6;
                                       // /INT1 active low; /INT1 on P0.7;

   EX0 = 1;                            // Enable /INT0 interrupts
   EX1 = 1;                            // Enable /INT1 interrupts
}
