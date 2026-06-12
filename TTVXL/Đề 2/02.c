#include <18F4550.h>
#fuses HS,NOMCLR
#use delay(clock=20MHz)

#define EXP_OUT_ENABLE_32LED  PIN_D4
#define EXP_OUT_ENABLE_4L7D  PIN_D6
#define EXP_OUT_CLOCK        PIN_E0
#define EXP_OUT_DO           PIN_E2
#define out1                 PIN_C1
#define out2                 PIN_C2
#define G_ALL                PIN_D0

#define btn_0                PIN_B5
#define btn_1                PIN_B4
#define btn_2                PIN_B3
#define btn_3                PIN_B2

#define NUMBER_OF_74595_32_LEDs 4
#include <74595_32_LEDsDON.c>
#define NUMBER_OF_74595_4L7D 4
#include <74595_4L7D.c>

#define RAND_MAX  9
#include <stdlib.h>

#define EEPROM_ADDR  0x00 

char aryCode7Seg[10] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
unsigned int8 varCode[4];
char led8_tuantu[10] = {0b00000000,
                        0b00000001,
                        0b00000010,
                        0b00000100,
                        0b00001000,
                        0b00010000,
                        0b00100000,
                        0b01000000,
                        0b10000000 };
unsigned int16 captocdo = 0;
unsigned int1 ttdc;
unsigned int8 i;
   int1 flag_btn0 = 0, flag_btn1 = 0, flag_btn2 = 0, flag_btn3 = 0;

void nutnhan() {
   // Ki?m tra cc nt v d?t c?
   if (!input(btn_0)) {flag_btn0 = 1;flag_btn1 = 0;flag_btn2 = 0;flag_btn3 = 0;}
   if (!input(btn_1)) {flag_btn0 = 0;flag_btn1 = 1;flag_btn2 = 0;flag_btn3 = 0;}
   if (!input(btn_2)) {flag_btn0 = 0;flag_btn1 = 0;flag_btn2 = 1;flag_btn3 = 0;}
   if (!input(btn_3)) {flag_btn0 = 0;flag_btn1 = 0;flag_btn2 = 0;flag_btn3 = 1;}
}






void hienthi() {
   varCode[1] = aryCode7Seg[i];  // v d? c? d?nh s? 2
   write_expanded_outputs_32LED(varCode);
}

void main() {
   output_low(G_ALL);
   port_b_pullups(TRUE); // B?t tr? ko ln PORTB
   i  = read_eeprom(EEPROM_ADDR);

   while(TRUE) {
   nutnhan();
   if(flag_btn0 == 1){
   i++;
   if(i == 9) i =1;
   for(unsigned int16 j = 0 ; j < 1000 ; j++)
   {
  
  
      varCode[0] = led8_tuantu[i];
      varCode[1] = aryCode7Seg[i];
//!      varCode[1] = (led_data >> 8) & 0xFF;
//!      varCode[2] = (led_data >> 16) & 0xFF;
//!      varCode[3] = (led_data >> 24) & 0xFF;

      write_expanded_outputs_32LED(varCode);
            varCode[1] = aryCode7Seg[i];
      write_expanded_outputs_4L7D(varCode);
      if(flag_btn1 == 1 || flag_btn2 == 1 ) break;
      delay_us(1);
      
      }
      write_eeprom(EEPROM_ADDR, i);
      }
         if(flag_btn1 == 1){
   i--;
   if(i == 0) i =8;
   for(unsigned int16 p = 0 ; p < 1000 ; p++)
   {
      varCode[0] = led8_tuantu[i];
      varCode[1] = aryCode7Seg[i];
//!      varCode[1] = (led_data >> 8) & 0xFF;
//!      varCode[2] = (led_data >> 16) & 0xFF;
//!      varCode[3] = (led_data >> 24) & 0xFF;

      write_expanded_outputs_32LED(varCode);
            varCode[1] = aryCode7Seg[i];
      write_expanded_outputs_4L7D(varCode);
      if(flag_btn0 == 1 || flag_btn2 == 1 ) break;
      delay_us(1);
      }
      write_eeprom(EEPROM_ADDR, i);
      }
               if(flag_btn2 == 1){
i=i;

      varCode[0] = led8_tuantu[0];
      varCode[1] = 0xff;
//!      varCode[1] = (led_data >> 8) & 0xFF;
//!      varCode[2] = (led_data >> 16) & 0xFF;
//!      varCode[3] = (led_data >> 24) & 0xFF;

      write_expanded_outputs_32LED(varCode);
      delay_ms(1);
      write_eeprom(EEPROM_ADDR, i);
      }
      
}
}

