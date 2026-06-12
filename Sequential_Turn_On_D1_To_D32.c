#include <18F6722.h>
#fuses HS,MCLR
#use delay(clock=40MHz)
#define EXP_OUT_ENABLE  PIN_B3   //MBI_LE0
#define EXP_OUT_CLOCK         PIN_D6   //MBI_SCK
#define EXP_OUT_DO            PIN_D4   //MBI_SDI

#define G_ALL                 PIN_C5   //MBI_OE

#define NUMBER_OF_74595 4
#include <74595.c>

unsigned int8 varCode[4];
unsigned int8 varCount;
unsigned int8 arySD[9]={0b00000000,
                        0b00000001,
                        0b00000011,
                        0b00000111,
                        0b00001111,
                        0b00011111,
                        0b00111111,
                        0b01111111,
                        0b11111111};
void main() 
{
output_low(G_ALL);
while(TRUE)
{
for(varCount=0;varCount<9;varCount++)
{
varCode[0]=arySD[varCount];
varCode[1]=0b00000000;
varCode[2]=0b00000000;
varCode[3]=0b00000000;
write_expanded_outputs(varCode);
delay_ms(200);
}

for(varCount=0;varCount<9;varCount++)
{
varCode[0]=0b11111111;
varCode[1]=arySD[varCount];
write_expanded_outputs(varCode);
delay_ms(200);
}
for(varCount=0;varCount<9;varCount++)
{
varCode[0]=0b11111111;
varCode[1]=0b11111111;
varCode[2]=arySD[varCount];
write_expanded_outputs(varCode);
delay_ms(200);
}
for(varCount=0;varCount<9;varCount++)
{
varCode[0]=0b11111111;
varCode[1]=0b11111111;
varCode[2]=0b11111111;
varCode[3]=arySD[varCount];
write_expanded_outputs(varCode);
delay_ms(200);
}
}
}
