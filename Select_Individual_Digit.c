#include <18F6722.h>
#fuses HS,MCLR
#use delay(clock=40MHz)

#define btn1   PIN_F1   // SELECT
#define btn0   PIN_B6   // ADJ

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

void main() {
   int8 select = 0;   // 0 = NUM1, 1 = NUM2 n v?, 2 = NUM2 ch?c
   int8 NUM1 = 0;
   int8 NUM2_unit = 0;
   int8 NUM2_tens = 0;

   output_low(G_ALL);

   varCode0[0] = 0;   // buzzer off
   varCode0[1] = 0;

   while(TRUE) {
      // X? l? SELECT
      if(input(btn1)==0) {
         delay_ms(20); // debounce
         if(input(btn1)==0) {
            select++;
            if(select>2) select=0;
            while(input(btn1)==0); // ch? nh? nt
         }
      }

      // X? l? ADJ
      if(input(btn0)==0) {
         delay_ms(20);
         if(input(btn0)==0) {
            if(select==0) { // ch?nh NUM1
               NUM1 = (NUM1+1)%10;
            } 
            else if(select==1) { // ch?nh NUM2 n v?
               NUM2_unit++;
               if(NUM2_unit>9) {
                  NUM2_unit=0;
                  NUM2_tens = (NUM2_tens+1)%10; // tng hng ch?c
               }
            } 
            else if(select==2) { // ch?nh NUM2 hng ch?c
               NUM2_tens = (NUM2_tens+1)%10;
            }
            while(input(btn0)==0);
         }
      }

      // Hi?n th? NUM1, NUM2
      varCode[0] = array[NUM1];
      varCode[1] = array[NUM2_unit];
      varCode[2] = array[NUM2_tens];

      // B?t d?u ch?m cho s? ang ch?n
      if(select==0) varCode[0] |= 0x80;
      if(select==1) varCode[1] |= 0x80;
      if(select==2) varCode[2] |= 0x80;

      // Cc LED 7 o?n c?n l?i t?t
      varCode[3]=varCode[4]=varCode[5]=varCode[6]=varCode[7]=0x00;

      // Xu?t d? li?u
      write_expanded_outputs_MA7DOAN(varCode);
      write_expanded_outputs_BUZZER(varCode0); // lun t?t buzzer
   }
}
