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
   int8 select = 0;  
   int8 NUM1, NUM2;

   output_low(G_ALL);

   varCode0[0] = 0;  
   varCode0[1] = 0;

   // ==== ?c l?i t? EEPROM khi kh?i d?ng ====
   NUM1 = read_eeprom(0);
   NUM2 = read_eeprom(1);

   if(NUM1 > 9)  NUM1 = 0;      // trnh d? li?u rc
   if(NUM2 > 99) NUM2 = 0;

   while(TRUE) 
   {
      // ---- Nt SELECT ----
      if(input(btn1)==0) 
      {
         delay_ms(20); 
         if(input(btn1)==0) 
         {
            select ^= 1; 
            while(input(btn1)==0); 
         }
      }

      // ---- Nt ADJ ----
      if(input(btn0)==0) 
      {
         delay_ms(20);
         if(input(btn0)==0) 
         {
            if(select==0) { 
               NUM1 = (NUM1+1)%10;
               write_eeprom(0, NUM1);   // luu l?i ngay
            } 
            else 
            {
               NUM2 = (NUM2+1)%100;
               write_eeprom(1, NUM2);   // luu l?i ngay
            }
            while(input(btn0)==0);
         }
      }

      // ---- Hi?n th? ----
      varCode[0] = array[NUM1];
      varCode[1] = array[NUM2 % 10];
      varCode[2] = array[NUM2 / 10];

      // Hi?n d?u ch?m t?i s? dang ch?nh
      if(select==0) 
      {
         varCode[0] |= 0x80; 
      } 
      else 
      {
         varCode[1] |= 0x80; 
      }

      varCode[3]=varCode[4]=varCode[5]=varCode[6]=varCode[7]=0x00;

      write_expanded_outputs_MA7DOAN(varCode);
      write_expanded_outputs_BUZZER(varCode0); // buzzer lun t?t
   }
}

