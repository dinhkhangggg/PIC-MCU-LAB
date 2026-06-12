/************************************ Yeu cau ****************************************************************
 1. Xuat DAC gia tri dien ap la 2V 
 2. Do nhiet do LM35A trung binh 20 lan va hien thi LCD
 3. Do nhiet do LM35B khong do trung binh hien thi LCD
 4. Khi he thong loi thi hien thi thong bao loi  
 
********************************* Cac lenh lien quan *********************************************************
 1. De doc ket qua chuyen doi ADC ta dung ham:
              -----------------------------------------------------
              result = pcf8591_read_adc(unsigned int8 channel_0_3);
              -----------------------------------------------------
     . Neu result = Ok thi he thong hoat dong tot, khong loi
     . Neu result = ERR thi he thong bi loi 
     . channel_0_3 la kenh tuong tu can chuyen doi ADC - gia tri tu 0 den 3
     . Sau khi doc xong ket qua se duoc lu trong cac bien "pcf8591.adc_value[x]" 
         . Voi x tu 0 den 3 tuong ung voi "channel_0_3"
 2. De chuyen doi DAC ta dung ham:
              -----------------------------------------------
                result = pcf8591_write_dac( int8 value);
              -----------------------------------------------
     . Neu result = Ok thi he thong hoat dong tot, khong loi
     . Neu result = ERR thi he thong bi loi 
     . value la gia tri 8 bit can xuat DAC - khi do gia tri ap ra tinh nhu sau:
              -----------------------------------------------
                        value *(Verf+ - Vref-)
                     V= ---------------------- (V)
                                  256
              -----------------------------------------------
 Vd: Xuat DAC = 2V ta suy ra => value = 2*256/(Vref+ - Vref-) = 2*256/(2.55-0) = 201
 
*********************************** Chuong trinh *************************************************************/
//#define   BOARD     D501
#include<tv_boards.c> 
#include<tv_pcf8591.c>  
#include<tv_lcd.c>
unsigned int16 sum=0;
unsigned int8 lm35b,n=0;
void error_display()                // Nhap nhay thong bao loi 
{
      lcd_command(lcd_clear_display);  delay_ms(500);
      lcd_data("Connection Error...!");delay_ms(500);
}
void main()
{
      system_init();  
      lcd_setup();
      pcf8591_write_dac(201);       // 1. Xuat 2V ra ngo DAC
      while(true)
      {     
            // 2. Do cam bien LM35A trung binh 100 lan va hien thi
            if(pcf8591_read_adc(1)==OK)  
            {
               sum+= pcf8591.adc_value[1];
               if(n>=20)
               {
                   lm35b=sum/20;
                   sum=0;n=0;
                   lcd_write_2x3_num(lm35b/10%10,0,1);
                   lcd_write_2x3_num(lm35b/1%10,3,1);
                   lcd_goto(0,0);lcd_data("   PCF8591 EXAMPLE  ");
                   lcd_goto(0,3);lcd_data("LM35A          LM35B");  
               }
               else n++;
            }
            else error_display();        // 4. Neu he thong loi thi xuat thong bao loi
            
            // 3. Do cam bien LM35B binh thuong va hien thi
            if(pcf8591_read_adc(0)==OK)  
            {
               lcd_write_2x3_num(pcf8591.adc_value[0]/10%10,14,1);
               lcd_write_2x3_num(pcf8591.adc_value[0]/1%10,17,1);
               lcd_goto(0,0);lcd_data("   PCF8591 EXAMPLE  ");
               lcd_goto(0,3);lcd_data("LM35A          LM35B");  
            }
            else error_display();        // 4. Neu he thong loi thi xuat thong bao loi    
      }
}

