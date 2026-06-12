#ifndef    __TV_D501_TH_C__
#define    __TV_D501_TH_C__
#include<TV_D501_TH.h>
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxx Module 32 LED don xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
result led32_display()
{
     SSPI_WRITE(led32.ledx8[3]);SSPI_WRITE(led32.ledx8[2]);
     SSPI_WRITE(led32.ledx8[1]);SSPI_WRITE(led32.ledx8[0]);
     output_high(LED32_RCK); output_low(LED32_RCK);
     return OK; 
}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxx Module 7 DOAN TT xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
result d7seg_display()
{
      SSPI_WRITE(d7seg.led[0]);SSPI_WRITE(d7seg.led[1]);
      SSPI_WRITE(d7seg.led[2]);SSPI_WRITE(d7seg.led[3]);
      output_high(D7SEG_RCK); output_low(D7SEG_RCK);
      return OK;
}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxx Module 7 DOAN QUET xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
result s7seg_display()
{
      for(int8 i=0x80,n=0;n<8;n++,i>>=1)
      {
            SSPI_WRITE(i);
            SSPI_WRITE(~s7seg.led[n]);     
            output_high(S7SEG_RCK); output_low(S7SEG_RCK); 
            delay_ms(1);
            SSPI_WRITE(~0xff);
            output_high(S7SEG_RCK); output_low(S7SEG_RCK);
      }
      return OK;
}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxx Module KEY4x4 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
unsigned int8 KEYREAD()
{
     unsigned int8 cot;
     for(cot=0;cot<4;cot++)
     {
        output_bit(pin_b7,cot&0x02);
        output_bit(pin_b6,cot&0x01);
        IF          (!INPUT(PIN_B2))    return (3 + cot*4);             
        ELSE    IF  (!INPUT(PIN_B3))    return (2 + cot*4);               
        ELSE    IF  (!INPUT(PIN_B4))    return (1 + cot*4);            
        ELSE    IF  (!INPUT(PIN_B5))    return (0 + cot*4);
     }
     return 0xff;
}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxx LCD 20x4 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
result private_write_glcd_lcd()
{      
    SSPI_WRITE(system_var.glcdcontrol);             
    SSPI_WRITE(system_var.glcddata);
    SSPI_WRITE(system_var.lcdcontrol);       
    SSPI_WRITE(system_var.lcddata);           
    output_high(LCD_RCK);     
    output_low(LCD_RCK);
    return OK;
}
result write_lcd20x4(unsigned int8 lcd_signal, lcd_ins_hthi)
{     
    system_var.lcdcontrol = ~lcd_signal;
    system_var.lcddata    = ~lcd_ins_hthi;      
    private_write_glcd_lcd();
    return OK;
}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxx GLCD 128x64 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
result write_glcd128x64(unsigned int8 glcd_signal,glcd_ins_hthi)
{     
    system_var.glcdcontrol = ~glcd_signal;
    system_var.glcddata    = ~glcd_ins_hthi;     
    private_write_glcd_lcd();
    return OK;
}
//!//xxxxxxxxxxxxxxxxxxxxxxxxxxxxx LED Matrix xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
result write_ledmatrix(unsigned int16 mahang,unsigned int16 macot1,
unsigned int16 macot2,unsigned int16 macot3)
{     
      SSPI_WRITE(macot3>>8);SSPI_WRITE(macot3);
      SSPI_WRITE(macot2>>8);SSPI_WRITE(macot2);
      SSPI_WRITE(macot1>>8);SSPI_WRITE(macot1);
      SSPI_WRITE(mahang>>8);SSPI_WRITE(mahang);
      output_high(matrix_rck);
      output_low(matrix_rck); 
      return OK;
}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxx TRIAC-BUZZER xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
result private_write_buzzer_relay()
{       
      SSPI_WRITE(system_var.rbdc.xbyte[1]);      
      SSPI_WRITE(system_var.rbdc.xbyte[0]);
      output_high(rbdc_rck);
      output_low(rbdc_rck); 
      return OK;
}
result buzzer_on()
{    
      system_var.rbdc.bit.buzzer=1;
      private_write_buzzer_relay();
      return OK;
}
result buzzer_off()
{     
      system_var.rbdc.bit.buzzer=0;
      private_write_buzzer_relay();  
      return OK;
}
result relay_1_on()
{    
      system_var.rbdc.bit.relay_1=1;
      private_write_buzzer_relay(); 
      return OK;
}
result relay_1_off()
{    
      system_var.rbdc.bit.relay_1=0;
      private_write_buzzer_relay();  
      return OK;
}
result relay_2_on()
{    
      system_var.rbdc.bit.relay_2=1;
      private_write_buzzer_relay(); 
      return OK;
}
result relay_2_off()
{    
      system_var.rbdc.bit.relay_2=0;
      private_write_buzzer_relay(); 
      return OK;
}
result triac_1_on()
{    
      system_var.rbdc.bit.triac_1=1;
      private_write_buzzer_relay(); 
      return OK;
}
result triac_1_off()
{    
      system_var.rbdc.bit.triac_1=0;
      private_write_buzzer_relay();   
      return OK;
}
result triac_2_on()
{    
      system_var.rbdc.bit.triac_2=1;
      private_write_buzzer_relay();
      return OK;
}
result triac_2_off()
{    
      system_var.rbdc.bit.triac_2=0;
      private_write_buzzer_relay();   
      return OK;
}
result DCMOTOR_ENABLE()
{
      system_var.rbdc.bit.DC_MOTOR_ENABLE=1;
      private_write_buzzer_relay();  
      setup_timer_2(t2_div_by_16,249,1);  // Cau hinh xung PWM T=0.8 ms, su dung PWM1
      setup_ccp1(ccp_pwm);  
      setup_ccp2(ccp_pwm);  
      return OK;
}
result DCMOTOR_LOWLEVEL_CONTROL(signed int16 duty, int1 direct)
{
     if(duty)
        if(direct){ set_pwm1_duty((int16)0);  set_pwm2_duty(duty);  }
        else      { set_pwm2_duty((int16)0);  set_pwm1_duty(duty);  }
     else         { set_pwm1_duty((int16)0);  set_pwm2_duty((int16)0);  }
     return OK;
}

result STEPMOTOR_ENABLE()
{
      system_var.rbdc.bit.step_motor_enable=1;
      return OK;
}
result STEPMOTOR_LOWLEVEL_CONTROL(int1 mode, int8 step)
{                                                                      
      const unsigned int8 Step_data[2][8]={0x0e,0x0a,0x0b,0x08,0x01,0x05,0x04,0x06,0x0e,0x0b,0x0d,0x07,0x0e,0x0b,0x0d,0x07};
      system_var.rbdc.bit.step_motor_in1=bit_test(Step_data[mode][step],0);
      system_var.rbdc.bit.step_motor_in2=bit_test(Step_data[mode][step],1);
      system_var.rbdc.bit.step_motor_in3=bit_test(Step_data[mode][step],2);
      system_var.rbdc.bit.step_motor_in4=bit_test(Step_data[mode][step],3);
      private_write_buzzer_relay();
      return OK;
}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxx BOARD_INIT xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
result system_init()
{
     output_high(ALLMODULE_G);  
     set_tris_a(0xff);    set_tris_b(0x3c);                                                   
     set_tris_c(0xf1);    set_tris_e(0x00);                                               
     set_tris_d(0x00);    output_d(0x00); 
     port_b_pullups(0xff); 
     private_write_buzzer_relay();
     led32.ledx32=0;
     led32_display();
     d7seg_display();
     s7seg_display();
     write_lcd20x4(0xff,0xff);
     write_glcd128x64(0xff,0xff);
     write_ledmatrix(0,0,0,0); 
     output_low(ALLMODULE_G);  
     setup_adc(adc_clock_div_32);        
     setup_adc_ports(an0_to_an2|vss_vdd);   
     setup_timer_3(t3_internal|t3_div_by_8);
     return OK;
}
void SSPI_WRITE(unsigned int8 x)
{ 
         if(x&0x80) ssdo=1; 
         else       ssdo=0;
         ssck=0;    ssck=1;   
         if(x&0x40) ssdo=1; 
         else       ssdo=0;
         ssck=0;    ssck=1;  
         if(x&0x20) ssdo=1; 
         else       ssdo=0;
         ssck=0;    ssck=1;  
         if(x&0x10) ssdo=1; 
         else       ssdo=0;
         ssck=0;    ssck=1;  
         if(x&0x08) ssdo=1; 
         else       ssdo=0;
         ssck=0;    ssck=1;  
         if(x&0x04) ssdo=1; 
         else       ssdo=0;
         ssck=0;    ssck=1;  
         if(x&0x02) ssdo=1; 
         else       ssdo=0;
         ssck=0;    ssck=1;  
         if(x&0x01) ssdo=1; 
         else       ssdo=0;
         ssck=0;    ssck=1;       
}
#endif
