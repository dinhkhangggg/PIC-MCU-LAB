#include <18F6722.h>
#fuses HS,MCLR
#use delay(clock=40MHz)

#define EXP_OUT_ENABLE_32LED  PIN_B3   
#define EXP_OUT_ENABLE_BUZZER PIN_F4   
#define EXP_OUT_CLOCK         PIN_D6   
#define EXP_OUT_DO            PIN_D4   
#define G_ALL                 PIN_C5 

#define NUMBER_OF_74595_32_LEDs 4
#include <74595_32_LEDs.c>

#define NUMBER_OF_74595_Buzzer 2
#include <74595_Buzzer.c>

unsigned int8 varCode[4];
unsigned int8 varCode1[2];
unsigned int8 I = 0;
unsigned int32 S = 0;

void main() 
{
    output_low(G_ALL);
    output_high(PIN_B3);
    output_high(PIN_F4);

    varCode1[0] = 0x00;
    varCode1[1] = 0x00;

    while(TRUE)
    {
        S = 0;  
        for(i=0; i<16; i++)
        {
            S = (S << 1) + 0x01;   
            varCode[0] = S & 0xFF;
            varCode[1] = (S >> 8) & 0xFF;
            
            write_expanded_outputs_32LED(varCode);
         

          
            if(i==7 || i==15)
            {
                varCode1[1] = 0b00100000;  // Buzzer ON
                   write_expanded_outputs_BUZZER(varCode1);
             
            }
            else
            {
               varCode1[1] = 0x00; 
                  write_expanded_outputs_BUZZER(varCode1);// Buzzer OFF
            }
            
                  delay_ms(300);
        }
    }
}

