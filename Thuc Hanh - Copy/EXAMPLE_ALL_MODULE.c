#define   BOARD     D501
#include<tv_boards.c> 
#include<tv_glcd.c> 
#include<tv_lcd.c>
#include<tv_ds1307.c> 
unsigned int8 T0;
unsigned int16 nd,kc,sumnd=0,sumkc=0;
unsigned int8 mode=0;
signed int8 n,m,sl=0;
unsigned int8 hieuchinhBCD(unsigned int8 x)   // 2.truong hop tang tu 0x09 len 1 thanh 0x0A nen phai +6 de thanh 0x10
{                                             // 2.truong hop giam tu 0x10 xuong 1 thanh 0x0f nen phai -6 de thanh 0x09
      if((x&0x0f)==0x0a) x+=6;
      if((x&0x0f)==0x0f) x-=6;
      return x;
}
void hieuchinhBCDvaNapthoigian()             // 2.Nap thoi gian vao ds1307
{
      ds1307.second= hieuchinhbcd(ds1307.second);
      ds1307.minute= hieuchinhbcd(ds1307.minute);
      ds1307.hour  = hieuchinhbcd(ds1307.hour);
      ds1307_set_time(ds1307);
}
void cactacvukhac()
{
       T0 = get_timer0();
       if(T0>99) set_timer0(1);
       lcd_write_2x3_num(T0/10,0,1);
       lcd_write_2x3_num(T0%10,3,1);
       set_adc_channel(lm35a_channel); delay_us(20);
       
       sumnd+= read_adc()/2.046;
       if(sl==20){nd=sumnd/20; sumnd=0;}
       lcd_write_2x3_num(nd/10,14,1);
       lcd_write_2x3_num(nd%10,17,1);
       set_adc_channel(gp2d12_channel); delay_us(20);
       sumkc+= read_adc();
       if(sl==20)
       {
          sumkc/=20;
          kc=pow(4277/sumkc,1.115);sumkc=0;
          sl=0;
       }
       else sl++;
       if (kc>80) kc=80;  
       if(kc<40) buzzer_on();
       else      buzzer_off();
       lcd_write_2x3_num(kc/10,7,1);
       lcd_write_2x3_num(kc%10,10,1);

      
      
       if(inputcd(MOD)==0){mode++; mode%=4;}   // mode =0: khong chinh, mode=1:chinh giay, mode=2:phut, mode=3:chinh gio
       if(inputcd(UP)==0)
       {
            if((mode==1)&&(ds1307.second<0x59)) ds1307.second++;
            if((mode==2)&&(ds1307.minute<0x59)) ds1307.minute++;
            if((mode==3)&&(ds1307.hour  <0x23)) ds1307.hour++;
            hieuchinhBCDvaNapthoigian();
       }
       if(inputcd(DW)==0)
       {
            if((mode==1)&&(ds1307.second>0)) ds1307.second--;
            if((mode==2)&&(ds1307.minute>0)) ds1307.minute--;
            if((mode==3)&&(ds1307.hour  >0)) ds1307.hour--;
           hieuchinhBCDvaNapthoigian();
       }
       if(ds1307_read_time()==OK)                   // doc thoi gian tu DS1307
       {
             d7seg.led[0]=m7d[ds1307.second%16]-128*(mode==1);
             d7seg.led[1]=m7d[ds1307.second/16%16];
             d7seg.led[2]=0xbf;
             d7seg.led[3]=m7d[ds1307.minute%16]-128*(mode==2);
             d7seg.led[4]=m7d[ds1307.minute/16%16];
             d7seg.led[5]=0xbf;
             d7seg.led[6]=m7d[ds1307.hour%16]-128*(mode==3);
             d7seg.led[7]=m7d[ds1307.hour/16%16];
             d7seg_display();
       }
}
void sangdanpst()
{
      for(n=32;n>=0;n--)
      {
            led32.ledx32=0xffffffff>>n;
            for(m=0;m<5;m++)cactacvukhac();
            led32_display();
      }
}
void sangdantsp()
{
      for(n=32;n>=0;n--)
      {
            led32.ledx32=0xffffffff<<n;
            for(m=0;m<5;m++)cactacvukhac();
            led32_display();
      }
}
void sangdichpst()
{
      for(n=32;n>=0;n--)
      {
            led32.ledx32=0x80000000>>n;
            for(m=0;m<5;m++)cactacvukhac();
            led32_display();
      }
}
void sangdichtsp()
{
      for(n=32;n>=0;n--)
      {
            led32.ledx32=(int32)0x01<<n;
            for(m=0;m<5;m++)cactacvukhac();
            led32_display();
      }
}
void sangdantrn()
{
      for(n=16;n>=0;n--)
      {
            led32.ledx16[0]=0xffff<<n;
            led32.ledx16[1]=0xffff>>n;
            for(m=0;m<5;m++)cactacvukhac();
            led32_display();
      }
}
void sangdannvt()
{
      for(n=16;n>=0;n--)
      {
            led32.ledx16[1]=0xffff<<n;
            led32.ledx16[0]=0xffff>>n;
            for(m=0;m<5;m++)cactacvukhac();
            led32_display();
      }
}
void main()
{
      system_init();  
      glcd_setup(); 
      lcd_setup(); 
      lcd_data(" COUNT  DIST   TEMP");
      lcd_goto(0,3);
      lcd_data(" (Cai)  (Cm)   ( C)");
      lcd_goto(16,3);
      lcd_data(0xdf);
      glcd_spkt_logo_raw(0);                  
      glcd_update();   
      glcd_text_setup(65,10,1,1,0);
      glcd_text(" Welcome");
      glcd_text_setup(65,30,1,1,0);
      glcd_text(" Pupils");
      glcd_text_setup(65,50,1,1,0);
      glcd_text(" To FEEE");
      glcd_update();
      setup_timer_0(t0_ext_h_to_l|t0_div_1);
      set_timer0(0);
      setup_adc(adc_clock_div_32);
      setup_adc_ports(an0_to_an4|vss_vdd);
      while(true)
      {    
             sangdanpst(); 
             sangdantsp();
             sangdichpst();
             sangdichtsp();
             sangdantrn();
             sangdannvt();
            
      }
}
