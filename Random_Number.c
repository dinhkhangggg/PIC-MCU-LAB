#include <18F6722.h>
#fuses HS,MCLR
#use delay(clock=40MHz)
#include <STDLIB.H>
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
   int16 r;
   int8 tens, unit;

   output_low(G_ALL);

   varCode0[0]=0; // tat buzzer
   varCode0[1]=0;

   srand(r);  // seed ngau nhin tu timer

   while(TRUE) {
      // Tao so ngau nhin 090
      r = rand() % 91;  
      // Tch so
      tens = r / 10;
      unit = r % 10;

      // Hien thi
      if(r < 10) 
      {
         varCode[0] = array[unit];
         varCode[1] = 0x00;  // tat LED hng chuc
      } else 
      {
         varCode[0] = array[unit]; // hng don vi
         varCode[1] = array[tens]; // hang don vi
      }

      varCode[2]=varCode[3]=varCode[4]=varCode[5]=varCode[6]=varCode[7]=0x00;

      write_expanded_outputs_MA7DOAN(varCode);
      write_expanded_outputs_BUZZER(varCode0);

      delay_ms(500); // ?i s? m?i giy
   }
}
