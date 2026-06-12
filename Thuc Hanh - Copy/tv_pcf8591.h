#ifndef  __TV_PCF8591_H__
#define  __TV_PCF8591_H__
typedef struct
{
      unsigned int8 adc_value[4];
}PCF8591_STRUCT;
PCF8591_STRUCT pcf8591;
result pcf8591_read_adc(int8 channel_0_3);
result pcf8591_write_dac( int8 value);
#endif
