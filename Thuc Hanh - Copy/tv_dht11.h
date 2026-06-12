#ifndef  __TV_DHT11_H__
#define  __TV_DHT11_H__
typedef struct
{ 
  unsigned int8  humidity_integral;
  unsigned int8  humidity_decimal;
  unsigned int8  temperature_integral;
  unsigned int8  temperature_decimal;
} DHT11_STRUCT;  
DHT11_STRUCT dht11;

result  dht11_read(void);
#endif
