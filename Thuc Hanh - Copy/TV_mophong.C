#ifndef    __TV_MOPHONG_C__
#define    __TV_MOPHONG_C__
#include<TV_MOPHONG.h>
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxx Module 32 LED don xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
result led32_display()
{
     spi_write2(led32.ledx8[3]);spi_write2(led32.ledx8[2]);
     spi_write2(led32.ledx8[1]);spi_write2(led32.ledx8[0]);
     output_high(LED32_RCK); output_low(LED32_RCK);
     return OK;
}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxx Module 7 DOAN TT xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
result d7seg_display()
{
      spi_write2(d7seg.led[4]);spi_write2(d7seg.led[5]);
      spi_write2(d7seg.led[6]);spi_write2(d7seg.led[7]);
      spi_write2(d7seg.led[0]);spi_write2(d7seg.led[1]);
      spi_write2(d7seg.led[2]);spi_write2(d7seg.led[3]);
      output_high(D7SEG_RCK); output_low(D7SEG_RCK);
      delay_ms(8);
      return OK;
}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxx Module 7 DOAN QUET xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
result s7seg_display()
{    
      for(int8 n=0;n<8;n++)
      {
            output_e(s7seg.led[n]); spi_write2(0x80>>n);
            output_high(S7SEG_RCK); output_low(S7SEG_RCK);
            delay_ms(1);
            spi_write2(0);
            output_high(S7SEG_RCK); output_low(S7SEG_RCK);
      }
      return OK;
}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxx Module KEY4x4 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
unsigned int8 KEYREAD()
{   const unsigned int8 mq[]={0xef,0xdf,0xbf,0x7f};
    for(int8 cot=0;cot<4;cot++)
    {
        if(mq[cot]&0x80) output_float(pin_b7);
        else             output_low(pin_b7);
        if(mq[cot]&0x40) output_float(pin_b6);
        else             output_low(pin_b6);
        if(mq[cot]&0x20) output_float(pin_b5);
        else             output_low(pin_b5);
        if(mq[cot]&0x10) output_float(pin_b4);
        else             output_low(pin_b4);
        IF          (!INPUT(pin_b0))    return 0+cot*4;             
        ELSE    IF  (!INPUT(pin_b1))    return 1+cot*4;                  
        ELSE    IF  (!INPUT(pin_b2))    return 2+cot*4;               
        ELSE    IF  (!INPUT(pin_b3))    return 3+cot*4;  
    }
    return 0xff;
}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxx Module LCD xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
result write_lcd20x4(unsigned int8 lcd_signal, lcd_ins_hthi)
{    
    output_bit(LCD_E,bit_test(lcd_signal,0));
    output_bit(LCD_RS,bit_test(lcd_signal,2));
    output_lcd(lcd_ins_hthi);
    return OK;
}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxx TRIAC-BUZZER xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
result buzzer_on(){output_high(BUZZER_PIN);return OK;}
result buzzer_off(){ output_low(BUZZER_PIN);return OK;}
result triac_2_on(){output_high(TRIAC_PIN);return OK;}
result triac_2_off(){output_low(TRIAC_PIN);return OK;}
result DCMOTOR_ENABLE()
{
   setup_timer_2(t2_div_by_16,249,1);  // Cau hinh xung PWM T=0.8 ms, su dung PWM1
   setup_ccp1(ccp_pwm);  
   set_pwm1_duty((int16)0) ;
   output_low(DCMOTOR_IN1) ;
   return OK;
}

result DCMOTOR_LOWLEVEL_CONTROL(signed int16 duty, int1 direct)
{
     if(duty)
        if(direct){ output_high(DCMOTOR_IN1); set_pwm1_duty(1000-duty);  }
        else      { output_low(DCMOTOR_IN1) ; set_pwm1_duty(duty)     ;  }
     else         { output_low(DCMOTOR_IN1) ; set_pwm1_duty((int16)0) ;  }
     return OK;
}
result STEPMOTOR_ENABLE(){return OK;}
result STEPMOTOR_LOWLEVEL_CONTROL(int1 mode, int8 step)
{                                                                      
      const unsigned int8 Step_data[2][8]={0x01,0x03,0x02,0x06,0x04,0x0c,0x08,0x09,0x01,0x02,0x04,0x08,0x01,0x02,0x04,0x08};
      output_bit(STEPMOTOR_IN0, bit_test(step_data[mode][step],0));
      output_bit(STEPMOTOR_IN1, bit_test(step_data[mode][step],1));
      output_bit(STEPMOTOR_IN2, bit_test(step_data[mode][step],2));
      output_bit(STEPMOTOR_IN3, bit_test(step_data[mode][step],3));
      return OK;
}

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxx BOARD_INIT xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
result system_init()
{
      CHECK_MODULE();
      set_tris_a(0xdf);
      set_tris_d(0x04);
      set_tris_e(0);
      set_tris_f(0);
      set_tris_g(0);
      set_tris_c(0xff);
      port_b_pullups(0xFF);
      output_g(0);
      output_high(pin_g0);
      led32.ledx32=0;
      led32_display();
      d7seg_display();
      s7seg_display();
      buzzer_off(); 
      triac_2_off();
      setup_timer_3(t3_internal|t3_div_by_8|T3_CCP2_TO_5);
      return OK;
}
/*********************************** Check module ************************************************************/
result CHECK_MODULE()
{
    int1 tt=1;
      unsigned int8 n;
      for(n=0;n<10;n++)
      {     output_bit(pin_d0,tt);
            if(input(pin_d1) !=tt)break;
            tt=!tt;
      }
      if(n<10) while(true)
      {    output_d(0xff);
           output_b(0xff); delay_ms(10);
           output_b(0); delay_ms(10);
      }
      return OK;
}
#endif
