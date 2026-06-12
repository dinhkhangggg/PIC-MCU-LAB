#include <18F6722.h>
#fuses HS,NOWDT,NOPROTECT,NOLVP,MCLR
#use delay(clock=40MHz)

#define EXP_OUT_ENABLE_BUZZER  PIN_F4
#define EXP_OUT_CLOCK          PIN_D6
#define EXP_OUT_DO             PIN_D4
#define G_ALL                  PIN_C5
#define EXP_OUT_ENABLE_MA7DOAN PIN_F6

#define NUMBER_OF_74595_Buzzer   2
#define NUMBER_OF_74595_32_LEDs    4
#define NUMBER_OF_74595_MA7DOAN  8

#include <74595_Buzzer.c>
#include <74595_MA7DOAN.c>

// B?ng m? LED 7 o?n cho cathode chung (0..9)
int8 array[10] = {0x3F,0x06,0x5B,0x4F,0x66,
                  0x6D,0x7D,0x07,0x7F,0x6F};

unsigned int8 varCode[8];
unsigned int8 varCode0[2];

unsigned int16 totalSP = 0;  // t?ng s?n ph?m
unsigned int8  SP = 0;       // s?n ph?m trong h?p
unsigned int16 BOX = 0;      // s? h?p ?y

void main() 
{
   output_low(G_ALL);
   varCode0[0]=0; 
   varCode0[1]=0;

   setup_timer_0(T0_EXT_L_TO_H | T0_DIV_1);
   set_timer0(0);

   while(TRUE)
   {
      totalSP = get_timer0();      // ?c s? sp t? Timer0
      BOX = totalSP / 24;          // tnh s? h?p
      SP  = totalSP % 24;          // tnh s? s?n ph?m trong h?p

      // Xa LED tr?c
      for(int i=0;i<8;i++) varCode[i]=0x00;

      // Hi?n th? SP (s?n ph?m trong h?p) ? LED0, LED1 (bn ph?i)
      varCode[0] = array[SP % 10];
      varCode[1] = array[(SP / 10) % 10];

      // Hi?n th? BOX (s? h?p) ? LED2, LED3 (bn tri)
      varCode[2] = array[BOX % 10];
      varCode[3] = array[(BOX / 10) % 10];

      // Cc LED c?n l?i t?t
      varCode[4] = varCode[5] = varCode[6] = varCode[7] = 0x00;

      // Xu?t d? li?u
      write_expanded_outputs_MA7DOAN(varCode);
      write_expanded_outputs_BUZZER(varCode0);

      delay_ms(200);
   }
}

