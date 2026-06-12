#include <18F6722.h>
#include <STDLIB.H>
#fuses HS,MCLR
#use delay(clock=40MHz)

#define EXP_OUT_ENABLE_BUZZER  PIN_F4
#define EXP_OUT_CLOCK          PIN_D6
#define EXP_OUT_DO             PIN_D4
#define G_ALL                  PIN_C5
#define EXP_OUT_ENABLE_32LED   PIN_B3
#define EXP_OUT_ENABLE_MA7DOAN PIN_F6

#define NUMBER_OF_74595_Buzzer 2
#define NUMBER_OF_74595_32_LEDs  4
#define NUMBER_OF_74595_MA7DOAN 8

#include <74595_Buzzer.c>
#include <74595_32_LEDs.c>
#include <74595_MA7DOAN.c>

int8 array[10]={0x3F,0x06,0x5B,0x4F,0x66,
                0x6D,0x7D,0x07,0x7F,0x6F};

unsigned int8 varCode[8];
unsigned int8 varCode0[2];

void display_number(int8 num) 
{
   int8 tens, unit;

   tens = num / 10;
   unit = num % 10;

   if(num < 10) 
   {
      varCode[0] = array[unit];
      varCode[1] = 0x00;
   } 
   else 
   {
      varCode[0] = array[unit];
      varCode[1] = array[tens];
   }

   varCode[2]=varCode[3]=varCode[4]=varCode[5]=varCode[6]=varCode[7]=0x00;

   write_expanded_outputs_MA7DOAN(varCode);
}

void main() 
{
   int8 r;

   output_low(G_ALL);

   varCode0[0]=0;
   varCode0[1]=0;

   r = read_eeprom(0);
   
   if(r > 90)
      r = 0;

   display_number(r);

   srand(r); 

   while(TRUE) 
   {
      delay_ms(500);  

      r = rand() % 91;   // 090

      write_eeprom(0, r); // luu xuong EEPROM

      display_number(r);
      write_expanded_outputs_BUZZER(varCode0); // lun tat buzzer
   }
}
