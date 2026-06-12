/************************************ Yeu cau ****************************************************************
 1. Bat chuong keu 1s roi tat
 2. Bat den gia nhiet
 3. Do nhiet do LM35A 
 4. Do nhiet do LM35B trung binh 20 lan
 5. Do khoang cach 
 
********************************* Cac lenh lien quan **********************************************************
 1. Lenh cau hinh xung clock cho ADC:
              ----------------------------------------------
                             setup_adc(mode);
              ----------------------------------------------
     .mode = adc_clock_div_X   => Chon nguon xung clock cap cho ADC (X : 2-64)
     .mode = ADC_OFF           => Tat ADC    
 2. Lenh cau hinh cac chan su dung cho bo ADC:
              ----------------------------------------------
                          setup_adc_ports(mode);
              ----------------------------------------------
     . mode = AN0_TO_ANX        => su dung cac chan tu an0 den anx la chan analog
     . mode = VSS_VDD           => du dung dien ap tham chieu noi 0-5V
     . mode = VREF_VREF         => su dung dien ap tham chieu ngoai ( board TH khong ket noi nen khong dung)
     . mode = VREF_VDD          => dien ap tham chieu am lay tu ben ngoai, tham chieu duong 5V
     . mode = VSS_VREF          => dien ap tham chieu am  0V, tham chieu duong lay tu ben ngoai
     . Co the su dung ket hop nhieu lua chon bang lenh OR 
     vd: setup_adc_ports(an0_to_an2|vss_vdd);  
 3. Lenh chon kenh can chuyen doi:
              ----------------------------------------------
                          set_adc_channel(x);
              ----------------------------------------------
     . X la kenh can chuyen doi
     . Sau lenh nay can delay 20us   
 4. Lenh doc ket qua chuyen doi:
               ----------------------------------------------
                           kq = read_adc();
              ----------------------------------------------
     . kq la bien 16 bit   
     
*********************************** Chuong trinh *************************************************************/
#define   BOARD     D501
#include<tv_boards.c> 
#include<tv_lcd.c>
unsigned int16 lm35b,sum=0,lm35a,kc;
unsigned int8 n=0;
#define solan  20
void main()
{
      system_init();  
      lcd_setup();
      lcd_data("    ADC EXAMPLE");
      setup_adc(adc_clock_div_32);
      setup_adc_ports(analog_pins|VSS_VDD);
      buzzer_on(); delay_ms(1000);  // 1.bat chuong
      buzzer_off();                 // 1.tat chuong
      triac_2_on();                 // 2.bat den gia nhiet - de tat den ta dung triac_2_off();
     
      while(true)
      {     // 3.LM35A khong do trung binh 
            set_adc_channel(lm35a_channel); delay_us(20);
            lm35a= read_adc()/2.046;
            
            // 4.LM35B do trung binh 20 lan
            set_adc_channel(lm35b_channel); delay_us(20);
            sum += read_adc();
            n++;
            if(n>=solan)
            { 
               lm35b = sum/solan/2.046;
               n=0; sum=0;
            }
            // 5.Do khoang cach
            set_adc_channel(gp2d12_channel); delay_us(20);
            kc= read_adc();
            kc =pow(4277/kc,1.115);
            if (kc>80) kc=80; 
            else if(kc<10) kc=10;
            
            lcd_goto(0,1);
            printf(lcd_data,"LM35A   :%lu (do C)  ", lm35a);
            lcd_goto(0,2);
            printf(lcd_data,"LM35B   :%lu (do C)  ", lm35b);
            lcd_goto(0,3);
            printf(lcd_data,"Distance :%lu (Cm)  ", kc);      
      }
}

