#include <18F6722.h> 
#device adc = 10
#fuses HS,NOWDT,NOPROTECT,NOLVP,MCLR
#use delay(clock=40MHz)

#define EXP_OUT_ENABLE_BUZZER  PIN_F4
#define EXP_OUT_CLOCK          PIN_D6
#define EXP_OUT_DO             PIN_D4
#define G_ALL                  PIN_C5
#define EXP_OUT_ENABLE_32LED   PIN_B3
#define EXP_OUT_ENABLE_MA7DOAN PIN_F6

#define NUMBER_OF_74595_Buzzer 2
#define NUMBER_OF_74595_32_LEDs  4
#define NUMBER_OF_74595_MA7DOAN 8

#include <74595_Buzzer.c>
#include <74595_32_LEDs.c>
#include <74595_MA7DOAN.c>

// M? LED 7 o?n cho cathode chung (09)
int8 array[10] = {0x3F,0x06,0x5B,0x4F,0x66,
                  0x6D,0x7D,0x07,0x7F,0x6F};

#define CHAR_C   0x39
#define CHAR_DEG 0x63
#define CHAR_H   0x76
#define CHAR_O   0x5C
#define CHAR_t   0x78

// B? nh? xu?t
unsigned int8 varCode[8];
unsigned int8 varCode0[2];

// Nt nh?n
#define btn0   PIN_B6   // tng
#define btn1   PIN_F1   // gi?m

#define EEPROM_ADDR  0x00  // lu gi tr? c?nh bo

void main()
{
   int16 adc_value;
   int16 tempC;
   int8 tens, unit;
   int blink = 0;
   int8 setTemp;

   // ?c ng?ng c?nh bo t? EEPROM
   setTemp = read_eeprom(EEPROM_ADDR);
   if(setTemp < 20 || setTemp > 80) setTemp = 34; // m?c ?nh n?u rc

   setup_adc_ports(AN0);
   setup_adc(ADC_CLOCK_INTERNAL);
   set_adc_channel(0);

   output_low(G_ALL);
   varCode0[0]=0;
   varCode0[1]=0;

   while(TRUE)
   {
      // ?c ADC -> nhi?t ?
      delay_ms(10);
      adc_value = read_adc();
      tempC = (adc_value) / 2.046;    // ?i sang C (Vref=5V)
      
      if(tempC > 99) tempC = 99;
      if(tempC < 0) tempC = 0;

      // ===== X? l? nt nh?n =====
      if(input(btn0)==0) 
      {         
         delay_ms(10);
            if(setTemp < 99) 
               setTemp++;
            else settemp =99;
            write_eeprom(EEPROM_ADDR,setTemp);
         
      }
      
      if(input(btn1)==0)
      {         // nt gi?m
         delay_ms(10);
         if(setTemp > 0) 
               setTemp--;
         else settemp=0;
         write_eeprom(EEPROM_ADDR,setTemp);
      }

      // ===== Hi?n th? =====
      for(int i=0;i<8;i++) varCode[i]=0x00;  // clear

      if(tempC >= setTemp) 
      {
         // C?nh bo HOT nh?p nhy
         if(blink==0) 
         {
            varCode[4] = CHAR_H;
            varCode[3] = CHAR_O;
            varCode[2] = CHAR_t;
            varCode0[0]=0xFF;  // b?t buzzer
            varCode0[1]=0xFF;
         } 
         else 
         {
            // t?t ? nh?p nhy
            varCode[0]=0x00;
            varCode[1]=0x00;
            varCode[2]=0x00;
            varCode0[0]=0;
            varCode0[1]=0;
         }
         blink ^= 1;
      } 
      else 
      {
         // B?nh th?ng: bn tri = setTemp, bn ph?i = temp o
         varCode0[0]=0; varCode0[1]=0;

         // nhi?t ? ci (setTemp) bn tri (LED6,7)
         tens = setTemp/10; unit = setTemp%10;
         varCode[7] = array[tens];
         varCode[6] = array[unit];
         varCode[4] = CHAR_C;       // LED2: C
         varCode[5] = CHAR_DEG;     // LED3: 

         // nhi?t ? o bn ph?i (LED2,3)
         tens = tempC/10; unit = tempC%10;
         varCode[3] = array[tens];
         varCode[2] = array[unit];
         varCode[0] = CHAR_C;       // LED2: C
         varCode[1] = CHAR_DEG;     // LED3: 
      }

      write_expanded_outputs_MA7DOAN(varCode);
      write_expanded_outputs_BUZZER(varCode0);

      delay_ms(300);
   }
}

