
///////////////////////////////////////////////////////

extern void Start_I2c0();
extern void Stop_I2c0();
extern void SendByte0(U8 c);
extern U8 RcvByte0();
extern void Ack_I2c0(bit a);
extern void delay_2us0();
extern U8 I2C_READ_EEPROM(U8 suba);
extern void I2C_WRITE_EEPROM(U8 suba,U8 dat);
extern U8 I2C_READ_TFP410(U8 suba);
extern void I2C_WRITE_TFP410(U8 suba,U8 dat);

///////////////////////////////////////////////////////

extern void Start_I2c1();
extern void Stop_I2c1();
extern void SendByte1(U8 c);
extern U8 RcvByte1();
extern void Ack_I2c1(bit a);
extern void delay_2us1();
extern void I2C_WRITE_OV5640(U16 suba,U8 dat);
extern U8 I2C_READ_OV5640(U16 suba);

///////////////////////////////////////////////////////

extern void delay_ms();

///////////////////////////////////////////////////////


