#ifndef  __TV_HX711_H__
#define  __TV_HX711_H__
typedef struct 
{
      signed int8  weight;          
}pivate_hx711_struct;
pivate_hx711_struct hx711;
result hx711_read(void);
#endif
