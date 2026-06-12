#include <18F6722.h>
#device adc=10
#fuses HS,MCLR
#use delay(clock=40MHz)

#define EXP_OUT_CLOCK           PIN_D6
#define EXP_OUT_DO              PIN_D4
#define G_ALL                   PIN_C5

#define EXP_OUT_ENABLE_MA7DOAN  PIN_F6
#define EXP_OUT_ENABLE_BUZZER   PIN_F4
#define NUMBER_OF_74595_MA7DOAN 8
#define NUMBER_OF_74595_Buzzer  2

#define btn1 PIN_F1   // ch? dng 1 nt nh?n

#include <74595_MA7DOAN.c>
#include <74595_Buzzer.c>

// LED 7 o?n CATHODE CHUNG
const int8 seg_cc[10] = {
   0x3F,0x06,0x5B,0x4F,0x66,
   0x6D,0x7D,0x07,0x7F,0x6F
};

unsigned int8 varCode[8];
unsigned int8 buzzer_data[2];

unsigned int32 adc_val = 0;
unsigned int8 tempC = 0;
int1 mode = 0;              // 0 = hi?n th? nhi?t ?, 1 = hi?n th? c?p
unsigned int8 level = 1;    // c?p t?c ? qu?t (14)
int1 beep_state = 0;
unsigned int16 beep_timer = 0;

void main()
{
   output_low(G_ALL);
   buzzer_data[0] = buzzer_data[1] = 0x00;
   write_expanded_outputs_BUZZER(buzzer_data);

   setup_adc_ports(AN0);
   setup_adc(ADC_CLOCK_INTERNAL);
   setup_ccp1(CCP_PWM);
   setup_timer_2(T2_DIV_BY_16,249,1);

   while(TRUE)
   {
      // ---- ?c c?m bi?n LM35 ----
      set_adc_channel(0);
      delay_us(20);
      adc_val = read_adc();
      delay_ms(5);
      tempC = (adc_val * 500) / 1023;

      // ---- Nt nh?n chuy?n mode ----
      if(!input(btn1))
      {
         delay_ms(80);
         mode = !mode;
         while(!input(btn1)); // ch?ng d?i
      }

      // ================= MODE 0: HI?N TH? NHI?T ? =================
      if(mode == 0)
      {
         // PWM i?u khi?n qu?t t? ?ng theo nhi?t ?
         unsigned int16 duty;
         if(tempC <= 25)       { duty = 900; level = 1; }   // mt, qu?t ch?m
         else if(tempC <= 30)  { duty = 600; level = 2; }   // ?m, qu?t TB
         else if(tempC <= 35)  { duty = 300; level = 3; }   // nng, qu?t nhanh
         else                  { duty = 100; level = 4; }   // r?t nng, qu?t max

         set_pwm1_duty(duty);

         // Hi?n th? nhi?t ? trn LED 7 o?n
         varCode[2] = seg_cc[tempC / 10];
         varCode[1] = seg_cc[tempC % 10];
      }

      // ================= MODE 1: HI?N TH? C?P QU?T =================
      else
      {
         // D?a theo cng cng th?c nhi?t ? ? c?p ?
         if(tempC <= 25)       level = 1;
         else if(tempC <= 30)  level = 2;
         else if(tempC <= 35)  level = 3;
         else                  level = 4;

         // Gi? PWM auto theo level
         switch(level)
         {
            case 1: set_pwm1_duty(900); break;
            case 2: set_pwm1_duty(600); break;
            case 3: set_pwm1_duty(300); break;
            case 4: set_pwm1_duty(100); break;
         }

         // Hi?n th? c?p ? (14)
         varCode[2] = 0x00;              // t?t LED hng ch?c
         varCode[1] = seg_cc[level];     // LED hng n v? hi?n th? s? c?p
      }

      // ---- Buzzer c?nh bo khi >35C ----
      if(tempC > 35)
      {
         beep_timer++;
         if(beep_timer >= 2)  // chu k? ~0.6s
         {
            beep_state = !beep_state;
            beep_timer = 0;
         }

         if(beep_state)
            buzzer_data[0] = 0xFF;
         else
            buzzer_data[0] = 0x00;
      }
      else
      {
         buzzer_data[0] = 0x00;
         beep_timer = 0;
         beep_state = 0;
      }

      write_expanded_outputs_MA7DOAN(varCode);
      write_expanded_outputs_BUZZER(buzzer_data);

      delay_ms(300);
   }
}

