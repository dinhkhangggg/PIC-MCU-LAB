#include<18f6722.h>
#device ADC=10
#fuses HS, NOWDT, NOPROTECT, NOLVP, MCLR
#use delay(clock=40MHz)
#include<lcd.c>

unsigned int16 varDuty,varValue;

void main()
{
//setup---------------------------------------------------------------
set_tris_e(0b00000110);//RE2/AN7
setup_adc_ports(AN0|VSS_VDD);
setup_adc(ADC_CLOCK_INTERNAL);
lcd_init();
set_tris_c(0b11111001);
setup_timer_2(T2_DIV_BY_16,249,1);//Tpwm=2ms
//value=Tpwm*Fosc*Duty/(100*DIV)
setup_ccp1(CCP_PWM);
while(TRUE)
//process-------------------------------------------------------------

{
set_adc_channel(7);
delay_us(20);
varDuty=read_adc()/10.23;
lcd_gotoxy(1,1);
printf(lcd_putc,"PV=%03Lu%%",varDuty);
//varValue=(int16)2*1000*8*varDuty/(100*16);
varValue=(int16)10*varDuty;
set_pwm1_duty(varValue);
}
}
