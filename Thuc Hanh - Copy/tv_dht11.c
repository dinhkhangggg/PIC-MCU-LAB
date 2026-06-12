#ifndef  __TV_DHT11_C__
#define  __TV_DHT11_C__
#include<TV_DHT11.h>
unsigned int8 DHT11_READ_1BYTE()
{
     unsigned int8 kq=0;
     for(int8 n=0x80;n!=0;n>>=1)
        {
            while(!input(DHT11_pin)){}  delay_us(30);
            if(input(DHT11_pin)){kq|=n; delay_us(50);}
        } 
      return kq;
}
result dht11_read()
{ 
   static unsigned int8  step=0;
   static unsigned int16 t;
   result res= BUSY;
   switch (step)
         {
             case 0:
                 output_low(dht11_Pin);
                 t    = delay_start();
                 step = 1;
                 break;
             case 1:
                 if(delay_stop(t)>=18)
                 {
                      output_high(dht11_Pin);
                      delay_us(50);
                      if(input(dht11_Pin)==0) 
                      {   delay_us(80);
                          if(input(dht11_Pin)==1) 
                          {  delay_us(80);
                            dht11.humidity_integral     =  DHT11_READ_1BYTE();
                            dht11.humidity_decimal      =  DHT11_READ_1BYTE();
                            dht11.temperature_integral  =  DHT11_READ_1BYTE();
                            dht11.temperature_decimal   =  DHT11_READ_1BYTE();
                            if((dht11.humidity_integral+dht11.humidity_decimal+dht11.temperature_integral+dht11.temperature_decimal)==DHT11_READ_1BYTE())
                            res= Ok; 
                            else res= ERROR;
                          }
                      }
                      t= delay_start();
                      step= 2;
                 }
                 break;
             case 2:
                 if(delay_stop(t)>=100) step=0;
                 break;
         }
    return res;    
}
#endif
