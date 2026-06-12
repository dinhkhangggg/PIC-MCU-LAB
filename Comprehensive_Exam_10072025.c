#include <18F6722.h>
#device adc=10
#fuses HS, NOWDT, NOPROTECT, NOLVP, MCLR
#use delay(clock=40MHz)

//================= ?NH NGH?A PH?N C?NG =================
#define EXP_OUT_ENABLE_BUZZER   PIN_F4
#define EXP_OUT_CLOCK           PIN_D6
#define EXP_OUT_DO              PIN_D4
#define G_ALL                   PIN_C5
#define EXP_OUT_ENABLE_32LED    PIN_B3
#define EXP_OUT_ENABLE_MA7DOAN  PIN_F6

// ==== ?i tn nt nh?n ====
#define btn0   PIN_B6   // RUN
#define btn1   PIN_F1   // STOP
#define btn2   PIN_B5   // SELECT
#define btn3   PIN_B4   // ADJUST

#define NUMBER_OF_74595_Buzzer   2
#define NUMBER_OF_74595_32_LEDs    4
#define NUMBER_OF_74595_MA7DOAN  8

#include <74595_Buzzer.c>
#include <74595_32_LEDs.c>
#include <74595_MA7DOAN.c>

//================= B?NG M? LED 7 O?N ===================
const int8 array[10] = {0x3F,0x06,0x5B,0x4F,0x66,
                        0x6D,0x7D,0x07,0x7F,0x6F};
#define CHAR_C   0x39
#define CHAR_DEG 0x63
#define CHAR_H   0x76
#define CHAR_O   0x5C
#define CHAR_t   0x78

//================= BI?N TON C?C ========================
unsigned int8 varCode[8];
unsigned int8 varCode0[2];
unsigned int8 varCode1[4];

#define EEPROM_TEMP_ADDR  0x00
#define EEPROM_PV_ADDR    0x01
#define VREF_MV 5000
#define SAMPLES 8

//================= CHNG TR?NH CHNH ===================
void main() {
   int16 adcA, adcB;
   int32 sumA, sumB;
   int16 tempA, tempB;
   int1 run = 0;
   int8 blink = 0;
   int16 totalSP = 0;
   int16 box = 0;
   int8 PV;
   int8 setTemp;
   int8 i;
   int16 tick = 0;    // ?m chu k? m ph?ng xung ?m s?n ph?m

   setup_adc_ports(AN0_TO_AN4 | VSS_VDD);
   setup_adc(ADC_CLOCK_INTERNAL);
   set_tris_a(0xFF);
   setup_timer_0(T0_EXT_L_TO_H | T0_DIV_1);
   set_timer0(0);

   output_low(G_ALL);
   varCode0[0]=0; varCode0[1]=0;

   // ?c d? li?u EEPROM
   setTemp = read_eeprom(EEPROM_TEMP_ADDR);
   if(setTemp<20 || setTemp>80) setTemp = 30;

   PV = read_eeprom(EEPROM_PV_ADDR);
   if(PV<1 || PV>99) PV = 20;

   while(TRUE) {
      // ========== Nt RUN / STOP ==========
      if(!input(btn0)) { delay_ms(20); run=1; while(!input(btn0)); }
      if(!input(btn1)){ delay_ms(20); run=0; while(!input(btn1)); }

      // ========== Nt ci ?t ==========
      if(!input(btn2)) {
         delay_ms(20);
         while(!input(btn2));
         if(!run) output_toggle(PIN_E0);
      }

      if(!input(btn3)) {
         delay_ms(20);
         while(!input(btn3));
         if(!run) {
            if(!input(PIN_E0)) {
               setTemp++;
               if(setTemp>80) setTemp=20;
               write_eeprom(EEPROM_TEMP_ADDR,setTemp);
            } else {
               PV++;
               if(PV>99) PV=1;
               write_eeprom(EEPROM_PV_ADDR,PV);
            }
         }
      }

      // ========== ?c ADC LM35 ==========
      sumA=0; set_adc_channel(0);
      delay_us(20);
      for(int i=0;i<SAMPLES;i++) { sumA+=read_adc(); delay_us(50); }
      adcA=(int16)(sumA/SAMPLES);

      sumB=0; set_adc_channel(4);
      delay_us(20);
      for(i=0;i<SAMPLES;i++) { sumB+=read_adc(); delay_us(50); }
      adcB=(int16)(sumB/SAMPLES);

      tempA = (int16)((((int32)adcA*VREF_MV)/1023)/10);
      tempB = (int16)((((int32)adcB*VREF_MV)/1023)/10);
      if(tempA>99) tempA=99; if(tempB>99) tempB=99;

      // ========== ?m s?n ph?m ==========
      if(run) {
         tick++;
         if(tick >= 3) {          // m ph?ng: c? ~0.9s tng 1 s?n ph?m
            tick = 0;
            totalSP++;
            if(totalSP > 9999) totalSP = 0;
         }
      }

      if(PV != 0)
         box = totalSP / PV;
      else
         box = 0;

      // ========== Hi?n th? ==========
      for(i=0;i<8;i++) varCode[i]=0x00;

      if(tempB >= setTemp) {
         if(blink==0) {
            varCode[7]=CHAR_H;
            varCode[6]=CHAR_O;
            varCode[5]=CHAR_t;
            varCode0[0]=0xFF; varCode0[1]=0xFF;
         } else {
            varCode[7]=varCode[6]=varCode[5]=0x00;
            varCode0[0]=varCode0[1]=0;
         }
         blink^=1;
      } 
      else {
         varCode0[0]=0; varCode0[1]=0;

         // nhi?t ?
         varCode[2] = array[tempB % 10];
         varCode[3] = array[(tempB / 10) % 10];
         varCode[0] = CHAR_C; 
         varCode[1] = CHAR_DEG;

         // s?n ph?m (2 ch? s?)
         varCode[4] = array[(totalSP % 100) % 10];
         varCode[5] = array[((totalSP % 100) / 10) % 10];

         // h?p (2 ch? s?)
         varCode[6] = array[(box % 100) % 10];
         varCode[7] = array[((box % 100) / 10) % 10];
      }

      // LED 32 hi?n th? RUN/STOP
      varCode1[0] = (run) ? 0xAA : 0xFF;
      varCode1[1] = (run) ? 0x55 : 0x00;
      varCode1[2] = (run) ? 0xAA : 0xFF;
      varCode1[3] = (run) ? 0x55 : 0x00;

      write_expanded_outputs_MA7DOAN(varCode);
      write_expanded_outputs_32LED(varCode1);
      write_expanded_outputs_BUZZER(varCode0);

      delay_ms(300);
   }
}

