#include <18F6722.h>
#device adc = 10
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
#include <74595_MA7DOAN.c>

// M? 7 o?n cathode chung (09)
int8 array[10] = {0x3F,0x06,0x5B,0x4F,0x66,
                  0x6D,0x7D,0x07,0x7F,0x6F};

unsigned int8 varCode[8];
unsigned int8 varCode0[2];

// ?m
unsigned int16 totalSP = 0;
unsigned int16 BOX = 0;
unsigned int8  SP  = 0;

// Gi tr? ?t tr?c (PV = s? sp/h?p)
unsigned int8 PV;
#define EEPROM_ADDR 0x00

// Nt nh?n
#define BTN_UP    PIN_B6   // tng PV
#define BTN_DOWN  PIN_F1   // gi?m PV

void main() 
{
   output_low(G_ALL);
   varCode0[0]=0; 
   varCode0[1]=0;

   // ?c PV t? EEPROM
   PV = read_eeprom(EEPROM_ADDR);
   if(PV < 1 || PV > 99) PV = 24;   // n?u cha c th? m?c ?nh 24

   // Timer0: Counter mode
   setup_timer_0(T0_EXT_L_TO_H | T0_DIV_1);
   set_timer0(0);

   while(TRUE)
   {
      // ===== ?m s?n ph?m =====
      totalSP = get_timer0();
      BOX = totalSP / PV;
      SP  = totalSP % PV;

      // ===== Nt nh?n ch?nh PV =====
      if(input(BTN_UP)==0) {
         delay_ms(20);
         if(input(BTN_UP)==0) {
            if(PV < 99) PV++;
            write_eeprom(EEPROM_ADDR, PV);
            while(input(BTN_UP)==0); // ch? nh?
         }
      }
      if(input(BTN_DOWN)==0) {
         delay_ms(20);
         if(input(BTN_DOWN)==0) {
            if(PV > 1) PV--;
            write_eeprom(EEPROM_ADDR, PV);
            while(input(BTN_DOWN)==0);
         }
      }

      // ===== Hi?n th? =====
      for(int i=0;i<8;i++) varCode[i]=0x00;

      // S?n ph?m trong h?p (LED01)
      varCode[0] = array[SP % 10];
      varCode[1] = array[(SP / 10) % 10];

      // S? h?p (LED23)
      varCode[2] = array[BOX % 10];
      varCode[3] = array[(BOX / 10) % 10];

      write_expanded_outputs_MA7DOAN(varCode);
      write_expanded_outputs_BUZZER(varCode0);

      delay_ms(100);
   }
}

