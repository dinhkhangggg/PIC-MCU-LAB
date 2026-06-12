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
//#include <74595_32_LEDs.c>
#include <74595_MA7DOAN.c>

// M? 7 o?n cho cathode chung (0..9)
int8 array[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

unsigned int8 varCode[8];
unsigned int8 varCode0[2];
unsigned int16 SP=0;   // ph?i dng 16-bit ? ?m ng

void main() 
{
   

   output_low(G_ALL);
   //output_low(EXP_OUT_ENABLE_MA7DOAN);       // b?t LED 7 o?n

   varCode0[0]=0; 
   varCode0[1]=0;
   
   setup_timer_0(T0_EXT_L_TO_H | T0_DIV_1);  // Counter, ?m c?nh ln
   set_timer0(0);
   while(TRUE)
   {
      setup_timer_0(T0_EXT_L_TO_H | T0_DIV_1);
      SP = get_timer0(); 

      // hi?n th? t?i a 2 ch? s?
      varCode[0] = array[SP % 10];         
      varCode[1] = array[(SP / 10) % 10];  
      varCode[2] = 0x00;
      varCode[3] = 0x00;
      varCode[4] = 0x00;
      varCode[5] = 0x00;
      varCode[6] = 0x00;
      varCode[7] = 0x00;
        
      write_expanded_outputs_MA7DOAN(varCode);
      write_expanded_outputs_BUZZER(varCode0);

      delay_ms(200);
   }
}

