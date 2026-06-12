#ifndef  __TV_DS18B20_H__
#define  __TV_DS18B20_H__
#include"touch.c"
typedef struct 
{     unsigned int8  romcode[3][8];      
      signed int8    mumber_of_ds18b20;
      unsigned int16 temperatureA;
      unsigned int16 temperatureB;
      
}DS18B20_STRUCT;
DS18B20_STRUCT ds18b20={{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},0,0,0};
result  ds18b20_read_temperature(void);
result  ds18b20_set_resolution(int8 res_9_12);
#endif
