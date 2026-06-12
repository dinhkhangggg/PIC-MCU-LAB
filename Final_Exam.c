#include <18F6722.h>
#device adc=10
#fuses HS,MCLR
#use delay(clock=40MHz)

#define EXP_OUT_CLOCK           PIN_D6
#define EXP_OUT_DO              PIN_D4
#define G_ALL                   PIN_C5
#define EXP_OUT_ENABLE_32LED    PIN_B3
#define EXP_OUT_ENABLE_MA7DOAN  PIN_F6

#define NUMBER_OF_74595_32_LEDs   4
#define NUMBER_OF_74595_MA7DOAN 8

#define btn1 PIN_F1
#define btn3 PIN_B7

#include <74595_32_LEDs.c>
#include <74595_MA7DOAN.c>

const int8 ma7doan[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

unsigned int8 led32[4];
unsigned int8 varCode[8];
unsigned int32 lm35a=0;
unsigned int8  hta=0;

int1 mode=0;               // 0: Auto, 1: Manual
int1 view_only=0;          // 1: ch? xem nhi?t ? (?ng c t?t)
unsigned int16 cd=100;     // duty m?c ?nh (PWM ?o: cng nh? cng nhanh)
int1 blink=0;              // nhy bargraph ? mode 1
int1 combo_lock=0;         // kha t? h?p nh?n 2 nt

void main() {
   output_low(G_ALL);
   led32[0]=led32[1]=led32[2]=led32[3]=0x00;
   write_expanded_outputs_32LED(led32);

   setup_adc_ports(AN0);
   setup_adc(ADC_CLOCK_INTERNAL);
   setup_ccp1(CCP_PWM);
   setup_timer_2(T2_DIV_BY_16,249,1);

   while(TRUE) {
      unsigned int8 bar = 0;

      set_adc_channel(0);
      delay_us(20);
      lm35a = read_adc();
      delay_ms(5);
      hta = (lm35a * 500) / 1023;

      // --- Phm t? h?p: nh?n gi? ?ng th?i ---
      if(!input(btn1) && !input(btn3)) 
      {
         if(!combo_lock) {
            delay_ms(80);                    // ch?ng d?i t? h?p
            if(!input(btn1) && !input(btn3)) {
               view_only = !view_only;       // ?o ch? ? xem
               combo_lock = 1;
            }
         }
      } else {
         combo_lock = 0;                     // nh? kha khi th? phm
      }

      if(view_only) 
      {
         // T?T H?N ?NG C: t?t PWM + ko RC2 v? Low
         setup_ccp1(CCP_OFF);
         output_high(PIN_C2);
         // 2 LED 7 o?n: nhi?t ?
         varCode[2] = ma7doan[hta/10];
         varCode[1] = ma7doan[hta%10];
         write_expanded_outputs_MA7DOAN(varCode);
         // 8 LED n: t?t h?t
         led32[0]=0x00; led32[1]=led32[2]=led32[3]=0x00;
         write_expanded_outputs_32LED(led32);
         delay_ms(300);
         continue;
      } 
      else 
      {
         // ?m b?o PWM b?t l?i khi thot view_only
         setup_ccp1(CCP_PWM);
         setup_timer_2(T2_DIV_BY_16,249,1);
      }

      // --- Nt chuy?n mode ---
      if(!input(btn1)) 
      {
         delay_ms(20);
         mode = !mode;
         while(!input(btn1));
      }

      // --- Nt tng duty (mode 1) ---
      if(!input(btn3)) {
         delay_ms(20);
         if(mode==1) {
            if(cd==100) cd=300;
            else if(cd==300) cd=600;
            else if(cd==600) cd=900;
            else cd=100;
         }
         while(!input(btn3));
      }

      // --- i?u khi?n PWM (?o: duty nh? = quay nhanh) ---
      if(mode==0) {
         if(hta <= 25)       set_pwm1_duty(900);   // l?nh ? ch?m
         else if(hta <= 30)  set_pwm1_duty(600);
         else if(hta <= 35)  set_pwm1_duty(300);
         else                set_pwm1_duty(100);   // nng ? nhanh

         // Bargraph theo nhi?t (nng ? nhi?u LED)
         {
            int8 n = hta/5; if(n>8) n=8;
            bar = (n==0)?0x00:((1<<n)-1);
         }
         led32[0]=bar; led32[1]=led32[2]=led32[3]=0x00;
      } else {
         set_pwm1_duty(cd);

         // M?c bar theo cd (?o ngh?a: 900 ch?m ? t LED; 100 nhanh ? nhi?u LED)
         if(cd==900)      bar=0x03;
         else if(cd==600) bar=0x0F;
         else if(cd==300) bar=0x3F;
         else             bar=0xFF;

         // Nh?p nhy theo c?p hi?n t?i
         if(blink==0) led32[0]=bar;
         else         led32[0]=0x00;
         blink = !blink;
         led32[1]=led32[2]=led32[3]=0x00;
      }

      // --- Hi?n th? 2 LED 7 o?n: nhi?t ? ---
      varCode[2] = ma7doan[hta/10];
      varCode[1] = ma7doan[hta%10];

      write_expanded_outputs_MA7DOAN(varCode);
      write_expanded_outputs_32LED(led32);
      delay_ms(300);
   }
}

