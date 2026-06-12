#ifndef  __TV_LOWLEVER_C__
#define  __TV_LOWLEVER_C__
#include <TV_LOWLEVER.H>
/*********************************** Soft I2C ************************************************************/
#if      BOARD == MOPHONG
result soft_i2c_clk()
{   
      output_high(soft_i2c_scl);
      DELAY_us(1);  
      output_low(soft_i2c_scl);
      return OK;
}
result soft_i2c_start()
{
      output_high(soft_i2c_sda);
      output_high(soft_i2c_scl);
      delay_us(1);
      output_low(soft_i2c_sda);
      delay_us(1);
      output_low(soft_i2c_scl);   
      return OK;
}
result soft_i2c_stop()
{
      output_low(soft_i2c_sda);
      output_high(soft_i2c_scl);
      delay_us(1);
      output_high(soft_i2c_sda);
      return OK;
}
result soft_i2c_write(unsigned int8 b)
{
      unsigned int8 m;
      int1 ack;
      for(m=0x80;m>0;m>>=1)
      {
            output_bit( soft_i2c_sda,b&m);
            soft_i2c_clk();
      }
      output_float(soft_i2c_sda);
      output_high(soft_i2c_scl);  
      delay_us(1);
      ack = input(soft_i2c_sda);
      output_low(soft_i2c_scl);  
      return ack;
}

unsigned int8 soft_i2c_read(int1 ack)
{ 
      unsigned int8 n,nhan=0;
      output_float(soft_i2c_sda);
      for(n=0x80;n>0;n>>=1)
      {      
            output_high(soft_i2c_scl);  
            delay_us(1);
            if(input(soft_i2c_sda))nhan=nhan|n;
            output_low(soft_i2c_scl);   
      }
      if(ack)  output_low(soft_i2c_sda);
      else     output_high(soft_i2c_sda); 
      soft_i2c_clk();
      output_high(soft_i2c_sda); 
      return nhan;
}
#else 
void soft_i2c_clk()
{    
      output_high(soft_i2c_scl);
      output_low(soft_i2c_scl);
}
void soft_i2c_start()
{
      output_high(soft_i2c_sda);
      output_high(soft_i2c_scl);
      output_low(soft_i2c_sda);
      output_low(soft_i2c_scl);      
}
void soft_i2c_stop()
{
      output_low(soft_i2c_scl);
      output_low(soft_i2c_sda);
      output_high(soft_i2c_scl);
      output_high(soft_i2c_sda);
}
result soft_i2c_write(unsigned int8 b)
{
      unsigned int8 m,dem=0;
      for(m=0x80;m>0;m>>=1)
      {
            output_bit(soft_i2c_sda,b&m);
            soft_i2c_clk();
      }
     output_float(soft_i2c_sda);
     output_high(soft_i2c_scl);
     while(input(soft_i2c_sda)&(dem<5)){dem++; delay_us(1);}
     output_low(soft_i2c_scl);
     if(dem==5) return ERROR;
     else        return OK;
}

unsigned int8 soft_i2c_read(int1 ack)
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
#endif
/*********************************** Delay ************************************************************/
unsigned int16 delay_start()
{    
      delay.timeline += get_timer3() ;set_timer3(2);
      if(TIM3IF) delay.timeline +=65536;TIM3IF=0;
      return delay.timeline/625;
}

unsigned int16 delay_stop(unsigned int16 t)
{    
      unsigned int16 tam;
      delay.timeline += get_timer3() ;set_timer3(2);
      if(TIM3IF) delay.timeline +=65536;TIM3IF=0;
      tam = delay.timeline/625;
      if(t<=tam) return tam-t;
      else       return 0xffff-t + tam;
}
/*********************************** Button ************************************************************/

result inputcd(int16 pin)
{
      static unsigned int16 oldpin=0xffff;
      static unsigned int16 t; 
      static unsigned int8 step=0;
      output_float(pin);
      switch (step)
      {
          case 0:
                 if(input(pin)==0)
                  {
                        oldpin   = pin;
                        t        = delay_start();
                        step     = 1;
                  }
                 break;
           case 1:
                 if(input(oldpin))step = 0;
                 else if((pin==oldpin)&&(delay_stop(t)>=BUTTON_HOLD_TIME))
                   {
                      t    = delay_start();
                      step = 2;
                      return OK;
                   }
                 break;
            case 2:
                 if(input(oldpin))step = 0;
                 else if((pin==oldpin)&&(delay_stop(t)>=500))
                   {
                       t        = delay_start();
                       step     = 3;
                   } 
                 break;
            case 3:
                   if(input(oldpin))step = 0; 
                   else if((pin==oldpin)&&(delay_stop(t)>=100))
                       {
                          t  = delay_start();
                          return OK;
                       }
                 break;
      }   
      return BUSY; 
}
#endif
