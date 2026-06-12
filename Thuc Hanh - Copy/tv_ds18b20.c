#ifndef  __TV_DS18B20_C__
#define  __TV_DS18B20_C__
#include<TV_DS18B20.h>
result DS18B20_SEARCH_ROM()
{
   int8
      i,                           
      last_desc_pos = 0,              
      desc_marker = 0,                 
      contents[8] = {0,0,0,0,0,0,0,0};          
   int1
      bitA,                            
      bitB,                            
      last_desc = 0;                   
   int1
      firstrom = TRUE,
      done = FALSE;
   if(touch_present())
   {
      do
      {
         last_desc_pos = desc_marker;
         reset_pulse();
         touch_write_byte(0xF0);
         for(i = 0; i < 64; i++)
         {
            bitA = touch_read_bit(); 
            bitB = touch_read_bit(); 
            if((!bitA) && (!bitB))
            {
               desc_marker = i;
               if(i == last_desc_pos)
                  last_desc = !last_desc;
               shift_right(contents, 8, last_desc);
               touch_write_bit(last_desc);
            }
            else
            {
               shift_right(contents, 8, bitA);
               touch_write_bit(bitA);
            }
         }
         if(firstrom)
         {
            for(i = 0; i < 8; i++)
                  ds18b20.romcode[0][i] =contents[i];
            firstrom = FALSE; ds18b20.mumber_of_ds18b20++;
         }
         else
         {
            for(i = 0; i < 8; i++)ds18b20.romcode[ds18b20.mumber_of_ds18b20][i] =contents[i];
            ds18b20.mumber_of_ds18b20++;
            done = TRUE;
            for(i = 0; i < 8; i++)
               if(ds18b20.romcode[0][i] != contents[i])
                  done = FALSE;
         }
      } while(!done);
      ds18b20.mumber_of_ds18b20--;
      return OK;
   }
   return ERROR;
}
result DS18B20_MATCH_ROM(int8 *romdata)
{
   int8  i;
   touch_write_byte(0x55);
   for(i = 0; i < 8; i++)
      touch_write_byte(romdata[i]);
   return OK;
}
       
result DS18B20_SINGLE_READ_TEMP()
{
    unsigned int8 ndh,ndl;
    if(touch_present())          //1. khoi dong
    {
       touch_write_byte(0xCC);   //2.  skip rom
       touch_write_byte(0x44);   //3.  yeu cau chuyen doi nhiet do
    }
    if(touch_present())          // 1. khoi dong
    {
       touch_write_byte(0xCC);   //2.  skip rom
       touch_write_byte(0xBE);   //3.  doc RAM
       ndl= touch_read_byte();   // byte thap nhiet do
       ndh= touch_read_byte();   // byte cao nhiet do
       ds18b20.temperatureA = make16(ndh,ndl);   // ghep lai thanh gia tri nhiet do 16 bit
       return  OK; 
    }
    return ERROR;
}

result  DS18B20_MULTI_READ_TEMP()
{
    unsigned int8 ndh,ndl;
    static int1 cb=1; cb=!cb;
    if(cb==0)
    {
       if(touch_present())
       {
          touch_write_byte(0xCC);
          touch_write_byte(0x44);
       }
    }
    if(touch_present())
    {
       DS18B20_MATCH_ROM(ds18b20.romcode[cb]);
       touch_write_byte(0xBE);
       ndl= touch_read_byte();
       ndh= touch_read_byte();
       if(cb==0) ds18b20.temperatureA= make16(ndh,ndl);
       else      ds18b20.temperatureB= make16(ndh,ndl);
       return  OK; 
    }
    return ERROR;
}
result  ds18b20_read_temperature()
{     static int1 ttsearchrom=0;
      if(ttsearchrom==0){ttsearchrom=1;if(DS18B20_SEARCH_ROM()==ERROR) return ERROR;}
      if(ds18b20.mumber_of_ds18b20==1) return  DS18B20_SINGLE_READ_TEMP();
      else                             return  DS18B20_MULTI_READ_TEMP();     
}
result  ds18b20_set_resolution(int8 res_9_12)
{
        const unsigned int8 res[]={0x1f,0x3f,0x5f,0x7f};
        if ((res_9_12 < 9)||(res_9_12>12)) return ERROR;
        if(touch_present())
        {
          touch_write_byte(0xCC);
          touch_write_byte(0x4e);
          touch_write_byte(0);
          touch_write_byte(0);
          touch_write_byte(res[res_9_12-9]);
          if(touch_present())
          {
             touch_write_byte(0xCC);
             touch_write_byte(0x48);
             return OK;
          } 
       }
       return ERROR;
}
#endif

//xxxxxxxxxxxxxxxxNeu muon set do phan giai rieng cho tung cam bien thi dung ham nayxxxxxxxxxxxxxx

//!result  ds18b20_multi_set_resolution(unsigned int8 n, int8 res )
//!{
//!       const unsigned int8 res[]={0x1f,0x3f,0x5f,0x7f};
//!       if(touch_present())
//!       {
//!          DS18B20_MATCH_ROM(ds18b20.romcode[n]);
//!          touch_write_byte(0x4e);
//!          touch_write_byte(0);
//!          touch_write_byte(0);
//!          touch_write_byte(res[res-9]);
//!          if(touch_present())
//!          {
//!             DS18B20_MATCH_ROM(ds18b20.romcode[n]);
//!             touch_write_byte(0x48);
//!             return OK;
//!          } 
//!       }
//!       return ERROR;
//!}
