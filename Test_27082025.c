#include <18F6722.h>
#fuses HS,MCLR
#use delay(clock=40MHz)
#define EXP_OUT_ENABLE        PIN_B3   //MBI_LE0
#define EXP_OUT_CLOCK         PIN_D6   //MBI_SCK
#define EXP_OUT_DO            PIN_D4   //MBI_SDI

#define G_ALL                 PIN_C5   //MBI_OE

#define NUMBER_OF_74595       4
#include <74595.c>

unsigned int8 varCode[4];

void main() 
{
output_low(G_ALL);

while(TRUE)
{
varCode[0]=0b00000000;
write_expanded_outputs(varCode);
delay_ms(300);

varCode[0]=0b00000001;
write_expanded_outputs(varCode);
delay_ms(300);

}
}



