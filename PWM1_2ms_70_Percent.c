#include <18F6722.h>
#fuses HS, NOWDT, NOPROTECT, NOLVP, MCLR
#use delay(clock=40MHz)

void main()
{
//setup-------------------------------------------------
//set_tris_c(0b11111001);
setup_ccp1(CCP_PWM);
setup_timer_2(T2_DIV_BY_16,249,1);
//value=Tpwm*Fosc*Duty/(100*DIV)
set_pwm1_duty(100);
//process-----------------------------------------------
while(TRUE)
{

}
}
