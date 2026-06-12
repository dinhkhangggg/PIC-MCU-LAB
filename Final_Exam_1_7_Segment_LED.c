#include <18F6722.h>
#device adc=10
#fuses HS,MCLR
#use delay(clock=40MHz)

#define EXP_OUT_CLOCK           PIN_D6
#define EXP_OUT_DO              PIN_D4
#define G_ALL                   PIN_C5
#define EXP_OUT_ENABLE_32LED    PIN_B3  
#define EXP_OUT_ENABLE_MA7DOAN  PIN_F6 
#define EXP_OUT_ENABLE_BUZZER   PIN_F4

#define NUMBER_OF_74595_32_LEDs   4
#define NUMBER_OF_74595_MA7DOAN 8
#define NUMBER_OF_74595_Buzzer  2

#define btn1 PIN_f1   // nt chuy?n mode
#define btn3 PIN_b7   // nt tng c?p ?

#include <74595_32_LEDs.c>
#include <74595_MA7DOAN.c>
#include <74595_Buzzer.c>

// LED 7 o?n CATHODE CHUNG (1 = sng)
const int8 seg_cc[10] = {
   0x3F,0x06,0x5B,0x4F,0x66,
   0x6D,0x7D,0x07,0x7F,0x6F
};

unsigned int8 led32[4];
unsigned int8 varCode[8];
unsigned int8 buzzer_data[2];

unsigned int32 lm35 = 0;
unsigned int8 tempC = 0;

int1 mode = 0;        // 0: Mode 1 (Auto theo nhi?t), 1: Mode 2 (Manual)
unsigned int8 level = 1;
int1 blink = 0;

void delay_msx(unsigned int16 t) {
   for (int i=0;i<t;i++) delay_ms(1);
}

void main() {
   output_low(G_ALL);
   buzzer_data[0] = 0x00; buzzer_data[1] = 0x00;
   led32[0]=led32[1]=led32[2]=led32[3]=0x00;

   write_expanded_outputs_BUZZER(buzzer_data);
   write_expanded_outputs_32LED(led32);

   setup_adc_ports(AN0);
   setup_adc(ADC_CLOCK_INTERNAL);
   setup_ccp1(CCP_PWM);
   setup_timer_2(T2_DIV_BY_16,249,1);

   while(TRUE) {
      // ================== Nt chuy?n mode ==================
      if(!input(btn1)) 
      {
         delay_ms(20);
         mode = !mode;
         //while(!input(btn1));
      }

      // ================== Nt tng c?p ? ==================
      if(!input(btn3)) {
         delay_ms(20);
         if(mode==1) {
            level++;
            if(level>4) level=1;
         }
         //while(!input(btn3));
      }

      // ================== MODE 1: i?u khi?n theo nhi?t ? ==================
      if(mode==0) {
         set_adc_channel(0);
         delay_us(20);
         lm35 = read_adc();
         delay_ms(5);
         tempC = (lm35 * 500) / 1023;

         // PWM ng?c: nhi?t cao ? duty nh? ? quay nhanh
         if(tempC <= 25)       set_pwm1_duty(900);
         else if(tempC <= 30)  set_pwm1_duty(600);
         else if(tempC <= 35)  set_pwm1_duty(300);
         else                  set_pwm1_duty(100);

         // Hi?n th? s? MODE
         varCode[0] = seg_cc[1];
         write_expanded_outputs_MA7DOAN(varCode);

         // Hi?n th? nhi?t ? ln LED n:
         // varCode[0-1] = hng n v?, varCode[2-3] = hng ch?c
         unsigned int8 chuc = tempC / 10;
         unsigned int8 donvi = tempC % 10;

         // m?i gi tr? = s? LED sng (t?i a 8)
         unsigned int8 bar_chuc = (chuc==0)?0x00:((1<<chuc)-1);
         unsigned int8 bar_dv   = (donvi==0)?0x00:((1<<donvi)-1);

         led32[0] = bar_dv;
         led32[1] = 0x00;
         led32[2] = bar_chuc;
         led32[3] = 0x00;

         // N?u nhi?t ? > 35: buzzer ku + nhy LED
         if(tempC > 35) {
            if(blink==0) {
               buzzer_data[0]=0xFF;
               led32[0]^=0xFF; // nhy ng?c LED
               led32[2]^=0xFF;
            } else {
               buzzer_data[0]=0x00;
            }
            blink = !blink;
         } else {
            buzzer_data[0]=0x00;
         }

         write_expanded_outputs_32LED(led32);
         write_expanded_outputs_BUZZER(buzzer_data);
         delay_msx(300);
      }

      // ================== MODE 2: Ch?n c?p ? b?ng nt ==================
      else {
         // t?t o nhi?t
         set_pwm1_duty( (level==1)?900 : (level==2)?600 : (level==3)?300 : 100 );

         varCode[0] = seg_cc[2]; // hi?n th? s? mode "2"
         write_expanded_outputs_MA7DOAN(varCode);

         // hi?n th? c?p ? qu?t
         if(level==1)      led32[0]=0xFF, led32[1]=led32[2]=led32[3]=0x00; // 8 LED
         else if(level==2) led32[0]=0xFF, led32[1]=0xFF, led32[2]=led32[3]=0x00; // 16 LED
         else if(level==3) led32[0]=0xFF, led32[1]=0xFF, led32[2]=0xFF, led32[3]=0x00; // 24 LED
         else              led32[0]=led32[1]=led32[2]=led32[3]=0xFF; // 32 LED

         buzzer_data[0]=0x00;
         write_expanded_outputs_32LED(led32);
         write_expanded_outputs_BUZZER(buzzer_data);
         delay_msx(300);
      }
   }
}

