#include <18F6722.h>
#fuses HS,MCLR
#use delay(clock=40MHz)
#define EXP_OUT_ENABLE_BUZZER       PIN_F4   //MBI_LE0
#define EXP_OUT_CLOCK         PIN_D6   //MBI_SCK
#define EXP_OUT_DO            PIN_D4   //MBI_SDI
#define G_ALL                 PIN_C5   //MBI_OE
#define EXP_OUT_ENABLE_32LED  PIN_B3 
//#define EXP_OUT_ENABLE_BUZZER       PIN_F6 
#define NUMBER_OF_74595_Buzzer 2
#define NUMBER_OF_74595_LED32 4
//#define NUMBER_OF_74595_7 4
//#include <74595.c>
#include <74595_32_LEDs.c>
#include <74595_Buzzer.c>


unsigned int8 varCode0[2];
unsigned int8 varCode1[4];
int8 x0 = 0x00, x1 = 0x00, x2 = 0x00, x3 = 0x00;
void main() 
{  int i=0;
      output_low(G_ALL);
      output_high(PIN_F4);
      output_high(PIN_B3);
      output_low(PIN_F6);
      while(TRUE)
      {
      

         for(i=0; i<8; i++)
        {
            x0 = (x0 << 1) + 0x01;
            varCode1[0] = x0;
            write_expanded_outputs_32LED(varCode1);
            varCode0[1]=0b00100000; 
            if(i==7)
      {
         varCode0[1]=0b00100000;
        write_expanded_outputs_buzzer(varCode0);
      }
            
            delay_ms(100);
            varCode0[1]=0b00000000;
            write_expanded_outputs_buzzer(varCode0);
            
        }
        for(i=0; i<8; i++)
        {
            x1 = (x1 << 1) + 0x01;
            varCode1[0] = x0;
            varCode1[1] = x1;
            write_expanded_outputs_32LED(varCode1);
            varCode0[1]=0b00100000;
            if(i==7)
      {
         varCode0[1]=0b00100000;
        write_expanded_outputs_buzzer(varCode0);
      }
            delay_ms(100);
            varCode0[1]=0b00000000;
            write_expanded_outputs_buzzer(varCode0);
            
        }
       
            
      }
}
