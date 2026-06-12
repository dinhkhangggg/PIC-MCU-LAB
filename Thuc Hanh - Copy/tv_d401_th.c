#ifndef    __TV_D401_TH_C__
#define    __TV_D404_TH_C__
#include<TV_D401_TH.h>
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxx Module 32 LED don xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
result led32_display()
{
      spi_write2(led32.ledx8[3]); spi_write2(led32.ledx8[2]);
      spi_write2(led32.ledx8[1]); spi_write2(led32.ledx8[0]);
      output_high(led32_rck);
      output_low(led32_rck);  
      return OK; 
}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxx Module 7 DOAN TT xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
result d7seg_display()
{
      for(signed int8 n=7;n>=0;n--) spi_write2(~d7seg.led[n]);
      output_high(d7seg_rck);
      output_low(d7seg_rck);
      return OK;
}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxx Module KEY4x4 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
unsigned int8 KEYREAD()
{
     unsigned int8 i,key=0xff;
     for(i = 1; i < 17; i++)
     {
       output_low(keypad_scl);
       delay_us(2);
       if (!input(keypad_sda))key = i; 
       output_high(keypad_scl);   
     } 
     if(key==16) return 0;
     else        return key;  
}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxx LCD 20x4 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
result write_lcd20x4(unsigned int8 lcd_signal, lcd_ins_hthi)
{   
    system_var.lcdcontrol = ~lcd_signal;
    system_var.lcddata    = ~lcd_ins_hthi;  
    spi_write2(system_var.lcdcontrol);
    spi_write2(system_var.lcddata);
    output_high(lcd_rck);
    output_low(lcd_rck); 
    return OK;
}
//!//xxxxxxxxxxxxxxxxxxxxxxxxxxxxx GLCD 128x64 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
result write_glcd128x64(unsigned int8 glcd_signal,glcd_ins_hthi)
{     
    system_var.glcdcontrol = ~glcd_signal;
    system_var.glcddata    = ~glcd_ins_hthi; 
    spi_write2(system_var.glcdcontrol);
    spi_write2(system_var.glcddata);
    output_high(glcd_rck);
    output_low(glcd_rck); 
    return OK;
}
//!//xxxxxxxxxxxxxxxxxxxxxxxxxxxxx LED Matrix xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
result write_ledmatrix(unsigned int16 mahang,unsigned int16 macot1,
unsigned int16 macot2,unsigned int16 macot3)
{     
      spi_write2(macot3>>8);spi_write2(macot3);
      spi_write2(macot2>>8);spi_write2(macot2);
      spi_write2(macot1>>8);spi_write2(macot1);
      spi_write2(mahang>>8);spi_write2(mahang);
      output_high(matrix_rck);
      output_low(matrix_rck); 
      return OK;
}
//!//xxxxxxxxxxxxxxxxxxxxxxxxxxxxx TRIAC-BUZZER xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
result private_write_buzzer_relay()
{       
      spi_write2(system_var.rbdc.xbyte[1]);      
      spi_write2(system_var.rbdc.xbyte[0]);
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
      system_var.rbdc.bit.step_motor_enable=0; 
      return OK;
}
result STEPMOTOR_LOWLEVEL_CONTROL(int1 mode, int8 step)
{                                                                 
      const unsigned int8 Step_data[2][8]={0x8,0x9,0x3,0xf,0x7,0x6,0x4,0x0,0x1,0x2,0x4,0x8,0x1,0x2,0x4,0x8};
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
      output_high(ALLMODULE_G);output_high(keypad_scl);
      setup_spi2(SPI_MASTER|SPI_L_TO_H|SPI_XMIT_L_TO_H);
      set_tris_a(0xff);        set_tris_b(0b11110001);
      set_tris_c(0b11000001);  set_tris_d(0b00001000);
      set_tris_e(0b01011111);  set_tris_f(0b00000111);    
      set_tris_g(0b00010110);  
      private_write_buzzer_relay();
      led32.ledx32=0;
      led32_display();
      d7seg_display();
      write_lcd20x4(0xff,0xff);
      write_glcd128x64(0xff,0xff);
      write_ledmatrix(0,0,0,0);
      output_LOW(ALLMODULE_G);  output_high(pin_d2);
      output_high(pin_c1);      output_high(pin_c2);
      setup_adc(adc_clock_div_32);        
      setup_adc_ports(an0_to_an4|vss_vdd);   
      setup_timer_3(t3_internal|t3_div_by_8);
      return OK;
}
#endif
