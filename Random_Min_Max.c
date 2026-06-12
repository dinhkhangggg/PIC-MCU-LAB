#include <18F6722.h>
#include <stdlib.h>
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

// M? s? cho LED 7 o?n cathode chung (09)
int8 array[10] = {0x3F,0x06,0x5B,0x4F,0x66,
                  0x6D,0x7D,0x07,0x7F,0x6F};

unsigned int8 varCode[8];
unsigned int8 varCode0[2];  // buzzer

void main()
{
   unsigned int8 r;
   unsigned int8 min = 99, max = 0;
   unsigned int8 tens, unit;

   output_low(G_ALL);

   // T?t buzzer
   varCode0[0] = 0;
   varCode0[1] = 0;

   srand(1234); // seed c? ?nh (n?u mu?n khc nhau th? dng timer/adc)

   while(TRUE)
   {
      // Sinh s? ng?u nhin [0..50]
      r = rand() % 100;

      // C?p nh?t min/max
      if(r < min) min = r;
      if(r > max) max = r;

      // Xa h?t varCode
      for(int i=0;i<8;i++) varCode[i]=0x00;

      // ==== Max ? LED1,2 ====
      tens = max / 10;
      unit = max % 10;
      varCode[0] = array[unit];              // LED1
      varCode[1] = (max >= 10) ? array[tens] : 0x00; // LED2

      // ==== Random ? LED4,5 ====
      tens = r / 10;
      unit = r % 10;
      varCode[3] = array[unit];              // LED4
      varCode[4] = (r >= 10) ? array[tens] : 0x00;      // LED5

      // ==== Min ? LED7,8 ====
      tens = min / 10;
      unit = min % 10;
      varCode[6] = array[unit];              // LED7
      varCode[7] = (min >= 10) ? array[tens] : 0x00; // LED8

      // Xu?t ra LED + buzzer
      write_expanded_outputs_MA7DOAN(varCode);
      write_expanded_outputs_BUZZER(varCode0);

      delay_ms(500); // ?i s? m?i 0.5 giy
   }
}

