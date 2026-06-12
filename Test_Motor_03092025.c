#include <18F6722.h>
#fuses HS,NOWDT,NOPROTECT
#use delay(clock=40000000)      // Th?ch anh 40 MHz

void main() {
   int16 duty;

   // --- C?u hình PWM ---
   setup_timer_2(T2_DIV_BY_16, 255, 1);   // Timer2: prescaler 16, period 255
   setup_ccp1(CCP_PWM);                   // CCP1 (RC2) ? ch? d? PWM

   // --- Ban d?u: 50% duty ---
   duty = 512;            // 50% c?a 1023
   set_pwm1_duty(duty);

   // Gi? trong 5 giây
   delay_ms(5000);

   // --- Sau 5 giây: 100% duty ---
   duty = 1023;           // 100% duty
   set_pwm1_duty(duty);

   while(TRUE) {
      // PWM ch?y t? d?ng, không c?n l?p gì thêm
   }
}
