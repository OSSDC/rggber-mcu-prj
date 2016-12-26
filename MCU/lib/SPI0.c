#include <compiler_defs.h>
#include <C8051F410_defs.h>                // SFR declarations
#include <SPI0.h>

SBIT (SCK, SFR_P1, 0);
SBIT (MISO, SFR_P1, 1);
SBIT (MOSI, SFR_P1, 2);
SBIT (NSS, SFR_P1, 3);

void SPI0_2BYTE_WRITE(U8 spi_addr, U8 spi_dat)
{
		U8 i;
		NSS = 0;
		SCK = 0;
		NOP();
	  NOP();
		NOP();
		NOP();
		NOP();
		for(i = 0; i < 8; i++)
		{
				if(spi_addr & 0x80)
				{
						MOSI = 1;
				}
				else
					MOSI = 0;
				spi_addr = spi_addr << 1;
				SCK = 1;
				NOP();
				NOP();
				NOP();
				NOP();
				NOP();
				SCK = 0;
				NOP();
				NOP();
				NOP();
				NOP();
				NOP();
		}
		for(i = 0; i < 8; i++)
		{
				if(spi_dat & 0x80)
				{
						MOSI = 1;
				}
				else
					MOSI = 0;
				spi_dat = spi_dat << 1;
				SCK = 1;
				NOP();
				NOP();
				NOP();
				NOP();
				NOP();
				SCK = 0;
				NOP();
				NOP();
				NOP();
				NOP();
				NOP();
		}
		NSS = 1;	
}