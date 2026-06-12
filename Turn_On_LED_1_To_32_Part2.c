#include <18F6722.h>
#fuses HS,MCLR
#use delay(clock=40MHz)

#define EXP_OUT_ENABLE_32LED        PIN_B3   //MBI_LE0
#define EXP_OUT_CLOCK         PIN_D6   //MBI_SCK
#define EXP_OUT_DO            PIN_D4   //MBI_SDI

#define G_ALL                 PIN_C5   //MBI_OE

#include <74595.c>
#define NUMBER_OF_74595_32_LEDs       4
#include <74595_32_LEDs.c>

unsigned int8 varCode[4];
unsigned int32 x=0;
unsigned int8 i;

void main() 
{
output_low(G_ALL);

while(TRUE)
{
 
for (i = 0; i<32;i++)
{
   x = (x << 1) + 0x01;
   
   varCode[0] = x >> 0;
   varCode[1] = x >> 8;
   varCode[2] = x >> 16;
   varCode[3] = x >> 24;
   write_expanded_outputs_32LED(varCode);
   delay_ms(100);
}

for (i =0; i<32;i++)
{
   x = (x >> 1);
   
   varCode[0] = x >> 0;
   varCode[1] = x >> 8;
   varCode[2] = x >> 16;
   varCode[3] = x >> 24;
   write_expanded_outputs_32LED(varCode);
   delay_ms(100);
}

}
}

