#include <18F6722.h>
#fuses HS,MCLR
#use delay(clock=40MHz)
#define EXP_OUT_ENABLE_8L7D_MBI  PIN_F6   //MBI_LE1
#define EXP_OUT_CLOCK            PIN_D6   //MBI_SCK
#define EXP_OUT_DO               PIN_D4   //MBI_SDI

#define G_ALL                    PIN_C5   //MBI_OE

#define NUMBER_OF_74595_8_LED_7_Segment_MBI 8
#include <74595_8_LED_7_Segment_MBI.c>
//                           '0'  '1'  '2'  '3'  '4'  '5'  '6'  '7'  '8'  '9'
const char aryCode7Seg[16]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,
//                           'A'  'b'  'C'  'd'  'E'  'F'
                            0x88,0x83,0xC6,0xA1,0x86,0x8E};

char varCode[8];
unsigned int8 varNum1;
unsigned int16 varNum2;
void main() 
{
output_low(G_ALL);
varCode[7]=0b01110011;//'P'
varCode[6]=0b00000110;//'I'
varCode[5]=~0xC6;//'C'
varCode[4]=0x00;

varCode[3]=~0xA1;
varNum2=401;
varCode[2]=~aryCode7Seg[varNum2/100];
varCode[1]=~aryCode7Seg[(varNum2%100)/10];
varCode[0]=~aryCode7Seg[(varNum2%100)%10];
write_expanded_outputs_8L7D_MBI(varCode);

while(TRUE)
{

}
}
