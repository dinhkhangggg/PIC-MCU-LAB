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

// M LED 7 do?n cho cathode chung (09)
int8 array[10] = {0x3F,0x06,0x5B,0x4F,0x66,
                  0x6D,0x7D,0x07,0x7F,0x6F};
#define CHAR_C   0x39    // 'C'
#define CHAR_DEG 0x63    // '' (n?u khng dng, dng DP: varCode[x] |= 0x80)

unsigned int8 varCode[8];
unsigned int8 varCode0[2];

#define SAMPLES   16

// !!! ?T NG Vref ? Y:
#define VREF_MV   5000   // n?u board 3.3V th d?i thnh 3300

void main()
{
   int16 adc1, adc2;
   int32 sum1, sum2;
   int16 tempA, tempB;   // C (0..99)
   int8 tens, unit;
   int16 i;

   setup_adc_ports(AN0_TO_AN4 | VSS_VDD);  
   setup_adc(ADC_CLOCK_INTERNAL);
   set_tris_a(0xFF);                        

   output_low(G_ALL);
   varCode0[0]=0; varCode0[1]=0;

   while(TRUE)
   {
      // ===== LM35A @ AN0: l?y SAMPLES m?u r?i trung bnh =====
      sum1 = 0;
      set_adc_channel(0);
      delay_us(20);                         // th?i gian n?p t? m?u
      for(i=0; i<SAMPLES; i++)
      {
         sum1 += read_adc();
         delay_us(50);
      }
      adc1 = (int16)(sum1 / SAMPLES);

      // ===== LM35B @ AN1 =====
      sum2 = 0;
      set_adc_channel(4);
      delay_us(20);
      for(i=0; i<SAMPLES; i++)
      {
         sum2 += read_adc();
         delay_us(50);
      }
      adc2 = (int16)(sum2 / SAMPLES);
      {
         int32 mv;
         mv     = ((int32)adc1 * VREF_MV + 511) / 1023;
         tempA  = (int16)((mv + 5) / 10);
         mv     = ((int32)adc2 * VREF_MV + 511) / 1023;
         tempB  = (int16)((mv + 5) / 10);
      }

      if(tempA < 0) tempA = 0; if(tempA > 99) tempA = 99;
      if(tempB < 0) tempB = 0; if(tempB > 99) tempB = 99;

      // Xa t?t c? LED
      for(i=0;i<8;i++) varCode[i]=0x00;

      // ===== Hi?n th? LM35A (gi? mapping gi?ng code chu?n c?a b?n) =====
      tens = tempA / 10;  unit = tempA % 10;
      varCode[2] = array[unit];   // LED0 (don v?)
      varCode[3] = array[tens];   // LED1 (ch?c)
      varCode[0] = CHAR_C;        // LED2 (C)
      varCode[1] = CHAR_DEG;      // LED3 ()  -> n?u sai, thay b?ng: varCode[0] |= 0x80;

      // ===== Hi?n th? LM35B =====
      tens = tempB / 10;  unit = tempB % 10;
      varCode[6] = array[unit];   // LED4
      varCode[7] = array[tens];   // LED5
      varCode[4] = CHAR_C;        // LED6
      varCode[5] = CHAR_DEG;      // LED7

      write_expanded_outputs_MA7DOAN(varCode);
      write_expanded_outputs_BUZZER(varCode0);
      delay_ms(300);
   }
}

