#ifndef  __TV_PCF8591_C__
#define  __TV_PCF8591_C__
#include <TV_PCF8591.h>
void PCF8591_i2c_clk()
{     
      output_high(soft_i2c_scl);
      delay_us(1);
      output_low(soft_i2c_scl);
}
void PCF8591_i2c_start()
{
      output_high(soft_i2c_sda);
      output_high(soft_i2c_scl);
      output_low(soft_i2c_sda);
      output_low(soft_i2c_scl);   
}
void PCF8591_i2c_stop()
{
      output_low(soft_i2c_scl);
      output_low(soft_i2c_sda);
      output_high(soft_i2c_scl);
      output_high(soft_i2c_sda);
}
result PCF8591_i2c_write(unsigned int8 b)
{
      unsigned int8 m,dem=0;
      for(m=0x80;m>0;m>>=1)
      {
            output_bit(soft_i2c_sda,b&m);
            PCF8591_i2c_clk();
      }
     output_float(soft_i2c_sda);
     output_high(soft_i2c_scl);
     while(input(soft_i2c_sda)&(dem<5)){dem++; delay_us(1);}
     output_low(soft_i2c_scl);
     if(dem==5) return ERROR;
     else        return OK;
}
unsigned int8 PCF8591_i2c_read(int1 ack)
{ 
      unsigned int8 n,nhan=0;
      output_float(soft_i2c_sda);
      for(n=0x80;n>0;n>>=1)
      {    
            output_high(soft_i2c_scl);  
            if(input(soft_i2c_sda))nhan=nhan|n; 
            output_low(soft_i2c_scl);  
      } 
      output_bit(soft_i2c_sda,!ack);
      soft_i2c_clk();
      return nhan;
}

result pcf8591_read_adc(unsigned int8 channel_0_3)
{
     const unsigned int8 k[4]= {0x40,0x41,0x42,0x43};
     PCF8591_i2c_start();
     if(PCF8591_i2c_write(0x90)==ERROR) return ERROR; 
     PCF8591_i2c_write(k[channel_0_3]);
     PCF8591_i2c_stop();
     delay_us(10);
     PCF8591_i2c_start();
     if(PCF8591_i2c_write(0x91)==ERROR) return ERROR; 
     pcf8591.adc_value[channel_0_3] = PCF8591_i2c_read(0);
     PCF8591_i2c_stop();
     delay_us(100);
     return OK; 
}
result pcf8591_write_dac( int8 value)
{
     PCF8591_i2c_start();
     if(soft_i2c_write(0x90)==ERROR) return ERROR; 
     PCF8591_i2c_write(0x40);
     PCF8591_i2c_write(value);  
     PCF8591_i2c_stop();
     return OK;
}
#endif
