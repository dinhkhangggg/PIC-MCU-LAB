#include <18F4550.h>
#fuses hs, PUT
#use delay(clock=8MHz)
   

#define EXP_OUT_ENABLE_LCD         PIN_D2  //RCK_LCD
#define EXP_OUT_ENABLE_RLB         PIN_D3  //RCK_RLB
#define EXP_OUT_ENABLE_32LED       PIN_D4  //RCK_32LED
#define EXP_OUT_ENABLE_8L7D        PIN_D5  //RCK_8L7D
#define EXP_OUT_ENABLE_4L7D        PIN_D6  //RCK_4L7D
#define EXP_OUT_ENABLE_MTL         PIN_D7  //RCK_MTL
#define EXP_OUT_CLOCK              PIN_E0  //SSCK
#define EXP_OUT_DO                 PIN_E2  //SSDO
#define G_ALL                      PIN_D0  //G_ALL
#define NUMBER_OF_74595_8L7D 2
#define NUMBER_OF_74595_32_LEDs 4
#define NUMBER_OF_74595_4L7D  4
#define NUMBER_OF_74595_RLB   1
#define NUMBER_OF_74595_MTL   1
#define NUMBER_OF_74595_LCD   1
#include <74595_8L7D.c>
#include <74595_RLB.c>
#include <74595_4L7D.c>
#include <74595_MTL.c>
#include <74595_LCD.c>
#include <74595_32_LEDsDON.c>

#define STOP pin_b3
#define RIGHT pin_b4
#define LEFT pin_b5
unsigned int32 i;

const char ma7doan[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0xff,
//                           'A'  'b'  'C'  'd'  'E'  'F'
                            0x88,0x83,0xC6,0xA1,0x86,0x8E};
    
    unsigned int32 varCode,varCode1,varCode2,varCode3,varCode4,run1,run2,save1,save2;
    unsigned int32 varSeg,varSeg1,varSeg2,varSeg3,varSeg4;
int32 RL[8]={
                        0b00000000000000000000000000000001,
                        0b00000000000000000000000000000010,
                        0b00000000000000000000000000000100,
                        0b00000000000000000000000000001000,
                        0b00000000000000000000000000010000,
                        0b00000000000000000000000000100000,
                        0b00000000000000000000000001000000,
                        0b00000000000000000000000010000000};
int32 LR[8]={
                        0b00000000000000000000000010000000,
                        0b00000000000000000000000001000000,
                        0b00000000000000000000000000100000,
                        0b00000000000000000000000000010000,
                        0b00000000000000000000000000001000,
                        0b00000000000000000000000000000100,
                        0b00000000000000000000000000000010,
                        0b00000000000000000000000000000001};
int8 OFF[8]={           0b00000000000000000000000000000000,
                        0b00000000000000000000000000000000,
                        0b00000000000000000000000000000000,
                        0b00000000000000000000000000000000,
                        0b00000000000000000000000000000000,
                        0b00000000000000000000000000000000,
                        0b00000000000000000000000000000000,
                        0b00000000000000000000000000000000};
void kiemtranut()
{
if(input(pin_b5)==0)
{
 delay_ms(3);
 if(input(pin_b5)==0)
 {
  while(input(pin_b5)==0) {}
 for(i=1; i<9 ; i++)  
{
       varCode1 = ma7doan[i];
        VarCode2 = 0xff ;
         varCode3 = 0xff ;       
       varCode4 = 0xff ;
     varCode=varCode4|(varCode3<<8)|(varCode2<<16)|(varCode1<<24);
    write_expanded_outputs_4L7D(&varcode );
    delay_ms(150);
varCode= RL[i];
write_expanded_outputs_32LED(&varCode);  
delay_ms(50);
}
 }
}
else if (input(pin_b4)==0)
{
 delay_ms(3);
 if(input(pin_b4)==0)
 {
  while(input(pin_b4)==0) {}
 for(i=1; i<9 ; i++)  
{
 varCode1 = ma7doan[i];
        VarCode2 = 0xff ;
         varCode3 = 0xff ;       
       varCode4 = 0xff ;
     varCode=varCode4|(varCode3<<8)|(varCode2<<16)|(varCode1<<24);
    write_expanded_outputs_4L7D(&varcode );
    delay_ms(150);
varCode=LR[i];
write_expanded_outputs_32LED(&varCode);  
delay_ms(50);
 }
}
}
else if (input(pin_b3)==0)
{
 delay_ms(3);
 if(input(pin_b3)==0)
 {
  while(input(pin_b3)==0) {}
for(i=0; i<8 ; i++)  
{varCode1 = 0xff;
        VarCode2 = 0xff ;
         varCode3 = 0xff ;       
       varCode4 = 0xff ;
     varCode=varCode4|(varCode3<<8)|(varCode2<<16)|(varCode1<<24);
    write_expanded_outputs_4L7D(&varcode);
varCode= OFF[i];
write_expanded_outputs_32LED(&varCode);  
delay_ms(50);
 }
}
}
 }

void main() 
{
output_low(G_ALL);
while(true)
{
kiemtranut();
}
}
