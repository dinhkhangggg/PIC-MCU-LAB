#include <18F4550.h>
#fuses HS
#use delay(clock=8MHz)
#define EXP_OUT_ENABLE_8L7D  PIN_D5   //RCK_8L7D
#define EXP_OUT_ENABLE_BUZZER    PIN_D3
#define EXP_OUT_ENABLE_MTL  PIN_D7  
#define EXP_OUT_ENABLE_LCD  PIN_D2
#define EXP_OUT_ENABLE_4L7D      PIN_D6   //RCK_4L7D
#define EXP_OUT_ENABLE_32LED     PIN_D4   //RCK_32LED
#define EXP_OUT_CLOCK            PIN_E0   
#define EXP_OUT_DO               PIN_E2

#define G_ALL                    PIN_D0   //G_ALL

//!#define NUMBER_OF_74595_8L7D 2
//!#define NUMBER_OF_74595_MTL 1
//!#define NUMBER_OF_74595_LCD 1
//!#define NUMBER_OF_74595_Buzzer 1
#define NUMBER_OF_74595_32_LEDs 4
#define NUMBER_OF_74595_4L7D 4
#include <74595_32_LEDsDON.c>
//!#include <74595_buzzer.c>
//!#include <74595_8L7D.c>
//!#include <74595_MTL.c>
//!#include <74595_LCD.c>
#include <74595_4L7D.c>

#define  BT0    PIN_B5
#define  BT1    PIN_B4

//                           '0'  '1'  '2'  '3'  '4'  '5'  '6'  '7'  '8'  '9'
const char aryCode7Seg[16]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E};
unsigned int32 varCode;
unsigned int32 varCode4,varCode1;
unsigned int16 TIME;
//!unsigned int16 varCodebuzzer, varCodeMTL, varCodeLCD,varCode8L7D;
int1 ttled;
unsigned int32 Led1, Led0;


void main()
{
//!   varCodebuzzer=0b01010000;
//!   write_expanded_outputs_buzzer(&varCodebuzzer);
//!   varCodeMTL=0X0;   //TAT MA TRAN LED
//!   write_expanded_outputs_MTL(&varCodeMTL);
//!   varCodeLCD=0X0;   //TAT LCD
//!   write_expanded_outputs_LCD(&varCodeLCD);
//!   varCode8L7D=0xffffffff;
//!   write_expanded_outputs_8L7D(&varCode8L7D);
   output_low(G_ALL);
   Led1=0b00000000000000000000000000000001;
   Led0=0b00000000000000000000000000000000;
   varCode4 = aryCode7Seg[TIME];
   varCode=0xff|0xff<<8|0xff<<16|varCode4<<24;
   write_expanded_outputs_4L7D(&varCode);
   
while(true)
{
   if(ttled==0)
   {
      write_expanded_outputs_32LED(&Led0);
      delay_ms(TIME*100);
   }
   if(ttled==1)
   {
      write_expanded_outputs_32LED(&Led1);
      delay_ms(TIME*100);
      write_expanded_outputs_32LED(&Led0);
      delay_ms(TIME*100);
   }
   if(input(BT0)==0)
   {
      delay_ms(10);
      if(input(BT0)==0)
      {
         while(input(BT0)==0);
         TIME=TIME+1;
         if(TIME>9) TIME=0;
         varCode4 = aryCode7Seg[TIME];
         varCode=0xff|0xff<<8|0xff<<16|varCode4<<24;
         write_expanded_outputs_4L7D(&varCode);
         varCode = read_eeprom(0x44);
      }
   }
   if(input(BT1)==0)
   {
      delay_ms(10);
      if(input(BT1)==0)
      {
         while(input(BT1)==0);
         ttled=~ttled;
         varCode1 = aryCode7Seg[ttled];
         varCode=varCode1|0xff<<8|0xff<<16|varCode4<<24;
         write_expanded_outputs_4L7D(&varCode);
         if(ttled==0)
         {
            while(ttled==0)
            {
            write_expanded_outputs_32LED(&Led0);
      delay_ms(TIME*100);
            }
         }
         if(ttled==1)
         {
            while(ttled==1)
            {
            write_expanded_outputs_32LED(&Led1);
      delay_ms(TIME*100);
      write_expanded_outputs_32LED(&Led0);
      delay_ms(TIME*100);
            }
         }
      }
   }
   write_eeprom(0x44,varCode);
}
}
