#ifndef  __TV_HX711_C__
#define  __TV_HX711_C__
#include<TV_HX711.h>
result hx711_read()
{
    signed int32 kq=0;
    unsigned int8 i=0;
    output_high( HX711_DO);
    output_low( HX711_CLK);
    while(input(HX711_DO));
    for (i=0;i<24;i++)
    {
      output_high( HX711_CLK);
      kq=kq<<1; 
      output_low( HX711_CLK);
      if(input(HX711_DO)) kq++; 
    }
    output_high( HX711_CLK);
    kq=kq^0x800000;
    output_low( HX711_CLK);
    hx711.weight = (signed int8) (kq/41773 -200);
   if(hx711.weight>=0) return OK; else return ERROR;
}
#if     ((BOARD ==  3)||(BOARD==2))
#error   "Board thuc hanh D501 va D503 khong co module nay!"
#endif
#if      BOARD == 1
#warning "Do dac diem moi board khac nhau nen SV mo thu vien "tv_hx711.c" chinh lai cac he so cua code hang 21 de duoc ket qua can nang chinh xac "
#endif
#endif
