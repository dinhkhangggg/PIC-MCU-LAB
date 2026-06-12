#ifndef  __TV_DS1307_C__
#define  __TV_DS1307_C__
#include<TV_DS1307.h>
result ds1307_read_time()
{  
      static int8 old_minute=0xff;
      soft_i2c_start();
      if(soft_i2c_write(0xd0)==ERROR) return ERROR;  
      soft_i2c_write(0);      
      soft_i2c_start();                          
      soft_i2c_write(0xd1);   
      ds1307.second             =  soft_i2c_read(1); 
      if(ds1307.minute!=old_minute)
      {
           ds1307.minute        =  soft_i2c_read(1);  
           ds1307.hour          =  soft_i2c_read(1);
           ds1307.day_of_week   =  soft_i2c_read(1);  
           ds1307.date          =  soft_i2c_read(1); 
           ds1307.month         =  soft_i2c_read(1);
           ds1307.year          =  soft_i2c_read(0);
           old_minute           =  ds1307.minute;
      }
      else ds1307.minute        =  soft_i2c_read(0);
      soft_i2c_stop();
      return OK;     
}
result ds1307_set_time(DS1307_STRUCT t)
{
      soft_i2c_start();
      if(soft_i2c_write(0xd0)==ERROR) return ERROR;
      soft_i2c_write(0);                                    
      soft_i2c_write(t.second);
      soft_i2c_write(t.minute);
      soft_i2c_write(t.hour);
      soft_i2c_write(t.day_of_week);
      soft_i2c_write(t.date);
      soft_i2c_write(t.month);
      soft_i2c_write(t.year);
      soft_i2c_stop();
      return OK;
}

result ds1307_battery_check()
{
    if(ds1307_read_time()==OK)                             
    {
         if(ds1307.second>0x59) return ERROR;
         else                   return OK;
        
    }
    return ERROR;
} 
#endif
