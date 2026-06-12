#include <18F6722.h>
#include <stdlib.h>        // dng chu?n th?ng l ?
#fuses HS,MCLR
#use delay(clock=40MHz)

#define EXP_OUT_ENABLE_BUZZER  PIN_F4
#define EXP_OUT_CLOCK          PIN_D6
#define EXP_OUT_DO             PIN_D4
#define G_ALL                  PIN_C5
#define EXP_OUT_ENABLE_32LED   PIN_B3
#define EXP_OUT_ENABLE_MA7DOAN PIN_F6

#define NUMBER_OF_74595_Buzzer   2
#define NUMBER_OF_74595_32_LEDs    4
#define NUMBER_OF_74595_MA7DOAN  8

#include <74595_Buzzer.c>
#include <74595_32_LEDs.c>
#include <74595_MA7DOAN.c>

// B?ng m? 7 o?n cho lo?i Cathode chung
int8 array[10] = {0x3F,0x06,0x5B,0x4F,0x66,
                  0x6D,0x7D,0x07,0x7F,0x6F};
#define MINUS 0x40   // segment G

unsigned int8 varCode[8];
unsigned int8 varCode0[2];  // buzzer

void display_number(signed int16 num)
{
   int8 tens, unit;

   varCode[0]=varCode[1]=varCode[2]=varCode[3]=varCode[4]=varCode[5]=varCode[6]=varCode[7]=0x00;

   if(num >= 0)
   {
      tens = (int8)(num / 10);
      unit = (int8)(num % 10);

      if(num < 10) 
      {
         varCode[0] = array[unit];
      } else 
      {
         varCode[0] = array[unit];
         varCode[1] = array[tens];
      }
   }
   else
   {
      num = -num;  
      tens = (int8)(num / 10);
      unit = (int8)(num % 10);

      if(num < 10) 
      {

         varCode[0] = array[unit];
         varCode[1] = MINUS;
      } 
      else 
      {
         // hi?n th? "-25" = [LED2: '-', LED1: '2', LED0: '5']
         varCode[0] = array[unit];
         varCode[1] = array[tens];
         varCode[2] = MINUS;
      }
   }

   write_expanded_outputs_MA7DOAN(varCode);
}

void main()
{
   signed int16 r;                 // !!! signed
   output_low(G_ALL);

   varCode0[0] = 0;
   varCode0[1] = 0;

   srand(12345);
   
   while(TRUE)
   {
      r = (signed int16)(rand() % 101) - 50;

      display_number(r);
      write_expanded_outputs_BUZZER(varCode0);   // lun t?t buzzer

      delay_ms(500);
   }
}

